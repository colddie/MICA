#ifndef CENTROID_TXX
#define CENTROID_TXX


#include "centroid.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void Centroid::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int Centroid::Run()
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

  // define coordinate vector, can only be done in the template
  typedef Vector<double, Dimension> VectorType;
  VectorType coordinate;
  double totalmass;


  typedef ImageMomentsCalculator<TImage> MomentCalculatorType;
  typename MomentCalculatorType::Pointer moment = MomentCalculatorType::New();
  moment->SetImage(reader->GetOutput());
  moment->Compute();
  coordinate = moment->GetCenterOfGravity();
  totalmass = moment->GetTotalMass();

  printf ("CENTROID\n");
  printf ("%1g\t %1g\t %1g\n",
           coordinate[0], coordinate[1], coordinate[2]);


  /* Write to output */


}



}

#endif // CENTROID_TXX
