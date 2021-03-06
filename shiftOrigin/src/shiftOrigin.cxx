#include "shiftOrigin.h"

//#include <vector>


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

// shift the image origin

int main(int argc, char** argv)
{


    const char *help[] = {
        "Usage: shiftOrigin -input inputfile -output outputfile -shift translation\n"
        "Options:\n"
        "-input         Name of the input file\n"
        "-output        Name of the output file\n"
        "-shift         Shift of the origin, format 'x y z'"

    };


    std::string inputfile;
    std::string outputfile;
    std::vector<double> shift;
    unsigned int dim;
    std::string imagetype;
    std::vector<double> spacing;
    std::vector<double> origin;
    std::vector<unsigned int> dimsize;



    /* return all paras to main class */
    ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    ap.MarkAsRequired("-output");
    ap.MarkAsRequired("-shift");
    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile = ap.OneParse("-input");
    outputfile = ap.OneParse("-output");
    shift = ap.MultiParse<double>("-shift", "isnum");




    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    spacing = io.Return_Spacing();
    origin = io.Return_Origin();
    dimsize = io.Return_DimensionSize();
    //    io.main_fun(inputfile, imagetype, dim, channel);





    /* return all paras to main class */
    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<" "<<shift[0]<<shift[1]<<shift[2]<<std::endl;

    itk::ShiftOrigin::Pointer filter = itk::ShiftOrigin::New();

    filter->m_inputfile = inputfile;
    filter->m_outputfile = outputfile;
    filter->m_shift = shift;
    filter->m_spacing = spacing;
    filter->m_origin = origin;
    filter->m_dimsize = dimsize;



    // update with image type
    if(dim == 3) {
           if(imagetype == "CHAR") {filter->Update<char, 3>();}
           if(imagetype == "UCHAR") {filter->Update<unsigned char, 3>();}
           if(imagetype == "SHORT") {filter->Update<short, 3>();}
           if(imagetype == "USHORT") {filter->Update<unsigned short, 3>();}
           if(imagetype == "INT") {filter->Update<int, 3>();}
           if(imagetype == "UINT") {filter->Update<unsigned int, 3>();}
           if(imagetype == "FLOAT") {filter->Update<float, 3>();}
           if(imagetype == "DOUBLE") {filter->Update<double, 3>();}
    }

    if(dim == 2) {
           if(imagetype == "CHAR") {filter->Update<char, 2>();}
           if(imagetype == "UCHAR") {filter->Update<unsigned char, 2>();}
           if(imagetype == "SHORT") {filter->Update<short, 2>();}
           if(imagetype == "USHORT") {filter->Update<unsigned short, 2>();}
           if(imagetype == "INT") {filter->Update<int, 2>();}
           if(imagetype == "UINT") {filter->Update<unsigned int, 2>();}
           if(imagetype == "FLOAT") {filter->Update<float, 2>();}
           if(imagetype == "DOUBLE") {filter->Update<double, 2>();}
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

