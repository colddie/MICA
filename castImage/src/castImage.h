#ifndef _castImage_H_
#define _castImage_H_

#include "itkcastImageFilter.h"
#include "IO_type.h"



namespace itk {


class CastImage : public itk::Object
{

public:

    std::string inputfile;
    std::string outputfile;

    /* Standard calss typedefs */
    typedef CastImage Self;
    typedef itk::Object Superclass;
    typedef SmartPointer< Self > Pointer;

//    static Self * New( unsigned int dim,
//                       itk::ImageIOBase::IOComponentType componentType1,
//                       itk::ImageIOBase::IOComponentType componentType2)
//    {
//        if(       componentType1 == itk::ImageIOBase::MapPixelType<InputType>::CType
//             &&   componentType2 == itk::ImageIOBase::MapPixelType<OutputType>::CType )
//        {
//            return new Self;
//        }
//        return 0;
//    }


    /* Method for creation through the object factoAddry */
    itkNewMacro(Self);

    /* Runtime type information */
    itkTypeMacro(CastImage, Object);


    /* Parse arguments */
    //    void SetArgs(const char *a[]){
    //            inputfile = std::string(a[0]);
    //            outputfile = std::string(a[1]);
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, class OutputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class TImage, class TImageO>
    int Run(void);






protected:

    CastImage(){};
    ~CastImage(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "castImage.txx"
#endif

#endif
