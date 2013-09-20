#ifndef REPLACEINTENSITY_TXX
#define REPLACEINTENSITY_TXX

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkChangeLabelImageFilter.h"

#include "replaceIntensity.h"


namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ReplaceIntensity::Update(){


    Run <InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ReplaceIntensity::Run()

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
    typedef ChangeLabelImageFilter<ImageType, ImageType> ReplacerType;
    typename ReplacerType::Pointer replacer = ReplacerType::New();
    replacer->SetInput(reader->GetOutput());
    replacer->SetChange(m_oldpixel, m_newpixel);



    /* Write to output */
    typedef ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(m_outputfile.c_str());
    writer->SetInput(replacer->GetOutput());

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

#endif // REPLACEINTENSITY_TXX
