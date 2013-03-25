#include "IO_type.h"

using namespace itk;


/* Return ByteOrder */
std::string IO_type::Return_ByteOrder() const
{
    ImageIOBase::ByteOrder byteorder = imageio->GetByteOrder();

    std::string str = imageio->GetByteOrderAsString(byteorder);
    return str;

}


/* Return the image type string */
std::string IO_type::Return_ImageType() const
{
    ImageIOBase::IOComponentType componentType = imageio->GetComponentType();

    std::string str = imageio->GetComponentTypeAsString(componentType);
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    //      imagetype = str.c_str();
    return str;

}


/* Return the pixel type string */
std::string IO_type::Return_PixelType() const
{
    ImageIOBase::IOPixelType pixelType = imageio->GetPixelType();

    std::string str = imageio->GetPixelTypeAsString(pixelType);
    return str;

}


/* Return the image dimesion number */
unsigned int IO_type::Return_DimensionNumber() const
{

    unsigned int dim = imageio->GetNumberOfDimensions();

    /* Vector image or not */
    std::string pixelType = Return_PixelType();
    if (pixelType == "vector") {
        dim++;
    }
    return dim;
}


/* Return the image component number */
unsigned int IO_type::Return_ChannelNumber() const
{

    unsigned int channel = imageio->GetNumberOfComponents();
    return channel;
}


/* Return the image dimension size */
std::vector<unsigned int> IO_type::Return_DimensionSize() const
{
    std::vector<unsigned int> dimsize;
    unsigned int dim = Return_DimensionNumber();
    dimsize.reserve(dim);

    /* Vector image or not */
    std::string pixelType = Return_PixelType();    //
    if (pixelType == "scalar") {
        for (int i=0; i<dim; i++) {
            dimsize.push_back(imageio->GetDimensions(i));
        }
    } else if (pixelType == "vector") {
        for (int i=0; i<dim-1; i++) {
        dimsize.push_back(imageio->GetDimensions(i));
        }
        unsigned int channel = imageio->GetNumberOfComponents();
        dimsize.push_back(channel);
    }
    return dimsize;
}


/* Return the spacing */
std::vector<double> IO_type::Return_Spacing() const
{
    std::vector<double> spacing;
    unsigned int dim = imageio->GetNumberOfDimensions();    //
    spacing.reserve(dim);
    for (int i=0; i<dim; i++) {
           spacing.push_back(imageio->GetSpacing(i));
    }

    return spacing;
}


/* Return the origin */
std::vector<double> IO_type::Return_Origin() const
{
    std::vector<double> origin;
    unsigned int dim = imageio->GetNumberOfDimensions();    //
    origin.reserve(dim);
    for (int i=0; i<dim; i++) {
           origin.push_back(imageio->GetOrigin(i));
    }


    return origin;
}



/* Return the direction */
itk::Matrix<double,3,3> IO_type::Return_Direction() const
{
    unsigned int dim = imageio->GetNumberOfDimensions();     //
    itk::Matrix<double, 3, 3> mat;
    std::vector<double> vec1, vec2, vec3;

    vec1 = imageio->GetDirection(0);
    vec2 = imageio->GetDirection(1);

    for (int j=0; j<dim; j++) {
        mat(0,j) = vec1[j];
    }
    for (int j=0; j<dim; j++) {
        mat(1,j) = vec2[j];
    }

   if (dim==3) {
       vec3 = imageio->GetDirection(2);
       for (int j=0; j<dim; j++) {
           mat(2,j) = vec3[j];
       }
   }

   return mat;
}

/* Return the direcion in one axes */
std::vector<double> IO_type::Return_Direction(unsigned int i) const
{
unsigned int dim = imageio->GetNumberOfDimensions();     //
if (i>=dim) {
    std::cout<<"Inexisted direction fro specified axes"<<std::endl;
    exit(EXIT_FAILURE);
}

return imageio->GetDirection(i);

}


/* Create ImageIO */
void IO_type::Create_IO(const char *a)
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
    imageio = ImageIO;

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





