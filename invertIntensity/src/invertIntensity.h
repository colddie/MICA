#ifndef _invertIntensity_H_
#define _invertIntensity_H_

#include "IO_type.h"
#include "argumentsParse.h"


namespace itk {


class InvertIntensity : public itk::Object
{

public:

    std::string         m_inputfile;
    std::string         m_outputfile;

    /* Standard calss typedefs */
    typedef InvertIntensity Self;
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
    itkTypeMacro(InvertIntensity, Object);


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
    template <class InputType, unsigned int Dimension>
    int Run(void);





protected:

    InvertIntensity(){};
    ~InvertIntensity(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "invertIntensity.txx"
#endif

#endif
