#include "RegistrationBase.h"

#include "commandIterationUpdate.h"
#include "registrationInterfaceCommand.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMultiResolutionPyramidImageFilter.h"
#include "itkTimeProbesCollectorBase.h"
#include "itkMemoryProbesCollectorBase.h"


using namespace itk;


///* Initilize the Registration */
//int AssignPointer::SetRegistration(const unsigned int &thread_num)
//{
//    _registration = RegistrationType::New();

//    if (thread_num>4) {
//        return EXIT_FAILURE;
//    }
//    else {
//        _registration->SetNumberOfThreads(thread_num);
//    }

//}



/* Read Inputs */
void AssignPointer::SetImages(const RegistrationType::Pointer registration,
                    const ImagePointer fixed, const ImagePointer moving) const
{
    registration->SetFixedImage(fixed);
    registration->SetMovingImage(moving);
    registration->SetFixedImageRegion(fixed->GetBufferedRegion());
}



/* Set Transform */
void AssignPointer::SetTransform(const RegistrationType::Pointer registration,
                                 const Transform_Type &transformType) const
{
    switch (transformType) {
    case TRANSFORM_TRANSLATION: {
        TranslationTransformType::Pointer transform = TranslationTransformType::New();
        registration->SetTransform(transform);break;
    }
    case TRANSFORM_VERSOR3D: {
        Versor3DTransformType::Pointer transform = Versor3DTransformType::New();
        registration->SetTransform(transform);break;
    }
    case TRANSFORM_QUATERNION: {
        QuaternionTransformType::Pointer transform = QuaternionTransformType::New();
        registration->SetTransform(transform);break;
    }
    case TRANSFORM_AFFINE: {
        AffineTransformType::Pointer transform = AffineTransformType::New();
        registration->SetTransform(transform);break;
    }
    case TRANSFORM_BSPLINE: {
        BSplineTransformType::Pointer transform = BSplineTransformType::New();
        registration->SetTransform(transform);break;
    }
    default:
        return;
}
}


/* Set Metric */
void AssignPointer::SetMetric(const RegistrationType::Pointer registration,
                              const Metric_Type &metricType) const
{
    switch (metricType) {
    case METRIC_MSE: {
        MSEMetricType::Pointer metric = MSEMetricType::New();
        registration->SetMetric(metric);break;
    }
    case METRIC_MI: {
        /*  The metric requires a number of parameters to be
        selected, including the standard deviation of the
        Gaussian kernel for the fixed image density estimate,
        the standard deviation of the kernel for the moving
        image density and the number of samples use to compute
        the densities and entropy values. Details on the
        concepts behind the computation of the metric can be
        found in Section \ref{sec:MutualInformationMetric}.
        Experience has shown that a kernel standard deviation
        of $0.4$ works well for images which have been
        normalized to a mean of zero and unit variance.  We
        will follow this empirical rule in this example. */
        MIMetricType::Pointer metric = MIMetricType::New();
        metric->SetFixedImageStandardDeviation( 0.4 );
        metric->SetMovingImageStandardDeviation( 0.4 );
        registration->SetMetric(metric);break;
    }
    case METRIC_MATTESMI: {
        /*  The metric requires two parameters to be selected: the
        number of bins used to compute the entropy and the
        number of spatial samples used to compute the density
        estimates. In typical application, 50 histogram bins
        are sufficient and the metric is relatively
        insensitive to changes in the number of bins. The
        number of spatial samples to be used depends on the
        content of the image. If the images are smooth and do
        not contain much detail, then using approximately $1$
        percent of the pixels will do. On the other hand, if
        the images are detailed, it may be necessary to use a
        much higher proportion, such as $20$ percent. */
        MattesMIMetricType::Pointer metric = MattesMIMetricType::New();
        metric->SetNumberOfHistogramBins(20);
        metric->SetNumberOfSpatialSamples(10000);
        registration->SetMetric(metric);break;
    }
    default:
        return;
    }

}


