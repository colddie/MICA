#ifndef MULTIPLYIMAGE_TXX
#define MULTIPLYIMAGE_TXX

#include "multiplyImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void MultiplyImage::Update(){

    typedef Image<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int MultiplyImage::Run()

{

  /* Read the input */
  typedef ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  reader1->SetFileName(inputfile1);
  reader2->SetFileName(inputfile2);

  try
      {
      reader1->Update();
      reader2->Update();
      }
    catch( ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
      }


  /* Run the filter */
  typedef MultiplyImageFilter<TImage, TImage, TImage> MultiplyImageFilterType;
  typename MultiplyImageFilterType::Pointer multiplyFilter = MultiplyImageFilterType::New();
  multiplyFilter->SetInput1(reader1->GetOutput());
  multiplyFilter->SetInput2(reader2->GetOutput());
  multiplyFilter->Update();


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(multiplyFilter->GetOutput());

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

#endif // MULTIPLYIMAGE_TXX
