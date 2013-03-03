#include "IO_type.h"

// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{
    /* check arguments number */
    if( argc < 2)
    {
        std::cout<< "Usage:" << std::endl;
        std::cout<< argv[0] <<" "<< "inputFile1" << " "
                 << std::endl;
        return EXIT_FAILURE;
    }



    /* read argument value */
    const char *inputfile = argv[1];
    unsigned int dim;
    unsigned int channel;

    std::string byteorder;
    std::string imagetype;
    std::string pixeltype;
    std::vector<unsigned int> dimsize;
    std::vector<double> spacing;
    std::vector<double> origin;




    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile);
    byteorder = io.Return_ByteOrder();
    imagetype = io.Return_ImageType();
    pixeltype = io.Return_PixelType();
    dim = io.Return_DimensionNumber();
    channel = io.Return_ChannelNumber();
    dimsize.reserve(dim);
    dimsize = io.Return_DimensionSize();
    //    spacing.reserve(imageio->GetNumberOfDimensions());
    spacing = io.Return_Spacing();
    //    origin.reserve(imageio->GetNumberOfDimensions());
    origin = io.Return_Origin();


    std::cout<<"hello";


    /* return all paras to main class */
    std::cout<<"FileName:"<<inputfile<<std::endl;
    std::cout<<"ByteOrder:"<<byteorder<<std::endl;
    std::cout<<"DataType:"<<imagetype<<std::endl;
    std::cout<<"PixelType:"<<pixeltype<<std::endl;
    std::cout<<"Dimension:"<<dim<<std::endl;
    std::cout<<"DimensionSize:"<<dimsize[0]<<" "<<dimsize[1]<<" "<<dimsize[2]<<std::endl;
    std::cout<<"ChannelNumber:"<<channel<<std::endl;
    std::cout<<"Spacing:"<<spacing[0]<<" "<<spacing[1]<<" "<<spacing[2]<<std::endl;
    std::cout<<"Origin:"<<origin[0]<<" "<<origin[1]<<" "<<origin[2];



  return EXIT_SUCCESS;
}




