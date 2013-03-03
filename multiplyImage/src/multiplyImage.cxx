#include "multiplyImage.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{
    /* check arguments number */
    if( argc < 4)
    {
        std::cout<< "Usage:" << std::endl;
        std::cout<< argv[0] <<" "<< "inputFile1" << " "
                 << "inputFile2" << " " <<"outputFile"<< std::endl;
        return EXIT_FAILURE;
    }



    /* read argument value */
    const char *inputfile1 = argv[1];
    const char *inputfile2 = argv[2];
    const char *outputfile = argv[3];
    unsigned int dim;

    std::string imagetype;





    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile1);
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();






    /* return all paras to main class */
    //    std::cout<<inputfile1<<" "<<inputfile2<<" "<<outputfile<<" "
    //            <<dim<<imagetype<<pixeltype<<channel<<std::endl;
    //    std::cout<<dimsize[0]<<dimsize[1]<<dimsize[2]<<byteorder<<std::endl;
    //    std::cout<<spacing[0]<<spacing[1]<<spacing[2]<<origin[0]<<origin[1]<<origin[2];

    itk::MultiplyImage::Pointer filter = itk::MultiplyImage::New();

    filter->inputfile1 = std::string(inputfile1);
    filter->inputfile2 = std::string(inputfile2);
    filter->outputfile = std::string(outputfile);

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


     //    filter->Run();



  return EXIT_SUCCESS;
}




