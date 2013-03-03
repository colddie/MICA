#ifndef STATISTICS_TXX
#define STATISTICS_TXX


#include "statistics.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void Statistics::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int Statistics::Run()
{

  typedef Image<InputType, Dimension> TImage;

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
  typedef StatisticsImageFilter<TImage> FilterType;
  typename FilterType::Pointer statisticfilter = FilterType::New();
  statisticfilter->SetInput(reader->GetOutput());
  statisticfilter->Update();

  std::cout<<"Mean:"<<statisticfilter->GetMean()<<std::endl;
  std::cout<<"Std:"<<statisticfilter->GetSigma()<<std::endl;
  std::cout<<"Min:"<<statisticfilter->GetMinimum()<<std::endl;
  std::cout<<"Max:"<<statisticfilter->GetMaximum()<<std::endl;
  std::cout<<"Sum:"<<statisticfilter->GetSum()<<std::endl;
  std::cout<<"Var:"<<statisticfilter->GetVariance()<<std::endl;




  /* Write to output */


}



}

#endif // STATISTICS_TXX
