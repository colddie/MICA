#include "reshapeImage.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class


// reshape like function in matlab

int main(int argc, char** argv)
{



    /* read argument value */
    //    const char *inputfile = argv[1];
    //    const char *outputfile = argv[2];

    const char *help[] = {
        "Usage: reshapeImage -input inputfile -output outputfile -size outputsize\n"
        "Options:\n"
        "-input            Name of the input file\n"
        "-output           Name of the output file\n"
        "-size             Output image size, format 'x y z'"
    };

    std::string inputfile;
    std::string outputfile;
    std::vector<unsigned int> outputsize;
    unsigned int dim;
    std::string imagetype;


    /* return all paras to main class */
    ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    ap.MarkAsRequired("-output");
    ap.MarkAsRequired("-size");
    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile = ap.OneParse("-input");
    outputfile = ap.OneParse("-output");
    outputsize = ap.MultiParse<unsigned int>("-size", "isnum");


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(inputfile, imagetype, dim, channel);





    /* return all paras to main class */
    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<std::endl;

    itk::ReshapeImage::Pointer filter = itk::ReshapeImage::New();

    filter->m_inputfile = inputfile;
    filter->m_outputfile = outputfile;
    filter->m_outputsize = outputsize;

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

     if(dim == 2) {
       if(imagetype == "CHAR") { filter->Update<char, 2>();}
       else if(imagetype == "UCHAR") { filter->Update<unsigned char, 2>();}
       else if(imagetype == "SHORT") { filter->Update<short, 2>();}
       else if(imagetype == "USHORT") { filter->Update<unsigned short, 2>();}
       else if(imagetype == "INT") { filter->Update<int, 2>();}
       else if(imagetype == "UINT") { filter->Update<unsigned int, 2>();}
       else if(imagetype == "FLOAT") { filter->Update<float, 2>();}
       else if(imagetype == "DOUBLE") { filter->Update<double, 2>();}
                }

//     if(dim == 4) {
//       /* only 3 channel float supported */
//         //       const unsigned int VectorDimension = channel;
//         //       const unsigned int VectorDimension = 3;
//         const unsigned int ImageDimension = 3;
//         //       typedef itk::Vector< float, VectorDimension >   PixelType;
//         //       typedef itk::Image< PixelType, ImageDimension > ImageType;
//         //       typedef itk::VectorImage<float, ImageDimension> ImageType;

//         filter->Update<float, ImageDimension>();
//     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

