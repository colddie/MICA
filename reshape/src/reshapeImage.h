#ifndef _reshapeImage_H_
#define _reshapeImage_H_

#include "IO_type.h"
#include "argumentsParse.h"
#include "itkReshapeImageToImageFilter.h"


namespace itk {


class ReshapeImage : public itk::Object
{

public:

    std::string m_inputfile;
    std::string m_outputfile;
    std::vector<unsigned int> m_outputsize;

    /* Standard calss typedefs */
    typedef ReshapeImage Self;
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
    itkTypeMacro(ReshapeImage, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[]){
    //            inputfile = std::string(a[0]);
    //            outputfile = std::string(a[1]);
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();
    /* Run the filter */
    template <class TImage>
    int Run(void);





protected:

    ReshapeImage(){};
    ~ReshapeImage(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "reshapeImage.txx"
#endif

#endif
