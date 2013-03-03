#ifndef _IO_type_h_
#define _IO_type_h_

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <string.h>

class IO_type
{

public:

//    enum IOtype {
//        type_UCHAR,
//        type_SHORT,
//        type_USHORT,
//        type_INT,
//        type_UINT,
//        type_FLOAT,
//        type_DOUBLE
//    };

//    int type_fun(const char *b[]);
//    unsigned int dim_fun(int a, char *b[]);
//    int main_fun(const char *a, const char* &type, unsigned int &dim, itk::ImageIOBase::IOComponentType &componentType);
    int main_fun(const char *a, std::string &imagetype,  unsigned int &dim, unsigned int &channel);

    IO_type(){};
    ~IO_type(){};

protected:

private:



};


/* Main func to get image componenttype */
int IO_type::main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel)
{


      /* Use ImageIOFactory to automaticlly identify the IO */
      itk::ImageIOBase::Pointer ImageIO =  itk::ImageIOFactory::CreateImageIO(a, itk::ImageIOFactory::ReadMode);

      if(!ImageIO->CanReadFile(a)) {
        std::cout << a << ": ImageIO cannot read "
                  << a << std::endl;
        return EXIT_FAILURE;
        }

      ImageIO->SetFileName(a);
      ImageIO->ReadImageInformation();
      dim = ImageIO->GetNumberOfDimensions();
      channel = ImageIO->GetNumberOfComponents();

      itk::ImageIOBase::IOComponentType componentType = ImageIO->GetComponentType();
      itk::ImageIOBase::IOPixelType pixelType = ImageIO->GetPixelType();     
      if(pixelType != itk::ImageIOBase::SCALAR && pixelType == itk::ImageIOBase::RGB) {
          std::cout << a << "No support for Image Pixel TYPE "
                    << ImageIO->GetPixelTypeAsString(pixelType) << std::endl;
          return EXIT_FAILURE;
      }


      /* 4D image or not */
          if (pixelType == itk::ImageIOBase::VECTOR) {
        /* Maybe vector field? */
        if (componentType == itk::ImageIOBase::FLOAT & dim == 3) {
            channel = ImageIO->GetNumberOfComponents();
            dim = 4;
         }
        }


      /* Return the imagetype */
       std::string str = ImageIO->GetComponentTypeAsString(componentType);
       std::transform(str.begin(), str.end(),str.begin(), ::toupper);
       //      imagetype = str.c_str();
      imagetype = str;
      return EXIT_SUCCESS;



}











#endif
