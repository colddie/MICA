#include "resampleImage.h"

//#include <vector>


// IOFactory already support multi format, add here to enable multi type
// see IO_type class


  //     Compute and set the output spacing
  //     Compute the output spacing from input spacing and old and new sizes.
  //
  //     The computation must be so that the following holds:
  //
  //     new width         old x spacing
  //     ----------   =   ---------------
  //     old width         new x spacing
  //
  //
  //     new height         old y spacing
  //    ------------  =   ---------------
  //     old height         new y spacing


int main(int argc, char** argv)
{





    const char *help[] = {
        "Usage: resampleImage [options] -input inputfile -output outputfile -scale scalenumber -size newdimsize -spa newspacing\n"
        "Options:\n"
        "-input         Name of the input file\n"
        "-output        Name of the output file\n"
        "[-scale]       Scale number in each dimension, format 'x y z'\n"
        "[-size]        New dimension size of the output image\n"
        "[-spa]         New spacing of the output image\n"
        "Notice: at least one optional argument should be given"


    };


    std::string inputfile;
    std::string outputfile;
    bool isscale = false;
    std::vector<double> scale;
    bool isnewdimsize = false;
    std::vector<unsigned int> newdimsize;
    bool isnewspacing = false;
    std::vector<double> newspacing;
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

    std::vector<string> optional_arguments;
    optional_arguments.push_back("-scale");
    optional_arguments.push_back("-size");
    optional_arguments.push_back("-spa");
    ap.MarkOneOfAsRequired(optional_arguments);

    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile = ap.OneParse("-input");
    outputfile = ap.OneParse("-output");
    isscale = ap.BoolParse("-scale");
    if (isscale) {
        scale = ap.MultiParse<double>("-scale", "isnum"); }
    isnewdimsize = ap.BoolParse("-size");
    if (isnewdimsize) {
        newdimsize = ap.MultiParse<unsigned int>("-size", "isnum");}
    isnewspacing = ap.BoolParse("-spa");
    if (isnewspacing) {
        newspacing = ap.MultiParse<double>("-spa", "isnum"); }



    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    spacing = io.Return_Spacing();
    origin = io.Return_Origin();
    dimsize = io.Return_DimensionSize();
    //    io.main_fun(inputfile, imagetype, dim, channel);



    // transfer dimension size and spacing ratio to scale factor
    scale.resize(dim);
    if (isnewdimsize) {
        transform(newdimsize.begin(), newdimsize.end(), dimsize.begin(), scale.begin(),divides<unsigned int>());
    }
    if (isnewspacing) {
        transform(spacing.begin(), spacing.end(), newspacing.begin(), scale.begin(),divides<double>());
    }

    // release unused vectors
    if (newdimsize.empty()) {
        std::vector<unsigned int> vec;
        vec.swap(newdimsize);
    }
    if (newspacing.empty()) {
        std::vector<double> vec;
        vec.swap(newspacing);
    }

    /* return all paras to main class */
    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<" "<<scale[0]<<scale[1]<<scale[2]<<std::endl;

    itk::ResampleImage::Pointer filter = itk::ResampleImage::New();

    filter->m_inputfile = inputfile;
    filter->m_outputfile = outputfile;
    filter->m_scale = scale;
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

