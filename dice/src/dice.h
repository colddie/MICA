#ifndef _dice_H_
#define _dice_H_

#include "IO_type.h"
#include "itkImageRegionIteratorWithIndex.h"


namespace itk {


class Dice : public itk::Object
{

public:

    std::string inputfile1;
    std::string inputfile2;

    /* Standard calss typedefs */
    typedef Dice Self;
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
    itkTypeMacro(Dice, Object);


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

    Dice(){};
    ~Dice(){};



private:

//    AddScalar(const Self &);
//    void operator =(const Self &);

};


}




#ifndef ITK_MANUAL_INSTANTIATION
#include "dice.txx"
#endif

#endif
