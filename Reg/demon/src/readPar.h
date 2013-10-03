#ifndef _readPar_H_
#define _readPar_H_


#include <vector>
#include <string>



class Par {

public:

    // Must specified
    std::string inputfile1;
    std::string inputfile2;
    std::string outputfile;
    std::string outputdeformfile;

    unsigned int histogramlevel;
    unsigned int levelnumber;
    unsigned int matchnumber;
    double       demonstd;
    std::vector<unsigned int> maxiteration;





    void ReadFileNames(const char *parfile);





    Par(){};
    ~Par(){};



};













#endif
