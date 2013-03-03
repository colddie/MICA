#ifndef CASTIMAGE_TXX
#define CASTIMAGE_TXX


#include "castImage.h"

namespace itk{

/* Update image type */
template <class InputType, class OutputType, unsigned int Dimension>
void CastImage::Update(){

    typedef Image<InputType, Dimension> TImageI;
    typedef Image<OutputType, Dimension> TImageO;
    Run<TImageI, TImageO>();
}


/* Run the filter */
template <class TImageI, class TImageO>
int CastImage::Run()

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
  typedef CastImageFilter<TImageI, TImageO> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  caster->SetInput(reader->GetOutput());

  try
      {
      caster->Update();
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
  writer->SetInput(caster->GetOutput());

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


#endif // CASTIMAGE_TXX
