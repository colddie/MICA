#ifndef _addImage_H_
#define _addImage_H_

#include "itkNaryAddImageFilter.h"

#include "IO_type.h"
#include "argumentsParse.h"




namespace itk {


class AddImage : public itk::Object
{

public:


    std::vector<string> inputfile;
    std::string outputfile;
    bool isweight;
    std::vector<double> weight;
    bool isaverage;

    /* Standard calss typedefs */
    typedef AddImage Self;
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
    itkTypeMacro(AddImage, Object);


    /* Parse aarguments */
    //    void SetArgs(const char *a[]){
    //            inputfile1 = std::string(a[0]);
    //            inputfile2 = std::string(a[1]);
    //            outputfile = std::string(a[2]);
    //        }

    /* SetInput file */

//    double divide_val(double val);

    /* Update the filter with image type*/
    template <class InputType, unsigned int Dimension>
    void Update();
    /* Run the filter */
    template <class TImage>
    int Run(void);




protected:

    AddImage(){};
    ~AddImage(){};



private:

    AddImage(const Self &);
    void operator =(const Self &);

};


}





#ifndef ITK_MANUAL_INSTANTIATION
#include "addImage.txx"
#endif

#endif
