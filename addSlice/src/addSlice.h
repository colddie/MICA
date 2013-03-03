#ifndef _addSlice_H_
#define _addSlice_H_

#include <algorithm>
#include <vector>

#include "itkCastImageFilter.h"
//#include "itkPasteImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkTileImageFilter.h"
#include "IO_type.h"







namespace itk {

class AddSlice : public itk::Object
{

public:

    std::string imagefile;
    std::string slicefile;
    std::string outputfile;
    unsigned int slicenumber;
    bool add_emptyslice;



    /* Standard calss typedefs */
    typedef AddSlice Self;
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
    itkTypeMacro(AddSlice, Object);


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





protected:

    AddSlice(){};
    ~AddSlice(){};



private:


//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "addSlice.txx"
#endif

#endif
