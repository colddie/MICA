#include "IO_format.h"



int IO_format::format_fun(char *b[])
{
//    enum IOformat a = main_fun(b);
//    switch(b)
//    {
//    case (META):
//        itk::MetaImageIO::Pointer ImageIO = itk::MetaImageIO::New();
//        return ImageIO;
//        break;






//    }



}


int IO_format::main_fun(char *b[])
{
    char *string = b[1];
//    char *outputfile = b[3];


    if (!strcmp (string,"mha")||!strcmp (string,"mhd")) {
        return META;
    }
    else if (!strcmp (string,"hdr")) {
        return ANALYZE;
    }
    else if (!strcmp (string,"raw")) {
        return RAW;
    }
    else if (!strcmp (string,"bmp")) {
        return BMP;
    }
    else if (!strcmp (string,"jpeg")) {
        return JPEG;
    }
    else if (!strcmp (string,"nift")) {
        return NIFT;
    }
    else if (!strcmp (string,"nrrd")) {
        return NRRD;
    }
    else if (!strcmp (string,"vtk")) {
        return VTK;
    }
    else if (!strcmp (string,"png")) {
        return PNG;
    }
    else if (!strcmp (string,"tiff")) {
        return TIFF;
    }
    else {
        return FORMAT_UNDEFINED;
    }


    return 0;
}
