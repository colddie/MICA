#ifndef _readFileNames_H_
#define _readFileNames_H_


#include <vector>
#include <string>



class Global {

public:

    // Must specified
    std::string inputfile1;
    std::string inputfile2;
    std::string outputfile;
    std::string outputdeformfile;

    unsigned int levelnumber;
    std::vector<unsigned int> downsample;





    void ReadFileNames(const char *parfile);





    Global(){};
    ~Global(){};



};













#endif
