#ifndef _readFromFile_H_
#define _readFromFile_H_

#include "enumerationType.h"

#include <vector>
#include <string>





//void readParameters(char *argv[], unsigned int &levelnumber,
//std::string &inputfile1, std::string &inputfile2, std::string &outputfile, std::string &outputdeformfile,
//std::vector<unsigned int> &gridspacing, std::vector<unsigned int> &gridnode,
//std::vector<unsigned int> &downsample, std::vector<unsigned int> &maximumiteration,
//std::vector<Transform_Type> &transformtype, std::vector<Metric_Type> &metrictype,
//std::vector<InterPolator_Type> &interpolatortype, std::vector<Optimizer_Type> &optimizertype);



class Parameters {


public:

//    // Must specified
//    std::string inputfile1;
//    std::string inputfile2;
//    std::string outputfile;
//    std::string outputdeformfile;

//    unsigned int levelnumber;
//    std::vector<unsigned int> maximumiteration;
//    std::vector<Transform_Type> transformtype;
//    std::vector<Metric_Type> metrictype;
//    std::vector<InterPolator_Type> interpolatortype;
//    std::vector<Optimizer_Type> optimizertype;
    unsigned int maximumiteration;
    Transform_Type transformtype;
    Metric_Type metrictype;
    InterPolator_Type interpolatortype;
    Optimizer_Type optimizertype;

    // Optional, given default
    std::vector<unsigned int> gridspacing;
//    std::vector<unsigned int> gridnode;
    unsigned int gridnode;
//    std::vector<unsigned int> downsample;


    void SetInitialParameters();
    void ReadParameters(const char *parfile, const unsigned int &stage);
//    void ReadFileName(const std::string &line, const std::string &label);
//    void ReadLevelNumber(const std::string &line);
//    void ReadSingleNumberATime(const std::string &line, const std::string &label);
//    void ReadTribleNumberATime(const std::string &line, const std::string &label);
    void CheckParameters();

    Parameters(){};
    ~Parameters(){};



private:


};














#endif
