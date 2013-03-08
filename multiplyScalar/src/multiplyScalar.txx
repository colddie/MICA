#ifndef MULTIPLYSCALAR_TXX
#define MULTIPLYSCALAR_TXX

#include "multiplyScalar.h"



///* Update image type */
//template <class InputType, unsigned int Dimension>
//void MultiplyScalar::Update(){

//    typedef VectorImage<InputType, Dimension> TImage;
//    Run<TImage>();
//}


///* Run the filter */
//template <class TImage>
//int MultiplyScalar::Run()

//{

//  /* Read the input */
//  typedef ImageFileReader<TImage> ReaderType;
//  typename ReaderType::Pointer reader = ReaderType::New();
//  reader->SetFileName(inputfile);

//  try
//      {
//      reader->Update();
//      }
//    catch( ExceptionObject & err )
//      {
//      std::cerr << "ExceptionObject caught !" << std::endl;
//      std::cerr << err << std::endl;
//      return EXIT_FAILURE;
//      }


//    /* Run the filter */
//  typedef MultiplyScalarFilter<TImage> FilterType;
//  typename FilterType::Pointer multiplyScalarFilter = FilterType::Name();
//  multiplyScalarFilter->SetIndex(index);
//  multiplyScalarFilter->SetInput(reader->GetOutput());
  //  typedef MultiplyImageFilter<TImage, TImage, TImage> MultiplyImageFilterType;
  //  typename MultiplyImageFilterType::Pointer multiplyImageFilter = MultiplyImageFilterType::New();
  //  multiplyImageFilter->SetInput1(reader->GetOutput());

  //  // vecotrize index
  //  typedef VariableLengthVector<double> VariableVectorType;
  //  VariableVectorType variableLengthVector;

  //  variableLengthVector.SetSize(channel);
  //  for(int i=0;i<channel;i++) {
  //      variableLengthVector[i] = index;}

  //  multiplyImageFilter->SetConstant2(variableLengthVector);
  //  //  multiplyImageFilter->SetConstant2(index);

  //try
  //    {
  //    multiplyImageFilter->Update();
  //    }
  //  catch( ExceptionObject & err )
  //    {
  //    std::cerr << "ExceptionObject caught !" << std::endl;
  //    std::cerr << err << std::endl;
  //    }


  //  /* Write to output */
  //  typedef ImageFileWriter<TImage> WriterType;
  //  typename WriterType::Pointer writer = WriterType::New();
  //  writer->SetFileName(outputfile);
  //  writer->SetInput(multiplyScalarFilter->GetOutput());

  //  try
  //      {
  //      writer->Update();
  //      }
  //    catch( ExceptionObject & err )
  //      {
  //      std::cerr << "ExceptionObject caught !" << std::endl;
  //      std::cerr << err << std::endl;
  //      return EXIT_FAILURE;
  //      }

  //  return EXIT_SUCCESS;
  //}

  //}

#endif // MULTIPLYSCALAR_TXX
