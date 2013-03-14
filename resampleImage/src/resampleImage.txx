#ifndef RESAMPLEIMAGE_TXX
#define RESAMPLEIMAGE_TXX


#include "resampleImage.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ResampleImage::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ResampleImage::Run()

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
  typedef  ResampleImageFilter<TImage, TImage> FilterType;
  typename FilterType::Pointer resampler = FilterType::New();

  //  typedef  AffineTransform<double, Dimension> TransformType;
  //  typename TransformType::Pointer transform = TransformType::New();
  //  typename TransformType::OutputVectorType scalevec;
  //  for (int i=0; i<m_scale.size(); i++) { scalevec[i] = m_scale[i]; }
  //  transform->Scale(scalevec);
  //  resampler->SetTransform(transform);

  typedef  NearestNeighborInterpolateImageFunction<TImage, double> InterpolatorType;
  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
  resampler->SetInterpolator(interpolator);

  /* Set the outer pixel intensity */
  resampler->SetDefaultPixelValue(0);

  typename TImage::SpacingType spacing;
  typename TImage::PointType origin;
  typename TImage::SizeType size;

  for (int i=0; i<m_scale.size(); i++)
  {   spacing[i] = m_spacing[i]/m_scale[i];
      origin[i] = m_origin[i];
      size[i] = m_dimsize[i]*m_scale[i];
  }
  cout<<"hello"<<spacing[0]<<spacing[1]<<spacing[2]<<m_scale[0]<<m_scale[1]<<m_scale[2];
  resampler->SetOutputSpacing(spacing);

  resampler->SetOutputOrigin(origin);

  resampler->SetSize(size);
  //  FixedArray<unsigned int> sizevec;
  //  transform(m_scale.begin(), m_scale.end(), m_dimsize.begin(), sizevec, multiplies<double>());
  //  resampler->SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());

  resampler->SetInput(reader->GetOutput());



  try
      {
      resampler->Update();
      }
    catch( ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
      }


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(m_outputfile);
  writer->SetInput(resampler->GetOutput());

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


#endif // RESAMPLEIMAGE_TXX
