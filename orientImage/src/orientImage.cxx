#include "orientImage.h"

//#include <vector>


// IOFactory already support multi format, add here to enable multi type
// see IO_type class

/* Reorientate the image. Possible solutions:
permuteAxiesImageFilter;
orientImageFilter;
versor and changeImageInformationFilter;
rigidTransform and resampleImageFilter;
affineTransform and resampleImageFilter;
eulerTransform and resampleImageFiltger.
*/



void CreateOrientMaps(std::map<std::string,itk::SpatialOrientation::ValidCoordinateOrientationFlags> &orientationMap)
{
      orientationMap["Axial"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI;
      orientationMap["Coronal"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA;
      orientationMap["Sagittal"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL;
      orientationMap["RIP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP;
      orientationMap["LIP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIP;
      orientationMap["RSP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP;
      orientationMap["LSP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSP;
      orientationMap["RIA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIA;
      orientationMap["LIA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIA;
      orientationMap["RSA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA;
      orientationMap["LSA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSA;
      orientationMap["IRP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRP;
      orientationMap["ILP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILP;
      orientationMap["SRP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRP;
      orientationMap["SLP"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLP;
      orientationMap["IRA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRA;
      orientationMap["ILA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILA;
      orientationMap["SRA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRA;
      orientationMap["SLA"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLA;
      orientationMap["RPI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPI;
      orientationMap["LPI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI;
      orientationMap["RAI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI;
      orientationMap["LAI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAI;
      orientationMap["RPS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPS;
      orientationMap["LPS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPS;
      orientationMap["RAS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAS;
      orientationMap["LAS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAS;
      orientationMap["PRI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRI;
      orientationMap["PLI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLI;
      orientationMap["ARI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARI;
      orientationMap["ALI"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALI;
      orientationMap["PRS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRS;
      orientationMap["PLS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLS;
      orientationMap["ARS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARS;
      orientationMap["ALS"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALS;
      orientationMap["IPR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPR;
      orientationMap["SPR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPR;
      orientationMap["IAR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAR;
      orientationMap["SAR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAR;
      orientationMap["IPL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPL;
      orientationMap["SPL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPL;
      orientationMap["IAL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAL;
      orientationMap["SAL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAL;
      orientationMap["PIR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIR;
      orientationMap["PSR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSR;
      orientationMap["AIR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR;
      orientationMap["ASR"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASR;
      orientationMap["PIL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIL;
      orientationMap["PSL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSL;
      orientationMap["AIL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIL;
      orientationMap["ASL"] = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL;



}

int main(int argc, char** argv)
{


    const char *help[] = {
        "Usage: orientImage -input inputfile -output outputfile [-old] givenDirection -new desiredDirection\n"
        "Options:\n"
        "-input         Name of the input file\n"
        "-output        Name of the output file\n"
        "[-old]         Given direction\n"
        "-new           New direction"
    };


    std::string inputfile;
    std::string outputfile;
    bool isoldgiven;
    std::string givendirection;
    std::string desireddirection;
    unsigned int dim;
    std::string imagetype;



    /* return all paras to main class */
    ArgumentParser ap;
    ap.SetCommandLineArg(argc, argv, help);

    ap.MarkAsRequired("-input");
    ap.MarkAsRequired("-output");
    ap.MarkAsRequired("-new");
    if (!ap.IsRequiredFound()) { exit(1); }

    inputfile = ap.OneParse("-input");
    outputfile = ap.OneParse("-output");
    isoldgiven = ap.BoolParse("-old");
    if (isoldgiven) { givendirection = ap.OneParse("-old"); }
    desireddirection = ap.OneParse("-new");

    std::map<string, itk::SpatialOrientation::ValidCoordinateOrientationFlags> orientationMap;
    CreateOrientMaps(orientationMap);
    std::map<std::string, itk::SpatialOrientation::ValidCoordinateOrientationFlags>::iterator in = orientationMap.find(givendirection);
    std::map<std::string, itk::SpatialOrientation::ValidCoordinateOrientationFlags>::iterator out = orientationMap.find(desireddirection);
    itk::SpatialOrientation::ValidCoordinateOrientationFlags in_direction = in->second;
    itk::SpatialOrientation::ValidCoordinateOrientationFlags out_direction = out->second;


    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(inputfile.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    //    io.main_fun(inputfile, imagetype, dim, channel);





    /* return all paras to main class */
    std::cout<<inputfile<<outputfile<<" "<<dim<<imagetype<<" "<<givendirection<<desireddirection<<std::endl;

    itk::OrientImage::Pointer filter = itk::OrientImage::New();

    filter->m_inputfile = inputfile;
    filter->m_outputfile = outputfile;
    filter->m_isoldgiven = isoldgiven;
    if (isoldgiven) { filter->m_indirection = in_direction; }
    filter->m_outdirection = out_direction;




    // update with image type
    if(dim == 3) {
           if(imagetype == "CHAR") {filter->Update<char, 3>();}
           if(imagetype == "UCHAR") {filter->Update<unsigned char, 3>();}
           if(imagetype == "SHORT") {filter->Update<short, 3>();}
           if(imagetype == "USHORT") {filter->Update<unsigned short, 3>();}
           if(imagetype == "INT") {filter->Update<int, 3>();}
           if(imagetype == "UINT") {filter->Update<unsigned int, 3>();}
           if(imagetype == "FLOAT") {filter->Update<float, 3>();}
           if(imagetype == "DOUBLE") {filter->Update<double, 3>();}
    }

    //    if(dim == 2) {
    //           if(imagetype == "CHAR") {filter->Update<char, 2>();}
    //           if(imagetype == "UCHAR") {filter->Update<unsigned char, 2>();}
    //           if(imagetype == "SHORT") {filter->Update<short, 2>();}
    //           if(imagetype == "USHORT") {filter->Update<unsigned short, 2>();}
    //           if(imagetype == "INT") {filter->Update<int, 2>();}
    //           if(imagetype == "UINT") {filter->Update<unsigned int, 2>();}
    //           if(imagetype == "FLOAT") {filter->Update<float, 2>();}
    //           if(imagetype == "DOUBLE") {filter->Update<double, 2>();}
    //    }


    //     if(dim == 4) {
    //       /* only 3 channel float supported */
    ////       const unsigned int VectorDimension = channel;
    //       const unsigned int VectorDimension = 3;
    //       const unsigned int ImageDimension = 3;
    //       typedef itk::Vector< float, VectorDimension >    PixelType;
    //       typedef itk::Image< PixelType, ImageDimension > ImageType;
    //       filter->Update<ImageType>();

    //     }


     //    filter->Run();



  return EXIT_SUCCESS;
}

