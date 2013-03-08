#ifndef _multiplyScalar_H_
#define _multiplyScalar_H_

#include "itkMultiplyImageFilter.h"
#include "IO_type.h"
#include "argumentsParse.h"


namespace itk {


class MultiplyScalar : public itk::Object
{

public:

    std::string inputfile;
    std::string outputfile;
    double index;
    unsigned int channel;

    /* Standard calss typedefs */
    typedef MultiplyScalar Self;
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

    /* Method for creation through the object factory */
    itkNewMacro(Self);

    /* Runtime type information */
    itkTypeMacro(MultiplyScalar, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[], unsigned int dim, unsigned int channel){
    //            inputfile = std::string(a[0]);
    //            outputfile = std::string(a[1]);
    //            index = atof(a[2]);
    //            dimension = dim;
    //            channels = channel;
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();
    /* Run the filter */
    template <class TImage>
    int Run(void);





protected:

    MultiplyScalar(){};
    ~MultiplyScalar(){};



private:

//    MultiplyScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "multiplyScalar.txx"
#endif

#endif
