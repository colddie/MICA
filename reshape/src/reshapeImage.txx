#ifndef RESHAPEIMAGE_TXX
#define RESHAPEIMAGE_TXX


#include "reshapeImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ReshapeImage::Update(){

    typedef Image<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int ReshapeImage::Run()

{

  /* Read the input */
  typedef ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(m_inputfile);

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
  typedef ReshapeImageToImageFilter<TImage> FilterType;
  typename FilterType::Pointer reshaper = FilterType::New();
  typename TImage::SizeType size;

  // transfer to default size type
  for (int i=0; i<m_outputsize.size(); i++) {
      size[i] = m_outputsize[i];cout<<size[i];
  }

  reshaper->SetInput(reader->GetOutput());
  reshaper->SetOutputSize(size);
  reshaper->Update();


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(m_outputfile);
  writer->SetInput(reshaper->GetOutput());

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

#endif // RESHAPEIMAGE_TXX