/* Set Interpolator */
void AssignPointer::SetInterpolator(const RegistrationType::Pointer registration,
                                    const InterPolator_Type &interpolatorType) const
{
    switch (interpolatorType) {
    case INTERPOLATOR_LINEAR: {
        LinearInterpolatorType::Pointer interpolator = LinearInterpolatorType::New();
        registration->SetInterpolator(interpolator);break; }
    case INTERPOLATOR_NEARESTNEIGHBOUR: {
        NearestNeighbourInterpolatorType::Pointer interpolator = NearestNeighbourInterpolatorType::New();
        registration->SetInterpolator(interpolator);break; }
    case INTERPOLATOR_BSPLINEINTER: {
        BSplineInterpolatorType::Pointer interpolator = BSplineInterpolatorType::New();
        registration->SetInterpolator(interpolator);break; }
    default:
        return;
}
}


/* Set Optimizer */
void AssignPointer::SetOptimizer(const RegistrationType::Pointer registration,
                                 const Optimizer_Type &optType) const
{
    switch (optType) {
    case OPTIMIZER_VERSOR3D: {
        VersorOptimizerType::Pointer optimizer = VersorOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    case OPTIMIZER_QUATERNION: {
        QuaternionOptimizerType::Pointer optimizer = QuaternionOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    case OPTIMIZER_AMOEBA: {
        AmoebaOptimizerType::Pointer optimizer = AmoebaOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    case OPTIMIZER_RSGD: {
        RSGDOptimizerType::Pointer optimizer = RSGDOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    case OPTIMIZER_LBFGS: {
        LBFGSOptimizerType::Pointer optimizer = LBFGSOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    case OPTIMIZER_LBFGSB: {
        LBFGSBOptimizerType::Pointer optimizer = LBFGSBOptimizerType::New();
        registration->SetOptimizer(optimizer); break;}
    default:
        return;
    }
}


/* Set Pyramid */
void AssignPointer::InitializePyramid(const unsigned int &level,const std::string &fixedfile,
                                      const std::string &movingfile,const std::vector<unsigned int> &downsample,
                                      std::vector<TImage::Pointer> &fixedImageArray,
                                      std::vector<TImage::Pointer> &movingImageArray
                                      )
{
    typedef ImageFileReader<TImage> ReaderType;
    ReaderType::Pointer fixedReader = ReaderType::New();
    ReaderType::Pointer movingReader = ReaderType::New();
    fixedReader->SetFileName(fixedfile);
    movingReader->SetFileName(movingfile);
    fixedReader->Update();
    movingReader->Update();

    typedef MultiResolutionPyramidImageFilter<TImage, TImage> ImagePyramidType;
    ImagePyramidType::Pointer fixedImagePyramid = ImagePyramidType::New();
    ImagePyramidType::Pointer movingImagePyramid = ImagePyramidType::New();

    fixedImagePyramid->SetInput(fixedReader->GetOutput());
    movingImagePyramid->SetInput(movingReader->GetOutput());

    //  Set Pyramids
    fixedImagePyramid->SetNumberOfLevels(level);
    movingImagePyramid->SetNumberOfLevels(level);

    Array2D<unsigned int> resolution(level,3);
    //    resolution(0,0) = 4;
    //    resolution(0,1) = 4;
    //    resolution(0,2) = 4;
    //    resolution(1,0) = 2;
    //    resolution(1,1) = 2;
    //    resolution(1,2) = 2;
    //    resolution(2,0) = 1;
    //    resolution(2,1) = 1;
    //    resolution(2,2) = 1;
    for (unsigned int i=0; i<level; i++) {
        for (unsigned int j=0; j<3; j++) {
            resolution(i,j) = downsample[i*3+j];
        }
    }

    fixedImagePyramid->SetSchedule(resolution);
    movingImagePyramid->SetSchedule(resolution);
    //  registration->SetSchedules(resolution, resolution);

    fixedImagePyramid->Update();
    movingImagePyramid->Update();

    // provided that last resolution was not 1 1 1, store fixedimage and moving image
    fixedImageArray.reserve(level+1);
    movingImageArray.reserve(level+1);

    for (unsigned int i=0; i<level; i++) {
        TImage::Pointer fixedimage = TImage::New();
        TImage::Pointer movingimage = TImage::New();
        fixedimage = fixedImagePyramid->GetOutput(i);
        movingimage = movingImagePyramid->GetOutput(i);
        fixedImageArray.push_back(fixedimage);
        movingImageArray.push_back(movingimage);
    }
    fixedImageArray.push_back(fixedReader->GetOutput());
    movingImageArray.push_back(movingReader->GetOutput());

    for (unsigned int i=0; i<level; i++) {
    std::cout<<fixedImageArray[i]->GetLargestPossibleRegion().GetSize()<<fixedImageArray[i]->GetSpacing()<<std::endl;
    }
}


/* Set optimizer */
void AssignPointer::BuildOptimizer(const RegistrationType::Pointer registration,
//                                   const Optimizer_Type &opttype,
                                   const Parameters &par) const
{
    switch (par.optimizertype) {
    case OPTIMIZER_VERSOR3D:
    {
        typedef VersorOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        optimizer->SetMaximumStepLength(0.5);
        optimizer->SetMinimumStepLength(0.0001);
        optimizer->SetNumberOfIterations(par.maximumiteration); break;
    }

    case OPTIMIZER_QUATERNION:
    {
        typedef QuaternionOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        optimizer->SetLearningRate(0.01);
        optimizer->SetNumberOfIterations(par.maximumiteration); break;
    }

    case OPTIMIZER_AMOEBA:
    {
        typedef AmoebaOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        optimizer->SetParametersConvergenceTolerance(0.05);
        optimizer->SetFunctionConvergenceTolerance(10000);  // Was 10000
        optimizer->SetMaximumNumberOfIterations(par.maximumiteration); break;
    }

    case OPTIMIZER_RSGD:
    {
        typedef RSGDOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        optimizer->SetMaximumStepLength(1);
        optimizer->SetMinimumStepLength(0.001);
        optimizer->SetNumberOfIterations(par.maximumiteration); break;
        optimizer->SetRelaxationFactor(0.5);

    }

    case OPTIMIZER_LBFGS:
    {
        //  Next we set the parameters of the LBFGS Optimizer
        typedef LBFGSOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());

        optimizer->SetGradientConvergenceTolerance( 0.1 );
        optimizer->SetLineSearchAccuracy( 0.9 );
        optimizer->SetDefaultStepLength( 1.5 );
        optimizer->TraceOn();
        optimizer->SetMaximumNumberOfFunctionEvaluations( 1000 ); break;
    }

    case OPTIMIZER_LBFGSB:
    {
        //        typedef LBFGSBOptimizerType OptimizerType;
        //        typedef OptimizerType::Pointer OptimizerPointer;
        typedef LBFGSBOptimizerType *OptimizerPointer;

        //  Next we set the parameters of the LBFGSB Optimizer
        LBFGSBOptimizerType::BoundSelectionType boundSelect( registration->GetTransform()->GetNumberOfParameters() );
        LBFGSBOptimizerType::BoundValueType upperBound( registration->GetTransform()->GetNumberOfParameters() );
        LBFGSBOptimizerType::BoundValueType lowerBound( registration->GetTransform()->GetNumberOfParameters() );

        boundSelect.Fill( 0 );
        upperBound.Fill( 0.0 );
        lowerBound.Fill( 0.0 );

        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        optimizer->SetBoundSelection( boundSelect );
        optimizer->SetUpperBound( upperBound );
        optimizer->SetLowerBound( lowerBound );

        //
        optimizer->SetCostFunctionConvergenceFactor (1e+7);
        optimizer->SetProjectedGradientTolerance (1e-4);
        //optimizer->SetProjectedGradientTolerance (1.5);
        optimizer->SetMaximumNumberOfIterations (par.maximumiteration);
        optimizer->SetMaximumNumberOfEvaluations (par.maximumiteration);
        optimizer->SetMaximumNumberOfCorrections (12); break;
    }

    }
}


/* Set the optimizer scales */
void AssignPointer::SetScales(const RegistrationType::Pointer registration,
                              const Transform_Type &transformtype) const
{
    switch (transformtype) {
    case TRANSFORM_VERSOR3D:
    {
        double rotation_scale, translation_scale;
        Array<double> optimizerScales(6);

        rotation_scale = 1.0;
        // translation_scale = 1.0 / 10000.0;
        translation_scale = 1.0 / 20000.0;

        optimizerScales[0] = rotation_scale;
        optimizerScales[1] = rotation_scale;
        optimizerScales[2] = rotation_scale;
        optimizerScales[3] = translation_scale;
        optimizerScales[4] = translation_scale;
        optimizerScales[5] = translation_scale;

        registration->GetOptimizer()->SetScales(optimizerScales); break;
    }

    case TRANSFORM_QUATERNION:
    {
        double rotation_scale, translation_scale;
        Array<double> optimizerScales(7);

        rotation_scale = 1.0;
        translation_scale = 1.0 / 10000.0;

        /* GCS FIX: Changing the scale fudge_factor is one way to avoid
               ITK "Too many samples..." problem */
        //double fudge_factor = 1000000.0;
        double fudge_factor = 1.0;
        rotation_scale = rotation_scale * fudge_factor;
        translation_scale = translation_scale * fudge_factor;

        optimizerScales[0] = rotation_scale;
        optimizerScales[1] = rotation_scale;
        optimizerScales[2] = rotation_scale;
        optimizerScales[3] = rotation_scale;
        optimizerScales[4] = translation_scale;
        optimizerScales[5] = translation_scale;
        optimizerScales[6] = translation_scale;

        registration->GetOptimizer()->SetScales(optimizerScales); break;
    }

    case TRANSFORM_AFFINE:
    {
        itk::Array<double> optimizerScales(12);

        const double matrix_scale = 1.0;
        //const double translationScale = 1.0 / 10000.0;
        const double translation_scale = 1.0 / 100000.0;
        //const double translation_scale = 1.0 / 1000000.0;
        optimizerScales[0] = matrix_scale;
        optimizerScales[1] = matrix_scale;
        optimizerScales[2] = matrix_scale;
        optimizerScales[3] = matrix_scale;
        optimizerScales[4] = matrix_scale;
        optimizerScales[5] = matrix_scale;
        optimizerScales[6] = matrix_scale;
        optimizerScales[7] = matrix_scale;
        optimizerScales[8] = matrix_scale;
        optimizerScales[9] = translation_scale;
        optimizerScales[10] = translation_scale;
        optimizerScales[11] = translation_scale;

        registration->GetOptimizer()->SetScales(optimizerScales);
    }
    }
}


/*   Create the Command observer and register it with the optimizer  */
void AssignPointer::AddObserver(const RegistrationType::Pointer registration,
                                const Optimizer_Type &opttype ) const
{
    switch (opttype) {
    case OPTIMIZER_VERSOR3D:
    {
        typedef VersorOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        typedef CommandIterationUpdate<VersorOptimizerType> UpdateType;
        UpdateType::Pointer observer = UpdateType::New();
        observer->SetOptimizer(optimizer);
        optimizer->AddObserver(IterationEvent(), observer); break;
    }

    case OPTIMIZER_QUATERNION:
    {
        typedef QuaternionOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        typedef CommandIterationUpdate<QuaternionOptimizerType> UpdateType;
        UpdateType::Pointer observer = UpdateType::New();
        observer->SetOptimizer(optimizer);
        optimizer->AddObserver(IterationEvent(), observer); break;
    }

    case OPTIMIZER_AMOEBA:
    {
//        typedef AmoebaOptimizerType *OptimizerPointer;
//        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
//        typedef CommandIterationUpdate<AmoebaOptimizerType> UpdateType;
//        UpdateType::Pointer observer = UpdateType::New();
//        observer->SetOptimizer(optimizer);
//        optimizer->AddObserver(IterationEvent(), observer); break;
    }

    case OPTIMIZER_RSGD:
    {
        typedef RSGDOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        typedef CommandIterationUpdate<RSGDOptimizerType> UpdateType;
        UpdateType::Pointer observer = UpdateType::New();
        observer->SetOptimizer(optimizer);
        optimizer->AddObserver(IterationEvent(), observer); break;
    }

    case OPTIMIZER_LBFGS:
    {
//        typedef LBFGSOptimizerType *OptimizerPointer;
//        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(_registration->GetOptimizer());
//        typedef CommandIterationUpdate<LBFGSOptimizerType> UpdateType;
//        UpdateType::Pointer observer = UpdateType::New();
//        observer->SetOptimizer(optimizer);
//        optimizer->AddObserver(IterationEvent(), observer); break;

    }

    case OPTIMIZER_LBFGSB:
    {
        typedef LBFGSBOptimizerType *OptimizerPointer;
        OptimizerPointer optimizer = dynamic_cast<OptimizerPointer>(registration->GetOptimizer());
        typedef CommandIterationUpdate<LBFGSBOptimizerType> UpdateType;
        UpdateType::Pointer observer = UpdateType::New();
        observer->SetOptimizer(optimizer);
        optimizer->AddObserver(IterationEvent(), observer); break;

    //    CommandIterationUpdate<LBFGSBOptimizerType>::Pointer observer =
    //            CommandIterationUpdate<LBFGSBOptimizerType>::New();
    //    observer->SetOptimizer(_registration->GetOptimizer());
    //    observer->AddStartObserver();
    //    observer->AddIterationObserver();
    //    observer->AddEndObserver();  // no need to call this
    }


//    // Create the Command interface observer and register it with the optimizer
//    RegistrationInterfaceCommand<RegistrationType, LBFGSBOptimizerType>::Pointer command =
//            RegistrationInterfaceCommand<RegistrationType, LBFGSBOptimizerType>::New();
//    command->SetRegistration(_registration,_registration->GetOptimizer());
//    command->AddResolutionObserver();
}
}


/* Write the warped image */
void AssignPointer::WarpToImage(const RegistrationType::Pointer registration, const TImage::Pointer fixedimage,
                                const TImage::Pointer movingimage, const std::string outputfile
                                ) const
{
    std::cout<<"Warping to image"<<std::endl;
    //  typename TransformType::Pointer finalTransform = TransformType::New();
    //  finalTransform->SetParameters(finalParameters);
    //  finalTransform->SetFixedParameters(transform_1->GetFixedParameters());

    //Resample the m_movingImage to m_fixedImage to see the difference
    typedef Image<float, 3> TImageOut;
    typedef ResampleImageFilter<TImage, TImageOut> ResamplerType;
    ResamplerType::Pointer resampler = ResamplerType::New();
    resampler->SetInput(movingimage);
    resampler->SetSize(fixedimage->GetLargestPossibleRegion().GetSize());
    resampler->SetOutputOrigin(fixedimage->GetOrigin());
    resampler->SetOutputSpacing(fixedimage->GetSpacing());
    resampler->SetOutputDirection(fixedimage->GetDirection());
    resampler->SetDefaultPixelValue(100);
    //  resampler->SetTransform(finalTransform);
    resampler->SetTransform(registration->GetTransform());
    resampler->Update();


    /* Write to output */
    typedef ImageFileWriter<TImageOut> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(resampler->GetOutput());
    writer->SetFileName(outputfile);
    writer->Update();
}


  /* Generate the deforamtion field from the registration */
void AssignPointer::GenerateDeformation(const RegistrationType::Pointer registration, const TImage::Pointer fixedimage,
                                        const TImage::Pointer movingimage, const std::string outputdeformfile
                                        ) const
{
    std::cout<<"Writing deformation field"<<std::endl;
    typedef Vector<float, 3>  VectorType;
    typedef Image<VectorType, 3> FieldType;

    FieldType::Pointer field = FieldType::New();
    TImage::RegionType fixedRegion = fixedimage->GetBufferedRegion();
    field->SetRegions(fixedRegion);
    field->SetOrigin(fixedimage->GetOrigin());
    field->SetSpacing(fixedimage->GetSpacing());
    field->SetDirection(fixedimage->GetDirection());
    field->Allocate();

    typedef ImageRegionIterator<FieldType> FieldIterator;
    FieldIterator fi(field, fixedRegion);

    fi.GoToBegin();

    BSplineTransformType::InputPointType fixedPoint;
    BSplineTransformType::OutputPointType movingPoint;
    FieldType::IndexType index;

    VectorType displacement;

    while(!fi.IsAtEnd()) {
        index = fi.GetIndex();
        field->TransformIndexToPhysicalPoint(index, fixedPoint);
        movingPoint = registration->GetTransform()->TransformPoint(fixedPoint);
        displacement = movingPoint - fixedPoint;
        fi.Set(displacement);
        ++fi;
    }

    typedef ImageFileWriter<FieldType> FieldWriterType;
    FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetInput(field);
    //  fieldWriter->SetFileName("deform_field.mha");
    fieldWriter->SetFileName(outputdeformfile);
    fieldWriter->Update();
}
