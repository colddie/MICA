#ifndef COUNTPIXEL_TXX
#define COUNTPIXEL_TXX


#include "countPixel.h"

#include "itkImageRegionConstIterator.h"


namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void CountPixel::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int CountPixel::Run()
{

  typedef Image<InputType, Dimension> ImageType;

  /* Read the input */
  typedef ImageFileReader<ImageType> ReaderType;
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
  typedef typename ImageType::SizeType SizeType;
  typedef itk::ImageRegionConstIterator< ImageType > IteratorType;

  IteratorType it( reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
  it.GoToBegin();

  unsigned int counter = 0;
  while ( !it.IsAtEnd() ) {
      if( it.Value() == m_value) {
          counter++;
      }
      ++it;
  }


  // get the image size
  SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();


  /* Print output */
  std::cout<<std::endl;
  std::cout<< counter << " " << "pixels value equal to " << m_value << " ";
  std::cout<< "in total " << size[0]<<"*"<<size[1]<<"*"<<size[2];
  std::cout<< " " << "pixels"<<std::endl;

}



}

#endif // COUNTPIXEL_TXX
