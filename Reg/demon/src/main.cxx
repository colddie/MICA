#include "demonReg.h"

#include "readPar.h"
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
    Par par;
    par.ReadFileNames(argv[1]);

    /* parse inputfile name to find the component type */
    IO_type io;
    io.Create_IO(par.inputfile1.c_str());
    imagetype = io.Return_ImageType();
    dim = io.Return_DimensionNumber();
    dimsize = io.Return_DimensionSize();





    std::cout<<par.inputfile1<<std::endl;
    std::cout<<par.inputfile2<<std::endl;
    std::cout<<par.outputfile<<std::endl;
    std::cout<<par.outputdeformfile<<std::endl;
    std::cout<<par.levelnumber<<std::endl;
    std::cout<<par.maxiteration<<std::endl;

    /* Run the filter */
    //DemonReg *filter = NULL;

    itk::DemonReg *filter = itk::DemonReg::New();

    filter->Run(par);
//    filter->Initialize(var);
//    filter->Update(var, par);
//    filter->Write(var);




    return EXIT_SUCCESS;
}
