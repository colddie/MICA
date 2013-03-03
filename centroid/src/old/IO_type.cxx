#include "IO_type.h"

//#include <iostream>




/* Main func to get image componenttype */
int IO_type::main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel)
{


    /* Use ImageIOFactory to automaticlly identify the IO */
    itk::ImageIOBase::Pointer ImageIO =  itk::ImageIOFactory::CreateImageIO(a, itk::ImageIOFactory::ReadMode);
    if(!ImageIO->CanReadFile(a)) {
        std::cout << a << ": MetaImageIO cannot read "
                  << a << std::endl;
        return EXIT_FAILURE;
    }

    /* Read image info */
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





