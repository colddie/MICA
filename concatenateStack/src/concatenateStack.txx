#ifndef CONCATENATESTACK_TXX
#define CONCATENATESTACK_TXX


#include "concatenateStack.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ConcatenateStack::Update() const{

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ConcatenateStack::Run() const
{

  typedef Image<InputType, Dimension> TImage;

  /* Read the input */
  typedef ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();



  /* Run the filter */
  typedef TileImageFilter<TImage, TImage> TilerType;
  typename TilerType::Pointer tiler = TilerType::New();
  typename TImage::Pointer inputimage = TImage::New();
  unsigned int index = 0;

  // set the layout
  FixedArray<unsigned int, 3> layout;
  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 0;
  tiler->SetLayout(layout);

  // set the input image into filter with index
  for (int i=0; i<inputfile.size(); i++) {
      reader->SetFileName(inputfile[i]);
      reader->UpdateLargestPossibleRegion();
      inputimage = reader->GetOutput();
      inputimage->DisconnectPipeline();
      tiler->SetInput(index++, inputimage);
  }
  tiler->Update();

  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(tiler->GetOutput());
  writer->Update();


}



}

#endif // ConcatenateStack_TXX
