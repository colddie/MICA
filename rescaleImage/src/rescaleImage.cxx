#include "rescaleImage.h"

//#include <vector>


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

//Casting of scalar images is done by the itk::RescaleImageFilter or
//itk::ShiftScaleImageFilter

void print_usage()
{
    /* print usage */
    printf (
                "Usage: rescaleImage [options] inputfile outputprefix\n"
                "Options:\n"
                "-bound       Set the desired minimum and maximum pixel values of the output image\n"
                "[-type]      Outputimage component type(if not given, same as input)\n"
                "-input       Name of the Inputfile\n"
                "-output      Name of the Outputprefix\n"

                );
}


int main(int argc, char** argv)
{



    /* check arguments number */
    //    if( argc < 4)
    //    {
    //        std::cout<< "Usage:" << std::endl;
    //        std::cout<< argv[0] <<" "<< "inputFile" << " "
    //                 << "outputFile" << " " <<"type_to_cast"<< std::endl;
    //        return EXIT_FAILURE;
    //    }

    std::vector<double> bound(2);
    std::string imagetypeo = "";
    const char *inputfile = NULL;
    const char *outputfile = NULL;

    for(int i=1; i<argc; i++) {
        if (!strcmp(argv[i], "-bound")) {
            i++;
            bound[0] = atof(argv[i]);
            i++;
            bound[1] = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-type")) {
            i++;
            imagetypeo = std::string(argv[i]);
        }
        else if (!strcmp(argv[i], "-input")) {
            i++;
            inputfile = strdup(argv[i]);
        }
        else if (!strcmp(argv[i], "-output")) {
            i++;
            outputfile = argv[i];
        }
        else {
            print_usage();
            return EXIT_FAILURE;
        }
    }

//    if(!b || ) {
//        print_usage();
//        return EXIT_FAILURE;
//    }


    /* read argument value */
    //    const char *inputfile = argv[1];
    //    const char *outputfile = argv[2];
    //    std::string imagetypeo = std::string(argv[3]);
    //    std::vector<double> bound(2);
    //    bound[0] = atof(argv[4]);
    //    bound[1] = atof(argv[5]);

    unsigned int dim;

    std::string imagetype;





    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile);
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(inputfile, imagetype, dim, channel);


    // In case no output type specified
    if(imagetypeo == "") {
        imagetypeo = imagetype;
    }



    /* return all paras to main class */

    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<imagetypeo<<" "<<bound[0]<<bound[1]<<std::endl;

    itk::RescaleImage::Pointer filter = itk::RescaleImage::New();

    filter->inputfile = std::string(inputfile);
    filter->outputfile = std::string(outputfile);
    filter->low = bound[0];
    filter->up = bound[1];


    // update with image type
    if(dim == 3) {
       if(imagetype == "CHAR") {
           if(imagetypeo == "CHAR") {filter->Update<char, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<char, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<char, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<char, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<char, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<char, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<char, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<char, double, 3>();}
       }
       else if(imagetype == "UCHAR") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned char, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned char, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned char, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned char, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<unsigned char, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned char, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned char, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned char, double, 3>();}
       }
       else if(imagetype == "SHORT") {
           if(imagetypeo == "CHAR") {filter->Update<short, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<short, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<short, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<short, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<short, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<short, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<short, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<short, double, 3>();}
       }
       else if(imagetype == "USHORT") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned short, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned short, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned short, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned short, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<unsigned short, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned short, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned short, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned short, double, 3>();}
       }
       else if(imagetype == "INT") {
           if(imagetypeo == "CHAR") {filter->Update<int, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<int, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<int, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<int, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<int, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<int, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<int, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<int, double, 3>();}
       }
       else if(imagetype == "UINT") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned int, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned int, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned int, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned int, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<unsigned int, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned int, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned int, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned int, double, 3>();}
       }
       else if(imagetype == "FLOAT") {
           if(imagetypeo == "CHAR") {filter->Update<float, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<float, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<float, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<float, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<float, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<float, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<float, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<float, double, 3>();}
       }
       else if(imagetype == "DOUBLE") {
           if(imagetypeo == "CHAR") {filter->Update<double, char, 3>();}
           if(imagetypeo == "UCHAR") {filter->Update<double, unsigned char, 3>();}
           if(imagetypeo == "SHORT") {filter->Update<double, short, 3>();}
           if(imagetypeo == "USHORT") {filter->Update<double, unsigned short, 3>();}
           if(imagetypeo == "INT") {filter->Update<double, int, 3>();}
           if(imagetypeo == "UINT") {filter->Update<double, unsigned int, 3>();}
           if(imagetypeo == "FLOAT") {filter->Update<double, float, 3>();}
           if(imagetypeo == "DOUBLE") {filter->Update<double, double, 3>();}
    }
    }

    if(dim == 2) {
       if(imagetype == "CHAR") {
           if(imagetypeo == "CHAR") {filter->Update<char, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<char, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<char, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<char, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<char, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<char, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<char, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<char, double, 2>();}
       }
       else if(imagetype == "UCHAR") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned char, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned char, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned char, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned char, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<unsigned char, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned char, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned char, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned char, double, 2>();}
       }
       else if(imagetype == "SHORT") {
           if(imagetypeo == "CHAR") {filter->Update<short, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<short, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<short, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<short, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<short, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<short, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<short, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<short, double, 2>();}
       }
       else if(imagetype == "USHORT") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned short, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned short, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned short, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned short, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<unsigned short, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned short, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned short, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned short, double, 2>();}
       }
       else if(imagetype == "INT") {
           if(imagetypeo == "CHAR") {filter->Update<int, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<int, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<int, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<int, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<int, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<int, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<int, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<int, double, 2>();}
       }
       else if(imagetype == "UINT") {
           if(imagetypeo == "CHAR") {filter->Update<unsigned int, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<unsigned int, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<unsigned int, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<unsigned int, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<unsigned int, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<unsigned int, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<unsigned int, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<unsigned int, double, 2>();}
       }
       else if(imagetype == "FLOAT") {
           if(imagetypeo == "CHAR") {filter->Update<float, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<float, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<float, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<float, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<float, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<float, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<float, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<float, double, 2>();}
       }
       else if(imagetype == "DOUBLE") {
           if(imagetypeo == "CHAR") {filter->Update<double, char, 2>();}
           if(imagetypeo == "UCHAR") {filter->Update<double, unsigned char, 2>();}
           if(imagetypeo == "SHORT") {filter->Update<double, short, 2>();}
           if(imagetypeo == "USHORT") {filter->Update<double, unsigned short, 2>();}
           if(imagetypeo == "INT") {filter->Update<double, int, 2>();}
           if(imagetypeo == "UINT") {filter->Update<double, unsigned int, 2>();}
           if(imagetypeo == "FLOAT") {filter->Update<double, float, 2>();}
           if(imagetypeo == "DOUBLE") {filter->Update<double, double, 2>();}
    }
    }

    //     if(dim == 4) {
    //       /* only 3 channel float supported */
    ////       const unsigned int VectorDimension = channel;
    //       const unsigned int VectorDimension = 3;
    //       const unsigned int ImageDimension = 3;
    //       typedef itk::Vector< float, VectorDimension >    PixelType;
    //       typedef itk::Image< PixelType, ImageDimension > ImageType;
    //       filter->Update<ImageType>();

    //     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

