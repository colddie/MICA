#ifndef CLIPINTENSITY_TXX
#define CLIPINTENSITY_TXX

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkThresholdImageFilter.h"

#include "clipIntensity.h"


namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ClipIntensity::Update(){


    Run <InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ClipIntensity::Run()

{
    typedef Image<InputType, Dimension> ImageType;


    /* Read image */
    typedef ImageFileReader<ImageType> ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(m_inputfile.c_str());

    try
    {
        reader->Update();
    }
    catch( ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }


    /* Run the filter */
    typedef ThresholdImageFilter<ImageType> ThresholdImageFilterType;
    typename ThresholdImageFilterType::Pointer thresholder = ThresholdImageFilterType::New();
    typename ThresholdImageFilterType::Pointer thresholder2 = ThresholdImageFilterType::New();
    thresholder->SetInput(reader->GetOutput());

    // up, low or both mode
    switch (m_mode) {
    case UP:
    {
        thresholder->ThresholdAbove(m_upper);
        thresholder->SetOutsideValue(m_upper);
        thresholder->Update();
        break;
    }
    case LOW:
    {
        thresholder->ThresholdBelow(m_lower);
        thresholder->SetOutsideValue(m_lower);
        thresholder->Update();
        break;
    }
    case BOTH:
    {
        thresholder->ThresholdAbove(m_upper);
        thresholder->SetOutsideValue(m_upper);

        thresholder2->SetInput(thresholder->GetOutput());
        thresholder2->ThresholdBelow(m_lower);
        thresholder2->SetOutsideValue(m_lower);
        thresholder2->Update();
        break;
    }
    default:
        return EXIT_FAILURE;
    }



    /* Write to output */
    typedef ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(m_outputfile.c_str());

    // get from different filter
    if (m_mode != BOTH) {
        writer->SetInput(thresholder->GetOutput());
    }
    else {
        writer->SetInput(thresholder2->GetOutput());
    }


    try
    {
        writer->Update();
    }
    catch( ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

}

#endif // CLIPINTENSITY_TXX
