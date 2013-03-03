#ifndef DICE_TXX
#define DICE_TXX


#include "dice.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void Dice::Update(){


    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int Dice::Run()

{

  typedef Image<InputType, Dimension> TImage;

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
  /* Loop through images, gathering Dice */
      unsigned int ref_size = 0;
      unsigned int cmp_size = 0;
      unsigned int TP = 0;
      unsigned int FN = 0;
      unsigned int FP = 0;
      unsigned int TN = 0;

      typename TImage::Pointer ref_img = reader1->GetOutput();
      typename TImage::Pointer cmp_img = reader2->GetOutput();
      typedef ImageRegionIteratorWithIndex<TImage> Iterator;
      Iterator it (ref_img, ref_img->GetLargestPossibleRegion());
      while (!it.IsAtEnd())
      {

          typename TImage::IndexType k;
          k = it.GetIndex();
          if (ref_img->GetPixel(k)) {
              ref_size++;
              if (cmp_img->GetPixel(k)) {
                  TP++;
              } else {
                  FN++;
              }
          } else {
              if (cmp_img->GetPixel(k))
                  FP++;
              else
                  TN++;
          }
          if (cmp_img->GetPixel(k)) {
              cmp_size++;
          }
          ++it;
      }


      printf("DICE COEFFICIENT:");
      printf("%1g\n", (float)(2*TP)/(float)(ref_size+cmp_size));
      printf("JACCARD INDEX:");
      printf("%1g\n", (float)(TP)/(float)(FP+FN+TP));
      printf("TRUE POSITIVE:");
      printf("%1d\n", TP);
      printf("FALSE NEGATIVE:");
      printf("%1d\n", FN);
      printf("FALSE POSITIVE:");
      printf("%1d\n", FP);
      printf("TRUE NEGATIVE:");
      printf("%1d\n", TN);
      //      printf("%1d\n", ref_size);
      //      printf("%1d\n", cmp_size);



        /* Write to output */

}

}
#endif // DICE_TXX
