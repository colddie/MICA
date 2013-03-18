#ifndef ORIENTIMAGE_TXX
#define ORIENTIMAGE_TXX


#include "orientImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void OrientImage::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int OrientImage::Run()

{

    typedef Image<InputType, Dimension> TImage;

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
  typedef OrientImageFilter<TImage, TImage> FilterType;
  typename FilterType::Pointer orienter = FilterType::New();

  // Inherit old direction or use specified one
  orienter->SetInput(reader->GetOutput());
  if (!m_isoldgiven) {
      orienter->UseImageDirectionOn(); }
  else {
      orienter->SetGivenCoordinateOrientation(m_indirection);}
  orienter->SetDesiredCoordinateOrientation(m_outdirection);
  orienter->Update();




  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(m_outputfile);
  writer->SetInput(orienter->GetOutput());

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


#endif // ORIENTIMAGE_TXX
