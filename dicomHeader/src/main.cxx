#include "dicomHeader.h"
#include "IO_type.h"
#include "argumentsParse.h"

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

// IOFactory already support multi format, add here to enable multi type
// see IO_type class

int main(int argc, char** argv)
{

    const char *help[] = {
        "Usage: dicomHeader -input inputDicom [-print] slideNumber\n"
        "[-modify] [-add] [-delete] -tagID tagID -tagValue tagValue -output outputDicom\n"
        "Options:\n"
        "-input            Name of the dicom series\n"
        "[-output]         Name of the dicom series\n"
        "[-print]          Slide number has the header to display\n"
        "[-modify]         Tag to change\n"
        "[-add]            Tag to insert\n"
        "[-delete]         Tag to delete\n"
        "[-tagID]          TagID\n"
        "[-tagValue]       TagValue"
    };

    std::string inputdicom;
    std::string outputdicom;
    unsigned int slidenum;
    bool isoutput;
    bool isprint;
    bool ismodify;
    bool isadd;
    bool isdelete;
    std::string tagid;
    std::string tagvalue;


    /* return all paras to main class */
    std::ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    //    ap.MarkAsRequired("-output");
    if (!ap.IsRequiredFound()) { exit(1); }

    inputdicom  = ap.OneParse("-input");
    isoutput = ap.BoolParse("-output");
    if (!isoutput) {
        outputdicom = ap.OneParse("-output"); }
    isprint = ap.BoolParse("-print");
    if(!isprint) {
    slidenum = atoi(ap.OneParse("-print").c_str()); }


    ismodify = ap.BoolParse("-modify");
    isadd = ap.BoolParse("-add");
    isdelete = ap.BoolParse("-delete");
    if (!ismodify||!isadd||!isdelete) {
    tagid = ap.OneParse("-tagID");
    tagvalue = ap.OneParse("-tagValue");  }





    itk::DicomHeader dicomHeader;

    dicomHeader.ReadDicom(inputdicom);

    if(!isprint) {
        dicomHeader.PrintHeader(slidenum);
    }

    if(!ismodify) {
        dicomHeader.ModifyHeader(tagid, tagvalue);
        dicomHeader.WriteDicom(outputdicom);
    }
    /* return all paras to main class */
//    std::cout<<inputfile[0]<<" "<<inputfile[1]<<" "<<outputfile<<" "
//            <<dim<<imagetype<<std::endl;




  return EXIT_SUCCESS;
}
