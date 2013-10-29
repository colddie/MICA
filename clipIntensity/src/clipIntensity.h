#ifndef _clipIntensity_H_
#define _clipIntensity_H_

#include "IO_type.h"
#include "argumentsParse.h"

//
enum MODE
{
  NONE,
  UP,
  LOW,
  BOTH
};


namespace itk {


class ClipIntensity : public itk::Object
{

public:

    std::string         m_inputfile;
    std::string         m_outputfile;
    double              m_upper;
    double              m_lower;
    MODE                m_mode;

    /* Standard calss typedefs */
    typedef ClipIntensity Self;
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
    itkTypeMacro(ClipIntensity, Object);


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

    ClipIntensity(){};
    ~ClipIntensity(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "clipIntensity.txx"
#endif

#endif
