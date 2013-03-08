#ifndef ADDSCALAR_TXX
#define ADDSCALAR_TXX

#include "addScalar.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void AddScalar::Update(){

    typedef VectorImage<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int AddScalar::Run()

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
  typedef AddImageFilter<TImage, TImage, TImage> AddImageFilterType;
  typename AddImageFilterType::Pointer addImageFilter = AddImageFilterType::New();
  addImageFilter->SetInput1(reader->GetOutput());

  // vecotrize index
  typedef itk::VariableLengthVector<double> VariableVectorType;
  VariableVectorType variableLengthVector;

  variableLengthVector.SetSize(channels);
  for(int i=0;i<channels;i++) {
      variableLengthVector[i] = index;}

  addImageFilter->SetConstant2(variableLengthVector);

  addImageFilter->Update();


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(addImageFilter->GetOutput());

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

#endif // ADDSCALAR_TXX
