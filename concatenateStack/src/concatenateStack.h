#ifndef _ConcatenateStack_H_
#define _ConcatenateStack_H_

#include <algorithm>
#include <vector>

#include "itkTileImageFilter.h"
#include "IO_type.h"
#include "argumentsParse.h"







namespace itk {

class ConcatenateStack : public Object
{

public:

    std::vector<std::string> inputfile;
    std::string outputfile;
    //    unsigned int inputnumber;


    /* Standard calss typedefs */
    typedef ConcatenateStack Self;
    typedef Object Superclass;
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
    itkTypeMacro(ConcatenateStack, Object);


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

    ConcatenateStack(){};
    ~ConcatenateStack(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "concatenateStack.txx"
#endif

#endif
