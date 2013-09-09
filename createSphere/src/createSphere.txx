#ifndef CREATESPHERE_TXX
#define CREATESPHERE_TXX

#include "itkSphereSpatialFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

#include "createSphere.h"


namespace itk{

/* Update image type */
template <class OutputType, unsigned int Dimension>
void createSphere::Update(){

    Run<OutputType, Dimension>();
}


/* Run the filter */
template <class OutputType, unsigned int Dimension>
int createSphere::Run()
{

    typedef Image<OutputType, Dimension> ImageType;

    /* Read the input */


    /* Run the filter */
    typedef itk::SphereSpatialFunction<Dimension> SphereSpatialFunctionType;
    typedef typename SphereSpatialFunctionType::InputType InputType;

    typedef typename ImageType::RegionType    RegionType;
    typedef typename ImageType::SizeType      SizeType;
    typedef typename ImageType::SizeValueType SizeValueType;
    typedef typename ImageType::SpacingType   SpacingType;

    //  parse arguments
    SizeType    size;
    SpacingType spacing;
    InputType   center;

    for (unsigned int i=0; i<Dimension; i++) {
        size[i] = static_cast<SizeValueType>(m_dimensionsize[i]);
        spacing[i] = m_imagespacing[i];
        center[i] = m_center[i];
    }

    //  create image
    RegionType region;
    region.SetSize(size);
    typename ImageType::Pointer image = ImageType::New();
    image->SetRegions(region);
    image->SetSpacing(spacing);
    image->Allocate();

    //  create and initialize sphere
    typename SphereSpatialFunctionType::Pointer sphere = SphereSpatialFunctionType::New();
    sphere->SetCenter(center);
    sphere->SetRadius(m_radius);

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
        it.Set(sphere->Evaluate(point));
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

#endif // CREATESPHERE_TXX
