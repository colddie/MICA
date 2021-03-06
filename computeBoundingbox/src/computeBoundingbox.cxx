#include "computeBoundingbox.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{
    //    /* check arguments number */
    //    if( argc < 3)
    //    {
    //        std::cout<< "Usage:" << std::endl;
    //        std::cout<< argv[0] <<" "<< "inputFile" << " "
    //                 << "outputFile" << " " << std::endl;
    //        return EXIT_FAILURE;
    //    }



    const char *help[] = {
        "Usage: computeBoundingbox -input inputfile [-val] valueToexclude\n"
        "Options:\n"
        "-input            Name of the input file\n"
        "[-val]            value to exclude"
    };

    std::string               inputfile;
    std::string               imagetype;
    unsigned int              dim;
    double                    val = 0.0;



    /* return all paras to main class */
    std::ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile  = ap.OneParse("-input");
    if (!ap.BoolParse("-val")) {
        val = atof(ap.OneParse("-val").c_str());
    }

    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype    = io.Return_ImageType();
    dim          = io.Return_DimensionNumber();


    /* return all paras to main class */
    std::cout<<inputfile<<" "<<dim<<imagetype<<std::endl;

    itk::ComputeBoundingbox::Pointer filter = itk::ComputeBoundingbox::New();

    filter->m_inputfile    = inputfile;
    filter->m_value        = val;



    // update with image type
    if(dim == 3) {
       if(imagetype == "CHAR")        { filter->Update<char, 3 >();}
       else if(imagetype == "UCHAR")  { filter->Update<unsigned char, 3>();}
       else if(imagetype == "SHORT")  { filter->Update<short, 3>();}
       else if(imagetype == "USHORT") { filter->Update<unsigned short, 3>();}
       else if(imagetype == "INT")    { filter->Update<int, 3>();}
       else if(imagetype == "UINT")   { filter->Update<unsigned int, 3>();}
       else if(imagetype == "FLOAT")  { filter->Update<float, 3>();}
       else if(imagetype == "DOUBLE") { filter->Update<double, 3>();}
    }

     if(dim == 2) {
       if(imagetype == "CHAR")        { filter->Update<char, 2>();}
       else if(imagetype == "UCHAR")  { filter->Update<unsigned char, 2>();}
       else if(imagetype == "SHORT")  { filter->Update<short, 2>();}
       else if(imagetype == "USHORT") { filter->Update<unsigned short, 2>();}
       else if(imagetype == "INT")    { filter->Update<int, 2>();}
       else if(imagetype == "UINT")   { filter->Update<unsigned int, 2>();}
       else if(imagetype == "FLOAT")  { filter->Update<float, 2>();}
       else if(imagetype == "DOUBLE") { filter->Update<double, 2>();}
     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

