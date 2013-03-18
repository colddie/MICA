#ifndef _permuteAxes_H_
#define _permuteAxes_H_

#include "itkPermuteAxesImageFilter.h"

#include "argumentsParse.h"
#include "IO_type.h"




namespace itk {

class PermuteAxes : public itk::Object
{

public:

    std::string m_inputfile;
    std::string m_outputfile;
    std::vector<unsigned int> m_order;

    /* Standard calss typedefs */
    typedef PermuteAxes Self;
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
    itkTypeMacro(PermuteAxes, Object);





    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class TImage>
    int Run(void);





protected:

    PermuteAxes(){};
    ~PermuteAxes(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "permuteAxes.txx"
#endif

#endif
