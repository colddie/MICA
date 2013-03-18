#ifndef _orientImage_H_
#define _orientImage_H_

#include "itkOrientImageFilter.h"

#include "argumentsParse.h"
#include "IO_type.h"




namespace itk {

class OrientImage : public itk::Object
{

public:

    std::string m_inputfile;
    std::string m_outputfile;
    bool m_isoldgiven;
    itk::SpatialOrientation::ValidCoordinateOrientationFlags m_indirection;
    itk::SpatialOrientation::ValidCoordinateOrientationFlags m_outdirection;



    /* Standard calss typedefs */
    typedef OrientImage Self;
    typedef itk::Object Superclass;
    typedef SmartPointer< Self > Pointer;

    //    static Self * New( unsigned int dim,
    //      itk::ImageIOBase::IOComponentType componentType )
    //    {
    //      if( VDimension == dim
    //        && IsType<TComponentType>( componentType ) )
    //      {
    //        return new Self;
    //      }
    //      return 0;
    //    }

    //    template <class T>
    //    bool IsType( itk::ImageIOBase::IOComponentType ct )
    //    {
    //      return ct == itk::ImageIOBase::MapPixelType<T>::CType;
    //    }

    /* Method for creation through the object factoAddry */
    itkNewMacro(Self);

    /* Runtime type information */
    itkTypeMacro(OrientImage, Object);



    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class InputType, unsigned int Dimension>
    int Run(void);





protected:

    OrientImage(){};
    ~OrientImage(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "orientImage.txx"
#endif

#endif
