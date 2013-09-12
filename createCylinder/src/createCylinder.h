#ifndef _createCylinder_H_
#define _createCylinder_H_

#include "IO_type.h"
#include "argumentsParse.h"
#include "toolBase.h"


namespace itk {


class createCylinder : public itk::Object
{

public:

    std::string m_outputfile;
    std::vector <unsigned int> m_dimensionsize;
    std::vector <double>       m_center;
    double                     m_radius;
    double                     m_length;
    std::vector <double>       m_imagespacing;
    std::vector <double>       m_orientation;

    /* Standard calss typedefs */
    typedef createCylinder Self;
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
    itkTypeMacro(createCylinder, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[]){
    //            inputfile = std::string(a[0]);
    //            outputfile = std::string(a[1]);
    //        }

    /* SetInput file */


    /* Update the filter with image type*/
    template <class OutputType, unsigned int Dimension>
    void Update();
    /* Run the filter */
    template <class OutputType, unsigned int Dimension>
    int Run(void);





protected:

    createCylinder(){
    };
    ~createCylinder(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "createCylinder.txx"
#endif

#endif
