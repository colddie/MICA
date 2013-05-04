#ifndef REGISTERATIONTYPE_H
#define REGISTERATIONTYPE_H

#include "enumerationType.h"
#include "readGlobal.h"
#include "readStages.h"

#include "itkImageRegistrationMethod.h"
//#include "itkMultiResolutionImageRegistrationMethod.h"

#include "itkMeanSquaresImageToImageMetric.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkMattesMutualInformationImageToImageMetric.h"

#include "itkTranslationTransform.h"
#include "itkVersorRigid3DTransform.h"
#include "itkQuaternionRigidTransform.h"
#include "itkAffineTransform.h"
#include "itkBSplineTransform.h"

#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"

#include "itkLBFGSOptimizer.h"
#include "itkLBFGSBOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkAmoebaOptimizer.h"
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkQuaternionRigidTransformGradientDescentOptimizer.h"

#include "itkResampleImageFilter.h"
#include "itkBSplineResampleImageFunction.h"




class AssignPointer: public itk::Object
{
public:

    // public member from parameter class

    AssignPointer()
    {};
    virtual ~AssignPointer()
    {};


protected:

    // define for derived class only
    //    std::string m_fixedFile;
    //    std::string m_movingFile;
    //    std::string m_outputFile;
    //    std::string m_outputDeformFile;
    //    unsigned int m_levelNumber;
    //    std::vector<unsigned int> m_gridNode;
    //    std::vector<unsigned int> m_downSample;
    //    std::vector<unsigned int> m_maxIteration;

    //    std::vector<Transform_Type> m_transformType;
    //    std::vector<Metric_Type> m_metricType;
    //    std::vector<InterPolator_Type> m_interpolatorType;
    //    std::vector<Optimizer_Type> m_optimizerType;

    // general type and global variables
    typedef double InputType;
    typedef itk::Image<InputType, 3> TImage;
    typedef TImage::Pointer ImagePointer;

    typedef itk::ImageRegistrationMethod<TImage, TImage> RegistrationType;

    typedef itk::TranslationTransform<InputType> TranslationTransformType;
    typedef itk::VersorRigid3DTransform<InputType> Versor3DTransformType;
    typedef itk::QuaternionRigidTransform<InputType> QuaternionTransformType;
    typedef itk::AffineTransform<InputType> AffineTransformType;
    typedef itk::BSplineTransform<InputType, 3, 3> BSplineTransformType;

    typedef itk::VersorRigid3DTransformOptimizer VersorOptimizerType;
    typedef itk::QuaternionRigidTransformGradientDescentOptimizer QuaternionOptimizerType;
    typedef itk::AmoebaOptimizer AmoebaOptimizerType;
    typedef itk::RegularStepGradientDescentOptimizer RSGDOptimizerType;
    typedef itk::LBFGSOptimizer LBFGSOptimizerType;
    typedef itk::LBFGSBOptimizer LBFGSBOptimizerType;

    // protected function for derived class
    //    int SetRegistration(const unsigned int &thread_num);
    void SetImages(const RegistrationType::Pointer registration,
                   const ImagePointer fixed,
                   const ImagePointer moving) const;

    void SetTransform(const RegistrationType::Pointer registration,
                      const Transform_Type &transformType) const;
    void SetMetric(const RegistrationType::Pointer registration,
                   const Metric_Type &metricType) const;
    void SetInterpolator(const RegistrationType::Pointer registration,
                         const InterPolator_Type &interpolatorType) const;
    void SetOptimizer(const RegistrationType::Pointer registration,
                      const Optimizer_Type &optType) const;

    void InitializePyramid(const unsigned int &level,
                           const std::string &fixedfile,
                           const std::string &movingfile,
                           const std::vector<unsigned int> &downsample,
                           std::vector<TImage::Pointer> &fixedImageArray,
                           std::vector<TImage::Pointer> &movingImageArray);

    void BuildOptimizer(const RegistrationType::Pointer registration,
//                        const Optimizer_Type &opttype,
                        const Parameters &par) const;

    void SetScales(const RegistrationType::Pointer registration,
                   const Transform_Type &transformtype) const;

    void AddObserver(const RegistrationType::Pointer registration,
                     const Optimizer_Type &opttype) const;

    void WarpToImage(const RegistrationType::Pointer registration,
                     const TImage::Pointer fixedimage,
                     const TImage::Pointer movingimage,
                     const std::string outputfile
                     ) const;

    void GenerateDeformation(const RegistrationType::Pointer registration,
                             const TImage::Pointer fixedimage,
                             const TImage::Pointer movingimage,
                             const std::string outputdeformfile
                             ) const;

public:

    // public function for derived class

    // overload in derived class
    //    virtual void SetTransform() = 0;
    virtual void Initialize(Global &var) = 0;
    virtual void Update(Global &var, std::vector<Parameters> &par) = 0;
    virtual void Write(Global &var) = 0;


private:

    typedef itk::MeanSquaresImageToImageMetric<TImage, TImage> MSEMetricType;
    typedef itk::MutualInformationImageToImageMetric<TImage, TImage> MIMetricType;
    typedef itk::MattesMutualInformationImageToImageMetric<TImage, TImage> MattesMIMetricType;

    typedef itk::LinearInterpolateImageFunction<TImage, InputType> LinearInterpolatorType;
    typedef itk::NearestNeighborInterpolateImageFunction<TImage, InputType> NearestNeighbourInterpolatorType;
    typedef itk::BSplineInterpolateImageFunction<TImage, InputType, InputType> BSplineInterpolatorType;



};




#endif // REGISTERATIONTYPE_H
