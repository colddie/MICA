#include "clipIntensity.h"


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
        "Usage: clipIntensity -input inputfile -output outputfile -up uplevel -low lowlevel\n"
        "Options:\n"
        "-input            Name of the input file\n"
        "-output           Name of the output file\n"
        "-up               Upper threshold level\n"
        "-low              Lower threshold level"
    };

    std::string               inputfile;
    std::string               outputfile;
    std::string               imagetype;
    unsigned int              dim;
    double                    upper;
    double                    lower;
    MODE mode = NONE;


    /* return all paras to main class */
    std::ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    ap.MarkAsRequired("-output");

    //    std::vector<std::string> optional_args;
    //    optional_args.push_back("-up");
    //    optional_args.push_back("-low");
    //    ap.MarkOneOfAsRequired(optional_args);
    //    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile  = ap.OneParse("-input");
    outputfile = ap.OneParse("-output");
    if (!ap.BoolParse("-up")) {
        upper   = atof(ap.OneParse("-up").c_str());
        mode = UP;
    }
    if (!ap.BoolParse("-low")) {
        lower   = atof(ap.OneParse("-low").c_str());
        if (mode != UP) {
            mode = LOW;
        } else if (upper >= lower) {
            mode = BOTH;
        } else {
            mode = NONE;
            return EXIT_FAILURE;
        }
    }

    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype    = io.Return_ImageType();
    dim          = io.Return_DimensionNumber();


    /* return all paras to main class */
    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<std::endl;

    itk::ClipIntensity::Pointer filter = itk::ClipIntensity::New();

    filter->m_inputfile    = inputfile;
    filter->m_outputfile   = outputfile;
    filter->m_upper     = upper;
    filter->m_lower     = lower;
    filter->m_mode      = mode;

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

