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


    unsigned int dim;
    std::vector<unsigned int> dimsize;
    std::string imagetype;


    /* global filenames, levelnumbers, downsample */
    Global var;
    var.ReadFileNames(argv[1]);

    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(var.inputfile1.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    dimsize = io.Return_DimensionSize();

    /* iterate through levels to get each's par */
    unsigned int levelnumber = var.levelnumber;
    std::vector<Parameters> par(levelnumber);

    for (unsigned int i=0; i<levelnumber; i++) {
    par[i].SetInitialParameters();
    par[i].ReadParameters(argv[1],i+1);

    par[i].gridnode = static_cast<unsigned int>(dimsize[0]/par[i].gridspacing[0]);

    std::cout<<var.inputfile1<<std::endl;
    std::cout<<var.inputfile2<<std::endl;
    std::cout<<var.outputfile<<std::endl;
    std::cout<<var.levelnumber<<" "<<par[i].gridspacing<<" "<<par[i].gridnode<<" "<<std::endl;
    std::cout<<var.downsample<<" "<<par[i].maximumiteration<<std::endl;
    std::cout<<par[i].transformtype<<" "<<par[i].metrictype<<" "<<par[i].interpolatortype<<" "<<par[i].optimizertype<<std::endl;
}

    /* Run the filter */
    AssignPointer *filter = NULL;

    if (!filter) filter = itk::BsplineMultiReg::New();


    filter->Initialize(var);
    filter->Update(var, par);
    filter->Write(var);




    return EXIT_SUCCESS;
}
