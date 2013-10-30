#ifndef COMPUTEBOUNDINGBOX_TXX
#define COMPUTEBOUNDINGBOX_TXX

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "vnl/vnl_math.h"

#include "computeBoundingbox.h"


namespace itk{

/* Update image type */
template <class InputType, unsigned int Dimension>
void ComputeBoundingbox::Update()
{

    Run <InputType, Dimension>();
}


/* Run the filter */
template <class InputType, unsigned int Dimension>
int ComputeBoundingbox::Run()

{
    typedef Image<InputType, Dimension> ImageType;


    /* Read image */
    typedef ImageFileReader<ImageType> ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(m_inputfile.c_str());

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
    typedef itk::ImageRegionConstIteratorWithIndex<ImageType>  IteratorType;
    typedef typename ImageType::PixelType PixelType;
    typedef typename ImageType::PointType PointType;
    typedef typename ImageType::IndexType IndexType;

    IteratorType iterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

    IndexType minIndex;
    IndexType maxIndex;
    iterator.GoToReverseBegin();
    minIndex = iterator.GetIndex();
    iterator.GoToBegin();
    maxIndex = iterator.GetIndex();
    //    PixelType zero = itk::NumericTraits< PixelType >::Zero;

    while (!iterator.IsAtEnd())
    {
        if (iterator.Get() > m_value)
        {
            const IndexType & index = iterator.GetIndex();
            for (unsigned int i = 0; i < Dimension; ++i)
            {
                minIndex[i] = vnl_math_min( index[i], minIndex[i]);
                maxIndex[i] = vnl_math_max( index[i], maxIndex[i]);
            }
        }
        ++iterator;
    }

    PointType minPoint;
    PointType maxPoint;
    reader->GetOutput()->TransformIndexToPhysicalPoint(minIndex, minPoint);
    reader->GetOutput()->TransformIndexToPhysicalPoint(maxIndex, maxPoint);


    /* Print output */
    std::cout << "MinimumIndex = " << minIndex << "\n"
      << "MaximumIndex = " << maxIndex << std::endl;
    std::cout << std::showpoint;
    std::cout << "MinimumPoint = " << minPoint << "\n"
      << "MaximumPoint = " << maxPoint << std::endl;

}


}


#endif // COMPUTEBOUNDINGBOX_TXX
