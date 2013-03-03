#include "concatenateStack.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

//void print_usage()
//{
//    /* print usage */
//    printf (
//                "Usage: concatenateStack -input inputfile1 inputfile2 ... inputfilen -output outputprefix\n"
//                "Options:\n"
//                "-input         Name of image on top\n"
//                "-output        Name of the Outputprefix\n"
//                );
//}


int main(int argc, char* argv[])
{
    //    /* check arguments number */
    //    if( argc < 5)
    //    {
    //        print_usage();
    //        return -1;
    //    }



    /* read argument value */
    //    // myltiple input arguments
    //    std::vector<std::string> arguments(argv+1, argv+argc);
    //    std::vector<std::string>::iterator iter;
    //    std::string outputfile;
    //    unsigned int dim;
    //    std::string imagetype;


    //    // delete '-input'
    //    iter = find(arguments.begin(), arguments.end(), "-input");
    //    iter = arguments.erase(iter);

    //    for (iter = arguments.begin(); iter!=arguments.end(); iter++) {
    //        std::cout<<*iter<<"\t";
    //    }

    //    // delete '-output', rest arguments are all inputfiles
    //    iter = find(arguments.begin(), arguments.end(), "-output");
    //    iter = arguments.erase(iter);
    //    iter++;
    //    outputfile = *iter;
    //    iter = arguments.erase(iter);

    //    for (iter = arguments.begin(); iter!=arguments.end(); iter++) {
    //        std::cout<<*iter<<"\t";
    //    }
    //    for (int i=1; i<argc;i++) {

    //        if (!strcmp(argv[i], "-output")) {
    //            i++;
    //            outputfile = argv[i];
    //        }
    //        else {
    //            print_usage();
    //            return -1;
    //        }
    //    }

    std::string outputfile;
    std::vector<string> inputfile;
    unsigned int dim;
    std::string imagetype;

    string help[3] = {
    "Usage: concatenateStack -input inputfile1 inputfile2 ... inputfilen -output outputprefix\n"
    "Options:\n"
    "-input         Name of image on top\n"
    "-output        Name of the Outputprefix\n"
    };


    /* return all paras to main class */
    ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);
    outputfile = ap.OneParse("-output");
    inputfile = ap.MultiParse("-input");

    std::cout<<outputfile<<inputfile[0]<<inputfile[1]<<inputfile[2];


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile[0].c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(arguments[0].c_str(), imagetype, dim, channel);

    std::cout<<"\n"<<dim<<imagetype<<std::endl;


    itk::ConcatenateStack::Pointer filter = itk::ConcatenateStack::New();

    filter->inputfile = inputfile;
    filter->outputfile = outputfile;



    // update with image type
    if(dim == 3) {
       if(imagetype == "CHAR") { filter->Update<char, 3 >();}
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

     //     if(dim == 4) {
     //       /* only 3 channel float supported */
     //         //       const unsigned int VectorDimension = channel;
     //         //       const unsigned int VectorDimension = 3;
     //       const unsigned int ImageDimension = 3;
     //       //       typedef itk::Vector<float, VectorDimension>    PixelType;
     //       //       typedef itk::Image< PixelType, ImageDimension > ImageType;
     //       // Using VectorImage to determine component numbers at runtime
     //       typedef itk::VectorImage<float, ImageDimension> ImageType;
     //       filter->Update<ImageType>();

     //     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

