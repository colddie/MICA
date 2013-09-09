#include "createSphere.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{




    const char *help[] = {
        ""
        "Usage:  createSphere\n"
        "-output outputfile\n"
        "-sz     image dimension size\n"
        "-c      center coordinate (mm)\n"
        "-r      radius (mm)\n"
        "-pixel  pixel type, supported: (unsigned) char,  (unsigned) short, float, double\n"
        ""
        "Options:\n"
        "-sp     image spacing (mm)\n"
        "-dim    image dimension, default 3, supported: 2, 3"
    };

    std::string outputfile;
    unsigned int dimension;
    std::string imagetype;
    std::vector <unsigned int> dimensionsize;
    std::vector <double>       center;
    double       radius;
    std::vector <double>       imagespacing;

    /* return all paras to main class */
    std::ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);
    outputfile = ap.OneParse("-output");
    imagetype = ap.OneParse("-pixel");
    dimensionsize = ap.MultiParse <unsigned int>("-sz", "isnum");
    center = ap.MultiParse <double>("-c", "isnum");
    radius = atof(ap.OneParse("-r").c_str());

    if (!ap.BoolParse("-dim")) {
        dimension = atoi(ap.OneParse("-dim").c_str());
    } else {
        dimension = 3;        
    }

    imagespacing.reserve(dimension);
    if (!ap.BoolParse("-sp")) {
        imagespacing = ap.MultiParse <double>("-sp", "isnum");
    } else {
        imagespacing.push_back(1);
        imagespacing.push_back(1);
        imagespacing.push_back(1);
    }

    ap.MarkAsRequired("-output");
    ap.MarkAsRequired("-sz");
    ap.MarkAsRequired("-c");
    ap.MarkAsRequired("-r");
    ap.MarkAsRequired("-pixel");
    if (!ap.IsRequiredFound()) { exit(1); }




    /* return all paras to main class */
    std::cout<<outputfile<<" "<<dimension<<imagetype<<std::endl;

    itk::createSphere::Pointer filter = itk::createSphere::New();


    filter->m_outputfile = outputfile;
    filter->m_dimensionsize = dimensionsize;
    filter->m_center = center;
    filter->m_radius = radius;
    filter->m_imagespacing = imagespacing;


    // update with image type
    if(dimension == 3) {
       if(imagetype == "char") { filter->Update<char, 3 >();}
       else if(imagetype == "uchar") { filter->Update<unsigned char, 3>();}
       else if(imagetype == "short") { filter->Update<short, 3>();}
       else if(imagetype == "ushort") { filter->Update<unsigned short, 3>();}
       else if(imagetype == "int") { filter->Update<int, 3>();}
       else if(imagetype == "uint") { filter->Update<unsigned int, 3>();}
       else if(imagetype == "float") { filter->Update<float, 3>();}
       else if(imagetype == "double") { filter->Update<double, 3>();}
    }

     if(dimension == 2) {
       if(imagetype == "char") { filter->Update<char, 2>();}
       else if(imagetype == "uchar") { filter->Update<unsigned char, 2>();}
       else if(imagetype == "short") { filter->Update<short, 2>();}
       else if(imagetype == "ushort") { filter->Update<unsigned short, 2>();}
       else if(imagetype == "int") { filter->Update<int, 2>();}
       else if(imagetype == "uint") { filter->Update<unsigned int, 2>();}
       else if(imagetype == "float") { filter->Update<float, 2>();}
       else if(imagetype == "double") { filter->Update<double, 2>();}
     }




  return EXIT_SUCCESS;
}

