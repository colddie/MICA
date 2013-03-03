#ifndef CROPIMAGE_TXX
#define CROPIMAGE_TXX


#include "cropImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void CropImage::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int CropImage::Run()
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
  unsigned int xh,xl,yh,yl,zh,zl;
  typename TImage::IndexType start;
  typename TImage::SizeType size;
  typename TImage::RegionType region;

  // Set the start point coordinate and roi size
  if (Dimension == 2) { sscanf(bound, "%d%d%d%d", &xl, &xh, &yl, &yh);
      size[0] = xh-xl; size[1] = yh-yl;
      start[0] = xl; start[1] = yl;
  }
  else if (Dimension == 3) { sscanf(bound, "%d%d%d%d%d%d", &xl, &xh, &yl, &yh, &zl, &zh);
      size[0] = xh-xl; size[1] = yh-yl; size[2] = zh-zl;
      start[0] = xl; start[1] = yl; start[2] = zl;
  }

  region.SetSize(size);
  region.SetIndex(start);

  // Run the filter, two filter two conditions
  typedef RegionOfInterestImageFilter<TImage, TImage> ROIFilterType;
  typename ROIFilterType::Pointer roifilter = ROIFilterType::New();
  typedef ExtractImageFilter<TImage, TImage> ROIFilterType1;
  typename ROIFilterType1::Pointer roifilter1 = ROIFilterType1::New();
 if (keep_origin == FALSE) {
      roifilter->SetRegionOfInterest(region);
      roifilter->SetInput(reader->GetOutput());
  } else if (keep_origin == TRUE){
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

#endif // CROPIMAGE_TXX
