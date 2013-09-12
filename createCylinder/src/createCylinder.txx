#ifndef CREATECYLINDER_TXX
#define CREATECYLINDER_TXX

#include "itkFiniteCylinderSpatialFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

#include "createCylinder.h"


namespace itk{

/* Update image type */
template <class OutputType, unsigned int Dimension>
void createCylinder::Update(){

    Run<OutputType, Dimension>();
}


/* Run the filter */
template <class OutputType, unsigned int Dimension>
int createCylinder::Run()
{

    typedef Image<OutputType, Dimension> ImageType;

    /* Read the input */


    /* Run the filter */
    typedef itk::FiniteCylinderSpatialFunction<Dimension> SpatialFunctionType;
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
    InputType       orientation;
//    OrientationType orientation;

    for (unsigned int i=0; i<Dimension; i++) {
        size[i]        = static_cast<SizeValueType>(m_dimensionsize[i]);
        spacing[i]     = m_imagespacing[i];
        center[i]      = m_center[i];
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
    typename SpatialFunctionType::Pointer cylinder = SpatialFunctionType::New();
    cylinder->SetCenter(center);
    cylinder->SetRadius(m_radius);
    cylinder->SetAxisLength(m_length);
    cylinder->SetOrientation(orientation);

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
        it.Set(cylinder->Evaluate(point));
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

#endif // createCylinder_TXX
