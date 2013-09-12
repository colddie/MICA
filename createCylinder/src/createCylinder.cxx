#include "createCylinder.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{




    const char *help[] = {
        ""
        "Usage:  createCylinder\n"
        "-output outputfile\n"
        "-sz     image dimension size\n"
        "-c      center coordinate (mm)\n"
        "-r      radius (mm)\n"
        "-l      medial axis length (mm)\n"
        "-pixel  pixel type, supported: (unsigned) char,  (unsigned) short, float, double\n"
        ""
        "Options:\n"
        "-o      orientation, default meidal axis point to x: 1 0 0\n"
        "-sp     image spacing (mm)\n"
        "Only support 3D"
    };

    unsigned int  dimension = 3;

    std::string                outputfile;
    std::string                imagetype;
    std::vector <unsigned int> dimensionsize;
    std::vector <double>       center;
    double                     radius;
    double                     length;
    std::vector <double>       imagespacing;
    std::vector <double>       orientation;

    /* return all paras to main class */
    std::ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);
    outputfile    =  ap.OneParse("-output");
    imagetype     =  ap.OneParse("-pixel");
    dimensionsize =  ap.MultiParse <unsigned int>("-sz", "isnum");
    center        =  ap.MultiParse <double>("-c", "isnum");
    radius        =  atof(ap.OneParse("-r").c_str());
    length        =  atof(ap.OneParse("-l").c_str());

    imagespacing.reserve(dimension);
    if (!ap.BoolParse("-sp")) {
        imagespacing = ap.MultiParse <double>("-sp", "isnum");
    } else {
        for (unsigned int i = 0; i < dimension; i++) {
            imagespacing.push_back(1);
        }
    }

    orientation.assign(dimension*dimension, 0);
    if (!ap.BoolParse("-o")) {
        orientation = ap.MultiParse <double>("-o", "isnum");
    } else {
        for (unsigned int i = 0; i < dimension; i++) {
            orientation[i*(dimension+1)] = 1.0;
        }
    }

    ap.MarkAsRequired("-output");
    ap.MarkAsRequired("-sz");
    ap.MarkAsRequired("-c");
    ap.MarkAsRequired("-r");
    ap.MarkAsRequired("-l");
    ap.MarkAsRequired("-pixel");
    if (!ap.IsRequiredFound()) { exit(1); }




    /* return all paras to main class */
    std::cout<<outputfile<<" "<<dimension<<imagetype<<std::endl;

    itk::createCylinder::Pointer filter = itk::createCylinder::New();


    filter->m_outputfile    = outputfile;
    filter->m_dimensionsize = dimensionsize;
    filter->m_center        = center;
    filter->m_radius        = radius;
    filter->m_length        = length;
    filter->m_imagespacing  = imagespacing;
    filter->m_orientation   = orientation;

    // update with image type
       if(imagetype == "char") { filter->Update<char, 3 >();}
       else if(imagetype == "uchar")  { filter->Update<unsigned char, 3>();}
       else if(imagetype == "short")  { filter->Update<short, 3>();}
       else if(imagetype == "ushort") { filter->Update<unsigned short, 3>();}
       else if(imagetype == "int")    { filter->Update<int, 3>();}
       else if(imagetype == "uint")   { filter->Update<unsigned int, 3>();}
       else if(imagetype == "float")  { filter->Update<float, 3>();}
       else if(imagetype == "double") { filter->Update<double, 3>();}



  return EXIT_SUCCESS;
}

