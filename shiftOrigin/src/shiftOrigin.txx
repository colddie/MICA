#ifndef SHIFTORIGIN_TXX
#define SHIFTORIGIN_TXX


#include "shiftOrigin.h"

namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ShiftOrigin::Update(){

    Run<InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ShiftOrigin::Run()

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
  typedef ChangeInformationImageFilter<TImage> FilterType;
  typename FilterType::Pointer changeFilter = FilterType::New();

  changeFilter->SetInput(reader->GetOutput());
  changeFilter->ChangeOriginOn();

  typename TImage::PointType origin;
  for (int i=0; i<m_origin.size(); i++)
  {
      origin[i] = m_origin[i]+m_shift[i];
  }

  changeFilter->SetOutputOrigin(origin);

//  typedef  ResampleImageFilter<TImage, TImage> FilterType;
//  typename FilterType::Pointer resizer = FilterType::New();

//  //  typedef  AffineTransform<double, Dimension> TransformType;
//  //  typename TransformType::Pointer transform = TransformType::New();
//  //  typename TransformType::OutputVectorType scalevec;
//  //  for (int i=0; i<m_scale.size(); i++) { scalevec[i] = m_scale[i]; }
//  //  transform->Scale(scalevec);
//  //  resizer->SetTransform(transform);

//  typedef  NearestNeighborInterpolateImageFunction<TImage, double> InterpolatorType;
//  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
//  resizer->SetInterpolator(interpolator);

//  /* Set the outer pixel intensity */
//  resizer->SetDefaultPixelValue(0);

//  typename TImage::SpacingType spacing;
//  typename TImage::PointType origin;
//  typename TImage::SizeType size;

//  origin = reader->GetOutput()->GetOrigin();

//  for (int i=0; i<m_scale.size(); i++)
//  {   spacing[i] = m_spacing[i]*m_scale[i];
//      origin[i] = m_origin[i];
//      size[i] = m_dimsize[i]*m_scale[i];
//  }

//  resizer->SetOutputSpacing(spacing);

//  resizer->SetOutputOrigin(origin);

//  resizer->SetSize(size);
//  //  FixedArray<unsigned int> sizevec;
//  //  transform(m_scale.begin(), m_scale.end(), m_dimsize.begin(), sizevec, multiplies<double>());
//  //  resizer->SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());

//  resizer->SetInput(reader->GetOutput());



//  try
//      {
//      resizer->Update();
//      }
//    catch( ExceptionObject & err )
//      {
//      std::cerr << "ExceptionObject caught !" << std::endl;
//      std::cerr << err << std::endl;
//      return EXIT_FAILURE;
//      }


  /* Write to output */
  typedef ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(m_outputfile);
  writer->SetInput(changeFilter->GetOutput());

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


#endif // SHIFTORIGIN_TXX
