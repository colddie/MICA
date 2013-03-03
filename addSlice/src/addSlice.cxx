#include "addSlice.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

void print_usage()
{
    /* print usage */
    printf (
                "Usage: addSlice -input stackimage -slice sliceimage -output outputprefix -number position_toinsert\n"
                "Options:\n"
                "-input         Name of image on top\n"
                "-slice         Name of the slice image to insert;blank indicates add empty slice\n"
                "-output        Name of the Outputprefix\n"
                "-number        Slice number to insert new slice after\n"
                );
}


int main(int argc, char* argv[])
{
    /* check arguments number */
    if( argc < 9)
    {
        print_usage();
        return -1;
    }



    /* read argument value */
    // myltiple input arguments
    std::string inputfile;
    std::string slicefile;
    std::string outputfile;
    unsigned int slicenumber;
    bool add_emptyslice = FALSE;
    unsigned int dim;
    unsigned int channel;
    std::string imagetype;


        for (int i=1; i<argc;i++) {

            if (!strcmp(argv[i], "-input")) {
                i++;
                inputfile = argv[i];
            }
            else if (!strcmp(argv[i], "-slice")) {
                i++;
                if (!strcmp(argv[i], "blank")) {
                    add_emptyslice = TRUE;
                } else {
                    slicefile = argv[i];
                }
            }
            else if (!strcmp(argv[i], "-output")) {
                i++;
                outputfile = argv[i];
            }
            else if (!strcmp(argv[i], "-number")) {
                i++;
                slicenumber = atoi(argv[i]);
            }
            else {
                print_usage();
                return -1;
            }
        }


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(inputfile.c_str(), imagetype, dim, channel);





    /* return all paras to main class */
    std::cout<<"\n"<<dim<<imagetype<<std::endl;

    itk::AddSlice::Pointer filter = itk::AddSlice::New();

    filter->imagefile = inputfile;
    filter->outputfile = outputfile;
    filter->slicenumber = slicenumber;
    filter->add_emptyslice = add_emptyslice;
    if (!add_emptyslice) { filter->slicefile = slicefile; }



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

    //     if(dim == 2) {
    //       if(imagetype == "CHAR") { filter->Update<char, 2>();}
    //       else if(imagetype == "UCHAR") { filter->Update<unsigned char, 2>();}
    //       else if(imagetype == "SHORT") { filter->Update<short, 2>();}
    //       else if(imagetype == "USHORT") { filter->Update<unsigned short, 2>();}
    //       else if(imagetype == "INT") { filter->Update<int, 2>();}
    //       else if(imagetype == "UINT") { filter->Update<unsigned int, 2>();}
    //       else if(imagetype == "FLOAT") { filter->Update<float, 2>();}
    //       else if(imagetype == "DOUBLE") { filter->Update<double, 2>();}
    //                }

     //     if(dim == 4) {
     //       /* only 3 channel float supported */
     //         //       const unsigned int VectorDimension = channel;
     //         //       const unsigned int VectorDimension = 3;
     //       const unsigned int ImageDimension = 3;
     //       //       typedef itk::Vector<float, VectorDimension>    PixelType;
     //       //       typedef itk::Image< PixelType, ImageDimension > ImageType;
     //       // Using VectorImage to determine component numbers at runtime
     //       typedef itk::VectorImage<float, ImageDimension> ImageType;
     //       filter->Update<ImageType>();

     //     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

