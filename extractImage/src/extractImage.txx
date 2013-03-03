#ifndef EXTRACTImage_TXX
#define EXTRACTImage_TXX


#include "extractImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ExtractImage::Update(){

    if (channel == 1) {
        Run<InputType, Dimension>(); }
    else {
        Run4D<InputType, Dimension>(); }
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ExtractImage::Run()
{

  // output image has 1 less dimension
  typedef Image<InputType, Dimension> InputImage;
  typedef Image<InputType, Dimension-1> OutputImage;

  /* Read the input */
  typedef ImageFileReader<InputImage> ReaderType;
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
  typename InputImage::SizeType imagesize;
  typename InputImage::IndexType index;
  typename InputImage::SizeType size;
  typename InputImage::RegionType region;

  // Set the plane and slice number to extract
  imagesize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  size = imagesize; size[plane] = 0;
  for (int i=0; i<3; i++) { index[i] = 0; }
  index[plane] = slice;


  region.SetSize(size);
  region.SetIndex(index);

  // Run the filter
  typedef ExtractImageFilter<InputImage, OutputImage> ROIFilterType;
  typename ROIFilterType::Pointer roifilter = ROIFilterType::New();

  roifilter->SetExtractionRegion(region);
  roifilter->SetInput(reader->GetOutput());
  roifilter->SetDirectionCollapseToIdentity();
  roifilter->Update();


  /* Write to output */
  typedef ImageFileWriter<OutputImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputfile);
  writer->SetInput(roifilter->GetOutput());

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


template <class InputType, unsigned int Dimension>
int ExtractImage::Run4D()
{
    // output image has 1 less dimension
    typedef VectorImage<InputType, 3> VectorImageType;
    typedef Image<InputType, 3> ScalarImageType;

    /* Read the input */
    typedef ImageFileReader<VectorImageType> ReaderType;
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
    typedef VectorIndexSelectionCastImageFilter<VectorImageType, ScalarImageType> IndexSelectionType;
    typename IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetIndex(slice);
    indexSelectionFilter->SetInput(reader->GetOutput());




    /* Write to output */
    typedef ImageFileWriter<ScalarImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputfile);
    writer->SetInput(indexSelectionFilter->GetOutput());

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

#endif // EXTRACTImage_TXX
