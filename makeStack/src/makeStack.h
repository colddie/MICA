#ifndef _MakeStack_H_
#define _MakeStack_H_

#include <algorithm>
//#include <vector>
#include <QDir>
#include <QString>

#include "itkTileImageFilter.h"
#include "IO_type.h"






namespace itk {

class MakeStack : public itk::Object
{

public:

    std::vector<std::string> inputfile;
    std::string outputfile;
    //    unsigned int inputnumber;


    /* Standard calss typedefs */
    typedef MakeStack Self;
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
    itkTypeMacro(MakeStack, Object);


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
    void Update(void) const;

    /* Run the filter */
    template <class InputType, unsigned int Dimension>
    int Run(void) const;





protected:

    MakeStack(){};
    ~MakeStack(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "makeStack.txx"
#endif

#endif
