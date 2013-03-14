#ifndef _resizeImage_H_
#define _resizeImage_H_

#include "itkChangeInformationImageFilter.h"

#include "argumentsParse.h"
#include "IO_type.h"




namespace itk {

class ResizeImage : public itk::Object
{

public:

    std::string m_inputfile;
    std::string m_outputfile;
    std::vector<double> m_scale;
    std::vector<double> m_spacing;
    std::vector<double> m_origin;
    std::vector<unsigned int> m_dimsize;


    /* Standard calss typedefs */
    typedef ResizeImage Self;
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
    itkTypeMacro(ResizeImage, Object);



    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class InputType, unsigned int Dimension>
    int Run(void);





protected:

    ResizeImage(){};
    ~ResizeImage(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "resizeImage.txx"
#endif

#endif
