#include "extractImage.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

void print_usage()
{
    /* print usage */
    printf (
                "Usage: extractImage [options] inputfile outputprefix\n"
                "Options:\n"
                "-plane         Set the desired plane, '0' is x, '1' is y, up to '3'\n"
                "-slice         Slice number to extract\n"
                "-input         Name of the Inputfile\n"
                "-output        Name of the Outputprefix\n"
                );
}


int main(int argc, char** argv)
{
    /* check arguments number */
    if( argc < 5)
    {
        print_usage();
        return -1;
    }



    /* read argument value */
    const char *inputfile;
    const char *outputfile;
    unsigned int plane;
    unsigned int slice;
    unsigned int dim;
    unsigned int channel;
    std::string imagetype;
    //    itk::ImageIOBase::IOComponentType componentType;

    for (int i=1; i<argc;i++) {
        if (!strcmp(argv[i], "-plane")) {
            i++;
            plane = atoi(argv[i]);
        }
        else  if (!strcmp(argv[i], "-slice")) {
            i++;
            slice = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-input")) {
            i++;
            inputfile = argv[i];
        }
        else if (!strcmp(argv[i], "-output")) {
            i++;
            outputfile = argv[i];
        }
        else {
            print_usage();
            return -1;
        }
    }


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile);
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    channel = io.Return_ChannelNumber();
    //    io.main_fun(inputfile, imagetype, dim, channel);





    /* return all paras to main class */

    std::cout<<inputfile<<" "<<dim<<imagetype<<channel<<plane<<slice<<std::endl;
    //    itk::ImageIOBase::IOComponentType componenttype = itk::ImageIOBase::GetComponentTypeFromString(imagetype);

    itk::ExtractImage::Pointer filter = itk::ExtractImage::New();

    //    filter->SetArgs(args);
    filter->inputfile = std::string(inputfile);
    filter->outputfile = std::string(outputfile);
    filter->plane = plane;
    filter->slice = slice;
    filter->channel = channel;

    // update with image type
    if(dim == 3) {
       if(imagetype == "CHAR") { filter->Update<char, 3>();}
       else if(imagetype == "UCHAR") { filter->Update<unsigned char, 3>();}
       else if(imagetype == "SHORT") { filter->Update<short, 3>();}
       else if(imagetype == "USHORT") { filter->Update<unsigned short, 3>();}
       else if(imagetype == "INT") { filter->Update<int, 3>();}
       else if(imagetype == "UINT") { filter->Update<unsigned int, 3>();}
       else if(imagetype == "FLOAT") { filter->Update<float, 3>();}
       else if(imagetype == "DOUBLE") { filter->Update<double, 3>();}
            }

//     if(dim == 2) {
//       if(imagetype == "CHAR") { filter->Update<char, 2>();}
//       else if(imagetype == "UCHAR") { filter->Update<unsigned char, 2>();}
//       else if(imagetype == "SHORT") { filter->Update<short, 2>();}
//       else if(imagetype == "USHORT") { filter->Update<unsigned short, 2>();}
//       else if(imagetype == "INT") { filter->Update<int, 2>();}
//       else if(imagetype == "UINT") { filter->Update<unsigned int, 2>();}
//       else if(imagetype == "FLOAT") { filter->Update<float, 2>();}
//       else if(imagetype == "DOUBLE") { filter->Update<double, 2>();}
//                }

     if(dim == 4) {
//         /* only 3 channel float supported */
//         //       const unsigned int VectorDimension = channel;
//         //       const unsigned int VectorDimension = 3;
//         const unsigned int ImageDimension = 3;
//         //       typedef itk::Vector<float, VectorDimension>    PixelType;
//         //       typedef itk::Image< PixelType, ImageDimension > ImageType;
//         // Using VectorImage to determine component numbers at runtime
//         typedef itk::VectorImage<float, ImageDimension> ImageType;
//         filter->Update<ImageType>();
         filter->Update<float, 3>();
     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

