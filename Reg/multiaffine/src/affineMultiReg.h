#ifndef _affineMultiReg_H_
#define _affineMultiReg_H_

#include "RegistrationBase.h"





namespace itk {


class AffineMultiReg : public AssignPointer
{


public:
//    std::string m_fixedFile;
//    std::string m_movingFile;
//    std::string m_outputFile;
//    std::string m_outputDeformFile;
//    unsigned int m_levelNumber;
//    std::vector<unsigned int> m_gridNode;
//    std::vector<unsigned int> m_downSample;
//    std::vector<unsigned int> m_maxIteration;

//    std::vector<Metric_Type> m_metricType;
//    std::vector<InterPolator_Type> m_interpolatorType;
//    std::vector<Opt_Type> m_optimizerType;

    /* Standard calss typedefs */
    typedef AffineMultiReg Self;
    //    typedef itk::Object Superclass;
    //    typedef SmartPointer< Self > Pointer;

    static Self * New()
    {
        return new Self;
    }

    /* Method for creation through the object factoAddry */
    //    itkNewMacro(Self);

    /* Runtime type information */
    itkTypeMacro(AffineMultiReg, AssignPointer);


    void Initialize(Global &var);
    /* Update the filter with image type*/
    void Update(Global &var, std::vector<Parameters> &par);
    void Write(Global &var);

    AffineMultiReg():AssignPointer(){};
    //    AffineMultiReg(const std::string &file1, const std::string &file2)
    //        :AssignPointer()
    //    {
    ////    this->fixedReader = _fixedReader;
    //    };
    ~AffineMultiReg(){};




protected:
    /* Internal function */
    void InitializeTransform(const unsigned int &level);

    //    void SetTransform();

    void BuildTransform() const;

    int Run();

    void GoToNextResolution() const;



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

    RegistrationType::Pointer _registration;

    std::vector<TImage::Pointer> m_fixedImage;
    std::vector<TImage::Pointer> m_movingImage;

    unsigned int m_currentLevel;

    //    std::vector<AffineTransformType::Pointer> m_transform;
    typedef AffineTransformType::ParametersType     ParametersType;
    ParametersType m_currentParameters;


};


}



#endif
