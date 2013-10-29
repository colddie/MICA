#ifndef INVERTINTENSITY_TXX
#define INVERTINTENSITY_TXX

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkStatisticsImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

#include "invertIntensity.h"


namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void InvertIntensity::Update(){


    Run <InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int InvertIntensity::Run()

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
    typedef StatisticsImageFilter<ImageType> StatisticsImageFilterType;
    typename StatisticsImageFilterType::Pointer statistics = StatisticsImageFilterType::New();
    statistics->SetInput(reader->GetOutput());
    statistics->Update();

    InputType max = std::numeric_limits<InputType>::min();
    if (statistics->GetMaximum() > max) {
        max = statistics->GetMaximum();
    }

    typedef InvertIntensityImageFilter<ImageType> InvertIntensityFilterType;
    typename InvertIntensityFilterType::Pointer inverter = InvertIntensityFilterType::New();
    inverter->SetInput(reader->GetOutput());
    inverter->SetMaximum(max);
    inverter->Update();


    /* Write to output */
    typedef ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(m_outputfile.c_str());
    writer->SetInput(inverter->GetOutput());

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

#endif // INVERTINTENSITY_TXX
