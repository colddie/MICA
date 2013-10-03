#include "demonReg.h"
#include "iterationUpdate.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkWarpImageFilter.h"



const unsigned int Dimension = 3;

int itk::DemonReg::Run(Par &par)
{
    typedef itk::Image< float, Dimension > ImageType;

    // Readin files
    typedef itk::ImageFileReader< ImageType > ReaderType;
    ReaderType::Pointer targetReader = ReaderType::New();
    targetReader->SetFileName(par.inputfile1);
    targetReader->Update();

    ReaderType::Pointer movingReader = ReaderType::New();
    movingReader->SetFileName(par.inputfile2);
    movingReader->Update();

    // Match histogram
    typedef  itk::HistogramMatchingImageFilter< ImageType, ImageType > MatchFilterType;
    MatchFilterType::Pointer matcher = MatchFilterType::New();

    matcher->SetInput(movingReader->GetOutput());
    matcher->SetReferenceImage(targetReader->GetOutput());
    matcher->SetNumberOfHistogramLevels(par.histogramlevel);
    matcher->SetNumberOfMatchPoints(par.matchnumber);
    matcher->ThresholdAtMeanIntensityOn();

    // Set deformation filter
    typedef itk::Vector< float, Dimension > VectorType;
    typedef itk::Image< VectorType, Dimension> DisplacementFieldType;

    typedef itk::DemonsRegistrationFilter<ImageType, ImageType, DisplacementFieldType>
            RegistrationFilterType;
    RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
    filter->SetStandardDeviations(par.demonstd);

    CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
    filter->AddObserver(itk::IterationEvent(), observer);

    //Use multiresolution scheme
    typedef itk::MultiResolutionPDEDeformableRegistration<ImageType, ImageType, DisplacementFieldType>
            MultiResolutionImageRegistrationFilterTyep;
    MultiResolutionImageRegistrationFilterTyep::Pointer multires = MultiResolutionImageRegistrationFilterTyep::New();
    multires->SetRegistrationFilter(filter);
    multires->SetNumberOfLevels(par.levelnumber);
    multires->SetFixedImage(targetReader->GetOutput());
    multires->SetMovingImage(matcher->GetOutput());

    unsigned int nIterations[4];
    for (unsigned int i=0; i<3; i++) {
        nIterations[i] = par.maxiteration[i];
    }
    multires->SetNumberOfIterations(nIterations);

    CommandResolutionLevelUpdate::Pointer levelObserver = CommandResolutionLevelUpdate::New();
    multires->AddObserver(itk::IterationEvent(), levelObserver);

    try
    {
        multires->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout<<excp<<std::endl;
        return EXIT_FAILURE;
    }


    // Warp to output image
    typedef itk::WarpImageFilter< ImageType, ImageType, DisplacementFieldType > WarperType;
    typedef itk::LinearInterpolateImageFunction< ImageType, double > InterpolatorType;

    WarperType::Pointer warper = WarperType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    ImageType::Pointer targetImage = targetReader->GetOutput();

    warper->SetInput(movingReader->GetOutput());
    warper->SetInterpolator(interpolator);
    warper->SetOutputSpacing(targetImage->GetSpacing());
    warper->SetOutputOrigin(targetImage->GetOrigin());
    warper->SetOutputDirection(targetImage->GetDirection());
    warper->SetDisplacementField(multires->GetOutput());

    typedef itk::ImageFileWriter< ImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(par.outputfile);
    writer->SetInput(warper->GetOutput());

    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout<<excp<<std::endl;
        return EXIT_FAILURE;
    }


    // Write the deformation filed
    typedef itk::ImageFileWriter< DisplacementFieldType > FieldWriterType;
    FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
    fieldWriter->SetFileName(par.outputdeformfile);
    fieldWriter->SetInput(multires->GetOutput());

    try
    {
        fieldWriter->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
        std::cout<<excp<<std::endl;
        return EXIT_FAILURE;
    }


}
