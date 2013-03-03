#ifndef _extractImage_H_
#define _extractImage_H_

#include "itkExtractImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "IO_type.h"





namespace itk {

class ExtractImage : public itk::Object
{

public:

    std::string inputfile;
    std::string outputfile;
    unsigned int plane, slice, channel;


    /* Standard calss typedefs */
    typedef ExtractImage Self;
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
    itkTypeMacro(ExtractImage, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[], std::vector<double> bound){
    //            inputfile = std::string(a[0]);
    //            outputfile = std::string(a[1]);

    //            low = bound[0];
    //            up =  bound[1];
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class InputType, unsigned int Dimension>
    int Run(void);

    template <class InputType, unsigned int Dimension>
    int Run4D(void);



protected:

    ExtractImage(){};
    ~ExtractImage(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "extractImage.txx"
#endif

#endif
