#include "bsplineMultiReg.h"

//#include "readStages.h"
//#include "readGlobal.h"
#include "toolBase.h"

//#include "argumentsParse.h"
#include "IO_type.h"


/* ITK throws exceptions when e.g. evaluating metrics with overlap
   of less that 25%.  Identify these so we can continue processing. */

int main(int argc, char** argv)
{


//    std::string inputfile1;
//    std::string inputfile2;
//    std::string outputfile;
//    std::string outputdeformfile;
    unsigned int dim;
    std::vector<unsigned int> dimsize;
    std::string imagetype;

//    unsigned int levelnumber;
//    std::vector<unsigned int> gridspacing;
//    std::vector<unsigned int> gridnode;
//    std::vector<unsigned int> downsample;
//    std::vector<unsigned int> maximumiteration;
//    std::vector<Transform_Type> transformtype;
//    std::vector<Metric_Type> metrictype;
//    std::vector<InterPolator_Type> interpolatortype;
//    std::vector<Optimizer_Type> optimizertype;

//    readParameters(argv, levelnumber,
//    inputfile1, inputfile2, outputfile, outputdeformfile,
//    gridspacing, gridnode,
//    downsample, maximumiteration,
//    transformtype, metrictype, interpolatortype,
//    optimizertype);


    //
    Global var;
    var.ReadFileNames(argv[1]);

    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(var.inputfile1.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    dimsize = io.Return_DimensionSize();

    //
    unsigned int levelnumber = var.levelnumber;

    std::vector<Parameters> par(levelnumber);

    for (unsigned int i=0; i<levelnumber; i++) {

//    par.SetInitialParameters();
    par[i].ReadParameters(argv[1],i+1);
    par[i].CheckParameters();

    par[i].gridnode = static_cast<unsigned int>(dimsize[0]/par[i].gridspacing[0]);

    std::cout<<var.inputfile1<<std::endl;
    std::cout<<var.inputfile2<<std::endl;
    std::cout<<var.outputfile<<std::endl;
    std::cout<<var.levelnumber<<" "<<par[i].gridspacing<<" "<<par[i].gridnode<<" "<<std::endl;
    std::cout<<var.downsample<<" "<<par[i].maximumiteration<<std::endl;
    std::cout<<par[i].transformtype<<" "<<par[i].metrictype<<" "<<par[i].interpolatortype<<" "<<par[i].optimizertype<<std::endl;
}

    AssignPointer *filter = NULL;

    /* return all paras to main class */
    if (!filter) filter = itk::BsplineMultiReg::New();


//    filter->m_par = par;
//    filter->m_fixedFile = par.inputfile1;
//    filter->m_movingFile = par.inputfile2;
//    filter->m_outputFile = par.outputfile;
//    filter->m_outputDeformFile = par.outputdeformfile;
//    filter->m_levelNumber = par.levelnumber;
//    filter->m_gridNode = par.gridnode;
//    filter->m_downSample = par.downsample;
//    filter->m_maxIteration = par.maximumiteration;
//    filter->m_transformType = par.transformtype;
//    filter->m_metricType = par.metrictype;
//    filter->m_interpolatorType = par.interpolatortype;
//    filter->m_optimizerType = par.optimizertype;

    // update with image type
    //    if(dim == 3) {

    filter->Initialize(var);
    filter->Update(var, par);
    filter->Write(var);
    //       if(imagetype == "CHAR") {  filter->Update<char, 3>();}
    //       else if(imagetype == "UNSIGNED_CHAR") { filter->Update<unsigned char, 3>();}
    //       else if(imagetype == "SHORT") { filter->Update<short, 3>();}
    //       else if(imagetype == "UNSIGNED_SHORT") { filter->Update<unsigned short, 3>();}
    //       else if(imagetype == "INT") { filter->Update<int, 3>();}
    //       else if(imagetype == "UNSIGNED_INT") { filter->Update<unsigned int, 3>();}
    //       else if(imagetype == "FLOAT") { filter->Update<float, 3>();}
    //       else if(imagetype == "DOUBLE") { filter->Update<double, 3>();}
    //            }

    //     if(dim == 2) {
    //       if(imagetype == "CHAR") { filter->Update<char, 2>();}
    //       else if(imagetype == "UNSIGNED_CHAR") { filter->Update<unsigned char, 2>();}
    //       else if(imagetype == "SHORT") { filter->Update<short, 2>();}
    //       else if(imagetype == "UNSIGNED_SHORT") { filter->Update<unsigned short, 2>();}
    //       else if(imagetype == "INT") { filter->Update<int, 2>();}
    //       else if(imagetype == "UNSIGNED_INT") { filter->Update<unsigned int, 2>();}
    //       else if(imagetype == "FLOAT") { filter->Update<float, 2>();}
    //       else if(imagetype == "DOUBLE") { filter->Update<double, 2>();}
    //                }

     //     if(dim == 4) {
     //       /* only 3 channel float supported */
     //         //       const unsigned int VectorDimension = channel;
     //         //       const unsigned int VectorDimension = 3;
     //         const unsigned int ImageDimension = 3;
     //         //       typedef itk::Vector< float, VectorDimension >   PixelType;
     //         //       typedef itk::Image< PixelType, ImageDimension > ImageType;
     //         //       typedef itk::VectorImage<float, ImageDimension> ImageType;

     //         filter->Update<float, ImageDimension>();
     //     }


     //    filter->Run();




  return EXIT_SUCCESS;
}
