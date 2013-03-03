#include "IO_type.h"

//#include <iostream>


//int IO_type::type_fun(const char *b[])
//{


//    unsigned int dim;
//    enum IOtype type;
//    main_fun (b, type, dim);
//    //addScalar add;

//	std::cout<<dim<<std::endl;
//        if(dim == 2) {
//            switch(type) {
//            case (type_UCHAR): {
//                 T<itk::Image<unsigned char,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_SHORT): {
//                 T<itk::Image<short,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_USHORT): {
//                 T<itk::Image<unsigned short,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_INT): {
//                 T<itk::Image<int,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_UINT): {
//                 T<itk::Image<unsigned int,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_FLOAT): {
//                 T<itk::Image<float,2> > T0;
//                 return  T0.New(b); }
//            break;
//            case (type_DOUBLE): {
//                 T<itk::Image<double,2> > T0;
//                 return  T0.New(b); }
//            break;
//            default:
//            break;
//            }

//          }
//        else if(dim == 3) {
//            switch(type) {
//            case (type_UCHAR):
//            return add.func<itk::Image<unsigned char, 3> >(b);
//            break;
//            case (type_SHORT):
//            return add.func<itk::Image<short, 3> >(b);
//            break;
//            case (type_USHORT):
//            return add.func<itk::Image<unsigned short, 3> >(b);
//            break;
//            case (type_INT):
//            return add.func<itk::Image<int, 3> >(b);
//            break;
//            case (type_UINT):
//            return add.func<itk::Image<unsigned int, 3> >(b);
//            break;
//            case (type_FLOAT): {
//                T<itk::Image<float, 3> > T0;
//                return T0.New(b); }
//            return add.func<itk::Image<float, 3> >(b);
//            break;
//            case (type_DOUBLE):
//            return add.func<itk::Image<double, 3> >(b);
//            break;
//            default:
//            break;
//            }
//        }

//}

//        else
//        {
//            switch(type)
//            {
//            case (type_UCHAR):
//            return add.func<itk::Image<unsigned char, 4> >(b);
//            break;
//            case (type_SHORT):
//            return add.func<itk::Image<short, 4> >(b);
//            break;
//            case (type_USHORT):
//            return add.func<itk::Image<unsigned short, 4> >(b);
//            break;
//            case (type_INT):
//            return add.func<itk::Image<int, 4> >(b);
//            break;
//            case (type_UINT):
//            return add.func<itk::Image<unsigned int, 4> >(b);
//            break;
//            case (type_FLOAT):
//            return add.func<itk::Image<float, 4> >(b);
//            break;
//            case (type_DOUBLE):
//            return add.func<itk::Image<double, 4> >(b);
//            break;
//            default:
//            break;
//            }
//        }

//   }



      /* Main func to get image componenttype */
//      int IO_type::main_fun(const char *a, const char* &imagetype, unsigned &dim, itk::ImageIOBase::IOComponentType &componentType)
int IO_type::main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel)
{


      /* Use ImageIOFactory to automaticlly identify the IO */
      itk::ImageIOBase::Pointer ImageIO =  itk::ImageIOFactory::CreateImageIO(a, itk::ImageIOFactory::ReadMode);
      if(!ImageIO->CanReadFile(a)) {
        std::cout << a << ": MetaImageIO cannot read "
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





//    switch(componentType) {
//    case itk::ImageIOBase::CHAR: {
//          type = type_CHAR;
//
//      }
//          break;
//    case itk::ImageIOBase::UCHAR: {
//              type = type_UCHAR;
//
//          }
//          break;
//    case itk::ImageIOBase::SHORT: {
//              type = type_SHORT;
//
//            }
//          break;
//    case itk::ImageIOBase::USHORT: {
//              type = type_USHORT;
//
//            }
//          break;
//    case itk::ImageIOBase::INT: {
//              type = type_INT;
//
//            }
//          break;
//    case itk::ImageIOBase::UINT: {
//              type = type_UINT;
//
//            }
//        break;
//    case itk::ImageIOBase::FLOAT: {
//              type = type_FLOAT;
//
//        break;
//            }
//          break;
//    case itk::ImageIOBase::DOUBLE: {
//              type = type_DOUBLE;
//
//            }
//          break;
//        default:
//        std::cout << "Unsupported pixel type " << pixelType << " unable to read or write file"
//                        << std::endl;
//        break;
//    }
}





