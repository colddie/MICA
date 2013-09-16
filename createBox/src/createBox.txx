#ifndef CREATEBOX_TXX
#define CREATEBOX_TXX

#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

#include "itkBoxSpatialFunction.h"
#include "createBox.h"


namespace itk{

/* Update image type */
template <class OutputType, unsigned int Dimension>
void createBox::Update(){

    Run<OutputType, Dimension>();
}


/* Run the filter */
template <class OutputType, unsigned int Dimension>
int createBox::Run()
{

    typedef Image<OutputType, Dimension> ImageType;

    /* Read the input */


    /* Run the filter */
    typedef itk::BoxSpatialFunction<Dimension> SpatialFunctionType;
    typedef typename SpatialFunctionType::InputType       InputType;
//    typedef typename SpatialFunctionType::OrientationType OrientationType;

    typedef typename ImageType::RegionType    RegionType;
    typedef typename ImageType::SizeType      SizeType;
    typedef typename ImageType::SizeValueType SizeValueType;
    typedef typename ImageType::SpacingType   SpacingType;

    //  parse arguments
    SizeType        size;
    SpacingType     spacing;
    InputType       center;
    InputType       radius;
    InputType       orientation;
//    OrientationType orientation;

    for (unsigned int i=0; i<Dimension; i++) {
        size[i]        = static_cast<SizeValueType>(m_dimensionsize[i]);
        spacing[i]     = m_imagespacing[i];
        center[i]      = m_center[i];
        radius[i]      = m_radius[i];
        orientation[i] = m_orientation[i];
    }
    //    orientation = vnl_matrix<double>(m_orientation, Dimension, Dimension);

    //  create image
    RegionType region;
    region.SetSize(size);
    typename ImageType::Pointer image = ImageType::New();
    image->SetRegions(region);
    image->SetSpacing(spacing);
    image->Allocate();

    //  create and initialize cylinder
    typename SpatialFunctionType::Pointer box = SpatialFunctionType::New();
    box->SetCenter(center);
    box->SetRadius(radius);
    box->SetOrientation(orientation);

    //  create iterator, index and point
    typedef itk::ImageRegionIterator<ImageType> IteratorType;
    typedef typename ImageType::PointType PointType;
    typedef typename ImageType::IndexType IndexType;

    IteratorType it(image, region);
    it.GoToBegin();
    PointType point;
    IndexType index;

    //  walk over image
    while (!it.IsAtEnd()) {
        index = it.GetIndex();
        image->TransformIndexToPhysicalPoint(index, point);
        it.Set(box->Evaluate(point));
        ++it;
    }


    /* Write to output */
    typedef ImageFileWriter<ImageType> ImageWriterType;
    typename ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName(m_outputfile.c_str());
    writer->SetInput(image);
    writer->Update();

}



}

#endif // createBox_TXX
