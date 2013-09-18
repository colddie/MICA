#ifndef BLANKIMAGE_TXX
#define BLANKIMAGE_TXX


#include "blankImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void BlankImage::Update(){


    Run <InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int BlankImage::Run()

{
    typedef Image<InputType, Dimension> ImageType;
    typedef typename ImageType::PixelType   PixelType;
    typedef typename ImageType::SizeType    SizeType;
    typedef typename ImageType::SpacingType SpacingType;
    typedef typename ImageType::PointType   OriginType;


  /* Create image */
    SizeType    size;
    SpacingType spacing;
    OriginType  origin;

    for (unsigned int i = 0; i < Dimension; i++) {
        size[i]    = m_imagesize[i];
        spacing[i] = m_imagespacing[i];
        origin[i]  = m_imageorigin[i];
    }

    typename ImageType::Pointer image = ImageType::New();
    image->SetRegions(size);
    image->SetOrigin(origin);
    image->SetSpacing(spacing);
    image->Allocate();
    image->FillBuffer(itk::NumericTraits<PixelType>::Zero);


    /* Write to output */
    typedef ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(m_outputfile.c_str());
    writer->SetInput(image);

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

#endif // BLANKIMAGE_TXX
