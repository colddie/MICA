#include "makeStack.h"


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

void print_usage()
{
    /* print usage */
    printf (
                "Usage: makeStack -output outputprefix\n"
                "Options:\n"
                "-output        Name of the Outputprefix\n"
                );
}


int main(int argc, char* argv[])
{
    /* check arguments number */
    if( argc < 3)
    {
        print_usage();
        return -1;
    }



    /* read argument value */
    // myltiple input arguments
    QDir dir;
    QStringList filenamelist;
    std::vector<std::string> arguments;
    std::string outputfile;
    unsigned int dim;
    std::string imagetype;

    for (int i=1; i<argc; i++) {

        //            if (!strcmp(argv[i], "-dir")) {
        //                i++;
        //                dir.setPath(QString::fromUtf8(argv[i]));
        //                filenamelist = dir.entryList();
        //                filenamelist.pop_front();
        //                filenamelist.pop_front();
        //                arguments.reserve(filenamelist.count());
        //                for (int j=0; j<filenamelist.count(); j++) {
        //                    std::string str = filenamelist[j].toLocal8Bit().constData();
        //                    arguments.push_back(str);
        //                }
        //            }
        if (!strcmp(argv[i], "-output")) {
            i++;
            outputfile = argv[i];
        }
        else {
            print_usage();
            return -1;
        }
    }

    /* read all filenames in current folder */
    dir.setPath(dir.currentPath());
    filenamelist = dir.entryList();
    filenamelist.pop_front();
    filenamelist.pop_front();
    arguments.reserve(filenamelist.count());
    for (int j=0; j<filenamelist.count(); j++) {
        std::string str = filenamelist[j].toLocal8Bit().constData();
        arguments.push_back(str);
    }
    std::vector<std::string>::iterator iter;
    // delete 'argv[0]'
    iter = find(arguments.begin(), arguments.end(), argv[0]);
    iter = arguments.erase(iter);


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(arguments[0].c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(arguments[0].c_str(), imagetype, dim, channel);





    /* return all paras to main class */
    std::cout<<arguments.size()<<arguments[0]<<arguments[1]<<arguments[2]<<std::endl;
    std::cout<<"\n"<<dim<<imagetype<<std::endl;

    itk::MakeStack::Pointer filter = itk::MakeStack::New();

    filter->inputfile = arguments;
    filter->outputfile = outputfile;



    // update with image type
    //    if(dim == 3) {
    //       if(imagetype == "CHAR") { filter->Update<char, 3 >();}
    //       else if(imagetype == "UCHAR") { filter->Update<unsigned char, 3>();}
    //       else if(imagetype == "SHORT") { filter->Update<short, 3>();}
    //       else if(imagetype == "USHORT") { filter->Update<unsigned short, 3>();}
    //       else if(imagetype == "INT") { filter->Update<int, 3>();}
    //       else if(imagetype == "UINT") { filter->Update<unsigned int, 3>();}
    //       else if(imagetype == "FLOAT") { filter->Update<float, 3>();}
    //       else if(imagetype == "DOUBLE") { filter->Update<double, 3>();}
    //            }

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

