#include "IO_type.h"

using namespace itk;


/* Return ByteOrder */
std::string IO_type::Return_ByteOrder(ImageIOBase::Pointer ImageIO)
{
    ImageIOBase::ByteOrder byteorder = ImageIO->GetByteOrder();

    std::string str = ImageIO->GetByteOrderAsString(byteorder);
    return str;

}


/* Return the image type string */
std::string IO_type::Return_ImageType(ImageIOBase::Pointer ImageIO)
{
    ImageIOBase::IOComponentType componentType = ImageIO->GetComponentType();

    std::string str = ImageIO->GetComponentTypeAsString(componentType);
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
     //      imagetype = str.c_str();
    return str;

}


/* Return the pixel type string */
std::string IO_type::Return_PixelType(ImageIOBase::Pointer ImageIO)
{
    ImageIOBase::IOPixelType pixelType = ImageIO->GetPixelType();

    std::string str = ImageIO->GetPixelTypeAsString(pixelType);
    return str;

}


/* Return the image dimesion number */
unsigned int IO_type::Return_DimensionNumber(ImageIOBase::Pointer ImageIO)
{

    unsigned int dim = ImageIO->GetNumberOfDimensions();

    /* Vector image or not */
    std::string pixelType = Return_PixelType(ImageIO);
    if (pixelType == "vector") {
        dim++;
    }
    return dim;
}


/* Return the image dimesion number */
unsigned int IO_type::Return_ChannelNumber(ImageIOBase::Pointer ImageIO)
{

    unsigned int channel = ImageIO->GetNumberOfComponents();
    return channel;
}


/* Return the image dimension size */
std::vector<unsigned int> IO_type::Return_DimensionSize(ImageIOBase::Pointer ImageIO)
{
    std::vector<unsigned int> dimsize;
    unsigned int dim = Return_DimensionNumber(ImageIO);
    dimsize.reserve(dim);

    /* Vector image or not */
    std::string pixelType = Return_PixelType(ImageIO);
    if (pixelType == "scalar") {
        for (int i=0; i<dim; i++) {
            dimsize.push_back(ImageIO->GetDimensions(i));
        }
    } else if (pixelType == "vector") {
        for (int i=0; i<dim-1; i++) {
        dimsize.push_back(ImageIO->GetDimensions(i));
        }
        unsigned int channel = ImageIO->GetNumberOfComponents();
        dimsize.push_back(channel);
    }
    return dimsize;
}


/* Return the spacing */
std::vector<double> IO_type::Return_Spacing(ImageIOBase::Pointer ImageIO)
{
    std::vector<double> spacing;
    unsigned int dim = ImageIO->GetNumberOfDimensions();
    spacing.reserve(dim);
    for (int i=0; i<dim; i++) {
           spacing.push_back(ImageIO->GetSpacing(i));
    }

    return spacing;
}


/* Return the origin */
std::vector<double> IO_type::Return_Origin(ImageIOBase::Pointer ImageIO)
{
    std::vector<double> origin;
    unsigned int dim = ImageIO->GetNumberOfDimensions();
    origin.reserve(dim);
    for (int i=0; i<dim; i++) {
           origin.push_back(ImageIO->GetOrigin(i));
    }


    return origin;
}


/* Create ImageIO */
ImageIOBase::Pointer IO_type::Create_IO(const char *a)
{

    /* Use ImageIOFactory to automaticlly identify the IO */
    ImageIOBase::Pointer ImageIO =  ImageIOFactory::CreateImageIO(a, ImageIOFactory::ReadMode);
    if(!ImageIO->CanReadFile(a)) {
      std::cout << a << ": MetaImageIO cannot read "
                << a << std::endl;
      exit(EXIT_FAILURE);
      }
    ImageIO->SetFileName(a);
    ImageIO->ReadImageInformation();
    return ImageIO;

}








//      /* Main func to get image componenttype */
//int IO_type::main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel)
//{


//      /* Use ImageIOFactory to automaticlly identify the IO */
//      itk::ImageIOBase::Pointer ImageIO =  itk::ImageIOFactory::CreateImageIO(a, itk::ImageIOFactory::ReadMode);
//      if(!ImageIO->CanReadFile(a)) {
//        std::cout << a << ": MetaImageIO cannot read "
//                  << a << std::endl;
//        return EXIT_FAILURE;
//        }

//      ImageIO->SetFileName(a);
//      ImageIO->ReadImageInformation();
//      dim = ImageIO->GetNumberOfDimensions();
//	  channel = ImageIO->GetNumberOfComponents();
//      itk::ImageIOBase::IOComponentType componentType = ImageIO->GetComponentType();
//      itk::ImageIOBase::IOPixelType pixelType = ImageIO->GetPixelType();
//      if(pixelType != itk::ImageIOBase::SCALAR && pixelType == itk::ImageIOBase::RGB) {
//          std::cout << a << "No support for Image Pixel TYPE "
//                    << ImageIO->GetPixelTypeAsString(pixelType) << std::endl;
//          return EXIT_FAILURE;
//      }


//      /* 4D image or not */
//          if (pixelType == itk::ImageIOBase::VECTOR) {
//        /* Maybe vector field? */
//        if (componentType == itk::ImageIOBase::FLOAT & dim == 3) {
//            channel = ImageIO->GetNumberOfComponents();
//            dim = 4;
//         }
//        }


//      /* Return the imagetype */
//       std::string str = ImageIO->GetComponentTypeAsString(componentType);
//       std::transform(str.begin(), str.end(),str.begin(), ::toupper);
//       //      imagetype = str.c_str();
//      imagetype = str;
//      return EXIT_SUCCESS;



//}





