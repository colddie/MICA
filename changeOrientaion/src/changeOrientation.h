#ifndef _changeOrientation_H_
#define _changeOrientation_H_

#include "itkChangeInformationImageFilter.h"
#include "itkPermuteAxesImageFilter.h"
#include "itkVersor.h"

#include "argumentsParse.h"
#include "IO_type.h"




namespace itk {

class ChangeOrientation : public itk::Object
{

public:

    std::string m_inputfile;
    std::string m_outputfile;
    std::vector<double> m_shift;
    std::vector<double> m_spacing;
    std::vector<double> m_origin;
    std::vector<unsigned int> m_dimsize;


    /* Standard calss typedefs */
    typedef ChangeOrientation Self;
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
    itkTypeMacro(ChangeOrientation, Object);



    /* SetInput file */


    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();

    /* Run the filter */
    template <class InputType, unsigned int Dimension>
    int Run(void);





protected:

    ChangeOrientation(){};
    ~ChangeOrientation(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "changeOrientation.txx"
#endif

#endif
