#ifndef RESCALEIMAGE_TXX
#define RESCALEIMAGE_TXX


#include "rescaleImage.h"

namespace itk{

/* Update image type */
template <class InputType, class OutputType, unsigned int Dimension>
void RescaleImage::Update(){

    typedef Image<InputType, Dimension> TImageI;
    typedef Image<OutputType, Dimension> TImageO;
    Run<TImageI, TImageO>();
}


/* Run the filter */
template <class TImageI, class TImageO>
int RescaleImage::Run()

{

  /* Read the input */
  typedef ImageFileReader<TImageI> ReaderType;
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
  typedef RescaleIntensityImageFilter<TImageI, TImageO> RescalerType;
  typename RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());
  rescaler->SetOutputMinimum(low);
  rescaler->SetOutputMaximum(up);

  try
      {
      rescaler->Update();
      }
    catch( ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
      }


  /* Write to output */
  typedef ImageFileWriter<TImageO> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(rescaler->GetOutput());

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


#endif // RESCALEIMAGE_TXX
