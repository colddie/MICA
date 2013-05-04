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

    // Optional, given default
    unsigned int maximumiteration;
    Transform_Type transformtype;
    Metric_Type metrictype;
    InterPolator_Type interpolatortype;
    Optimizer_Type optimizertype;

    // Metric
    double fixedimagestandarddeviation;
    double movingimagestandarddeviation;
    unsigned int numberofhistogrambins;
    unsigned int numberofspatialsamples;

    // Optimizer
    double maximumsteplength;
    double minimumsteplength;

    double learningrate;

    double parametersconvergencetolerance;
    double functionconvergencetolerance;

    double relaxationfactor;

    double gradientconvergencetolerance;
    double linesearchaccurancy;
    double defaultsteplength;
    double maximumnumberoffunctionevaluations;

    double costfunctionconvergencefactor;
    double projectedgradienttolerance;
    unsigned int maximumnumberofcorrections;



    // Bspline
    std::vector<unsigned int> gridspacing;
    unsigned int gridnode;

    // Demons



    // Public interface
    void SetInitialParameters();
    void ReadParameters(const char *parfile, const unsigned int &stage);


    Parameters(){};
    ~Parameters(){};



private:


};











#endif
