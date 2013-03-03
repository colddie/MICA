#ifndef _multiplyImage_H_
#define _multiplyImage_H_

#include "itkMultiplyImageFilter.h"
#include "IO_type.h"


namespace itk {


class MultiplyImage : public itk::Object
{

public:


    std::string inputfile1;
    std::string inputfile2;
    std::string outputfile;

    /* Standard calss typedefs */
    typedef MultiplyImage Self;
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
    itkTypeMacro(MultiplyImage, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[]){
    //            inputfile1 = std::string(a[0]);
    //            inputfile2 = std::string(a[1]);
    //            outputfile = std::string(a[2]);
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();
    /* Run the filter */
    template <class TImage>
    int Run(void);





protected:

    MultiplyImage(){};
    ~MultiplyImage(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "multiplyImage.txx"
#endif

#endif
