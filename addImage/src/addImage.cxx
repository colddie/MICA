#include "addImage.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{
    //    /* check arguments number */
    //    if( argc < 4)
    //    {
    //        std::cout<< "Usage:" << std::endl;
    //        std::cout<< argv[0] <<" "<< "inputFile1" << " "
    //                 << "inputFile2" << " " <<"outputFile"<< std::endl;
    //        return EXIT_FAILURE;
    //    }



    /* read argument value */
    //    const char *inputfile1 = argv[1];
    //    const char *inputfile2 = argv[2];
    //    const char *outputfile = argv[3];

    const char *help[] = {
        "Usage: addImage -input inputfile1 inputfile2... inputfileN -output outputfile [-scale] scalevector [-average]\n"
        "Options:\n"
        "-input            Name of the input images\n"
        "-output           Name of the output image\n"
        "[-weight]         Weight to scale each input, default is '1 1...1'\n"
        "[-average]        Average of the all inputs"
    };

    std::vector<string> inputfile;
    std::string outputfile;
    bool isweight = false;
    std::vector<double> weight;
    bool isaverage = false;
    unsigned int dim;
    std::string imagetype;


    /* return all paras to main class */
    ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    ap.MarkAsRequired("-output");
    if (!ap.IsRequiredFound()) { exit(1); }


    inputfile  = ap.MultiParse("-input");
    outputfile = ap.OneParse("-output");
    isweight = ap.BoolParse("-weight");
    if (isweight) { weight = ap.MultiParse("-weight", "true"); }
    isaverage = ap.BoolParse("-average");



    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile[0].c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();



    /* return all paras to main class */
    std::cout<<inputfile[0]<<" "<<inputfile[1]<<" "<<outputfile<<" "
            <<dim<<imagetype<<std::endl;
    //    std::cout<<dimsize[0]<<dimsize[1]<<dimsize[2]<<byteorder<<std::endl;
    //    std::cout<<spacing[0]<<spacing[1]<<spacing[2]<<origin[0]<<origin[1]<<origin[2];

    itk::AddImage::Pointer filter = itk::AddImage::New();

    filter->inputfile = inputfile;
    filter->outputfile = outputfile;
    filter->isweight = isweight;
    if(isweight) { filter->weight = weight; }
    filter->isaverage = isaverage;

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

     if(dim == 4) {
       /* only 3 channel float supported */
         //       const unsigned int VectorDimension = channel;
         //       const unsigned int VectorDimension = 3;
         const unsigned int ImageDimension = 3;
         //       typedef itk::Vector< float, VectorDimension >   PixelType;
         //       typedef itk::Image< PixelType, ImageDimension > ImageType;
         //       typedef itk::VectorImage<float, ImageDimension> ImageType;

         filter->Update<float, ImageDimension>();
     }


     //         filter->Run();



  return EXIT_SUCCESS;
}




