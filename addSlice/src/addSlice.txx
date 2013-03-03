#ifndef ADDSLICE_TXX
#define ADDSLICE_TXX


#include "addSlice.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void AddSlice::Update(){

    Run<InputType, Dimension>();

}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int AddSlice::Run()
{

  typedef Image<InputType, Dimension> TImage;
  typedef Image<InputType, Dimension-1> TSlice;


  /* Read the input */
  typedef ImageFileReader<TImage> ImageReaderType;
  typedef ImageFileReader<TSlice> SliceReaderType;
  typename ImageReaderType::Pointer reader = ImageReaderType::New();
  typename SliceReaderType::Pointer reader1 = SliceReaderType::New();
  typename TImage::Pointer inputimage = TImage::New();
  typename TSlice::Pointer inputslice = TSlice::New();

  reader->SetFileName(imagefile); reader->UpdateLargestPossibleRegion();
  inputimage = reader->GetOutput(); inputimage->DisconnectPipeline();

  // get the image size, create blank slice or not
  typename TImage::SizeType size;
  size = inputimage->GetLargestPossibleRegion().GetSize();

  if (add_emptyslice) {
      typename TSlice::RegionType regionadd;
      typename TSlice::IndexType startadd;
      typename TSlice::SizeType sizeadd;
      startadd[0] = 0;
      startadd[1] = 0;
      sizeadd[0] = size[0];
      sizeadd[1] = size[1];

      regionadd.SetSize(sizeadd);
      regionadd.SetIndex(startadd);

      inputslice->SetRegions(regionadd);
      inputslice->Allocate();

      ImageRegionIterator<TSlice> imageIterator(inputslice,regionadd);
      while(!imageIterator.IsAtEnd()) {
          imageIterator.Set(0);
          ++imageIterator;}

  }
  else {
      reader1->SetFileName(slicefile); reader1->UpdateLargestPossibleRegion();
      inputslice = reader1->GetOutput(); inputslice->DisconnectPipeline();

  }



  // cast slice as stack
  typedef CastImageFilter<TSlice, TImage> CasterType;
  typename CasterType::Pointer caster = CasterType::New();

  caster->SetInput(inputslice);
  caster->Update();

  // define the tiler
  typedef TileImageFilter<TImage, TImage> TilerType;
  typename TilerType::Pointer tiler = TilerType::New();

  FixedArray<unsigned int, 3> layout;
  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 0;
  tiler->SetLayout(layout);


  // insert begin
  if (slicenumber == 0) {
      // reset the slice's spacing consistent to the stack
      typename TImage::Pointer resizeslice;
      resizeslice = caster->GetOutput();
      Vector<double, 3> spa;
      spa = inputimage->GetSpacing();
      resizeslice->SetSpacing(spa);

      tiler->SetInput(0, resizeslice);
      tiler->SetInput(1, inputimage);

  }
  // insert end
  else if (slicenumber == size[2]) {
      tiler->SetInput(0, inputimage);
      tiler->SetInput(1, caster->GetOutput());

  }
  // insert other
  else {

      // split into 3 parts
      typedef ExtractImageFilter<TImage, TImage> ExtractFilterType;
      typename ExtractFilterType::Pointer extractfilter1 = ExtractFilterType::New();
      typename ExtractFilterType::Pointer extractfilter2 = ExtractFilterType::New();

      typename TImage::IndexType start1, start2;
      typename TImage::SizeType size1, size2;
      typename TImage::RegionType region1, region2;
      for(int i=0; i<3; i++) {start1[i] = 0;}
      for(int j=0; j<2; j++) {start2[j] = 0;} start2[2] = slicenumber;
      size1 = size; size2 = size;
      size1[2] = slicenumber; size2[2] = size[2] - slicenumber;
      //      std::cout<<size[0]<<size[1]<<size[2]<<std::endl;
      //      std::cout<<size1[0]<<size1[1]<<size1[2]<<size2[0]<<size2[1]<<size2[2];

      region1.SetSize(size1); region1.SetIndex(start1);
      region2.SetSize(size2); region2.SetIndex(start2);

      extractfilter1->SetExtractionRegion(region1);
      extractfilter1->SetInput(reader->GetOutput());
      extractfilter1->Update();
      extractfilter2->SetExtractionRegion(region2);
      extractfilter2->SetInput(reader->GetOutput());
      extractfilter2->Update();

      // tile 3 parts
      tiler->SetInput(0,extractfilter1->GetOutput());
      tiler->SetInput(1,caster->GetOutput());
      tiler->SetInput(2,extractfilter2->GetOutput());

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

#endif // ADDSLICE_TXX
