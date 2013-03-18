#ifndef PERMUTEAXES_TXX
#define PERMUTEAXES_TXX


#include "permuteAxes.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void PermuteAxes::Update(){

    typedef Image<InputType, Dimension> TImage;
    Run<TImage>();
}


/* Run the filter */
template <class TImage>
int PermuteAxes::Run()

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
  typedef PermuteAxesImageFilter<TImage> PermuteFilterType;
  typename PermuteFilterType::Pointer permuter = PermuteFilterType::New();

  FixedArray<unsigned int, 3> order;
  for (int i=0; i<m_order.size(); i++) { order[i] = m_order[i]; }

  permuter->SetInput(reader->GetOutput());
  permuter->SetOrder(order);
  permuter->Update();



  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(m_outputfile);
  writer->SetInput(permuter->GetOutput());

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


#endif // PERMUTEAXES_TXX
