#ifndef CONVERTIMAGE_TXX
#define CONVERTIMAGE_TXX


#include "convertImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ConvertImage::Update(){

    typedef VectorImage<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int ConvertImage::Run()

{

  /* Read the input */
  typedef ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputfile);

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



  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(reader->GetOutput());

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

#endif // CONVERTIMAGE_TXX
