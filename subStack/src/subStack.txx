#ifndef SUBSTACK_TXX
#define SUBSTACK_TXX


#include "subStack.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void SubStack::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int SubStack::Run()
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
  unsigned int startslice, endslice;
  typename TImage::IndexType start;
  typename TImage::SizeType size;
  typename TImage::RegionType region;

  // Set the start point coordinate and roi size
  sscanf(bound, "%d%d", &startslice, &endslice);
  for(int i=0; i<2; i++) {start[i] = 0;}
  size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  start[2] = startslice-1;
  size[2] = startslice+endslice-2;

  region.SetSize(size);
  region.SetIndex(start);

  // Run the filter, two filter two conditions
  typedef RegionOfInterestImageFilter<TImage, TImage> ROIFilterType;
  typename ROIFilterType::Pointer roifilter = ROIFilterType::New();
  typedef ExtractImageFilter<TImage, TImage> ROIFilterType1;
  typename ROIFilterType1::Pointer roifilter1 = ROIFilterType1::New();
  if (keep_origin == 0) {
      roifilter->SetRegionOfInterest(region);
      roifilter->SetInput(reader->GetOutput());
  } else if (keep_origin == 1){
      roifilter1->SetExtractionRegion(region);
      roifilter1->SetInput(reader->GetOutput());
  }



  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  if (keep_origin == 0) {
      writer->SetInput(roifilter->GetOutput()); }
  else if (keep_origin == 1) {
      writer->SetInput(roifilter1->GetOutput()); }

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

}



}

#endif // SUBSTACK_TXX
