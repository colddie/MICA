#include "readStages.h"
#include "toolBase.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//void readParameters(char *argv[], unsigned int &levelnumber,
//string &inputfile1, string &inputfile2, string &outputfile, string &outputdeformfile,
//vector<unsigned int> &gridspacing, vector<unsigned int> &gridnode,
//vector<unsigned int> &downsample, vector<unsigned int> &maximumiteration,
//vector<Transform_Type> &transformtype, vector<Metric_Type> &metrictype,
//vector<InterPolator_Type> &interpolatortype, vector<Optimizer_Type> &optimizertype)

void Parameters::SetInitialParameters()
{

    // Assume GLOBAL already given
    //general
    transformtype = TRANSFORM_VERSOR3D;
    metrictype = METRIC_MSE;
    interpolatortype = INTERPOLATOR_LINEAR;
    optimizertype = OPTIMIZER_RSGD;

    maximumiteration = 25;


    //metric
    fixedimagestandarddeviation = 0.4;
    movingimagestandarddeviation = 0.4;

    numberofhistogrambins = 20;
    numberofspatialsamples = 10000;

    //optimizer
    maximumsteplength = 0.5;
    minimumsteplength = 10;

    learningrate = 0.01;

    parametersconvergencetolerance = 0.05;
    functionconvergencetolerance = 10000;

    relaxationfactor = 0.5;

    gradientconvergencetolerance = 0.1;
    linesearchaccurancy = 0.9;
    defaultsteplength = 1.5;
    maximumnumberoffunctionevaluations = 1000;

    costfunctionconvergencefactor = 1e+7;
    projectedgradienttolerance = 1e-4;
    maximumnumberofcorrections = 12;


    //bspline
    gridspacing.push_back(20);
    gridspacing.push_back(20);
    gridspacing.push_back(20);

    gridnode = 6;

}


//void Parameters::CheckParameters()
//{
//}


void Parameters::ReadParameters(const char *parfile, const unsigned int &stage)
{

    //unsigned int count = 0;

    ifstream file1;
    file1.open(parfile, ios::binary);
    if (!file1) {
        cout<<"Error opening file"<<parfile<<"for read"<<endl;
        exit(1);
    }

    std::istreambuf_iterator<char> eos;
    std::string s(std::istreambuf_iterator<char>(file1), eos);
//    string::iterator iter = s.begin();
//    string::iterator iter_next = s.end();
    unsigned int block_begin = 0;
    unsigned int block_end = string::npos;

    for (unsigned int i=0; i<stage; i++) {
        if(s.find("#STAGE", block_begin)!=string::npos) {
//            iter = std::find(iter, s.end(),'#STAGE');
        block_begin = s.find("#STAGE", block_begin+1);
        }
    }


    if(s.find("#STAGE", block_begin)!=string::npos) {
//    iter_next = std::find(iter, s.end(), '#STAGE');
    block_end = s.find("#STAGE", block_begin+1);
    }


    string block = s.substr(block_begin, block_end);
    //        string block = string(iter, iter_next);

    // has to initialized this way
    stringstream ss;
    ss.str(block);
    //cout<<block;

    string line;
    unsigned int pos;
    unsigned int sx,sy,sz;

    for (unsigned int i=0; i<10; i++) {
        getline(ss, line);
        if(line.find("transform")!=string::npos) {
            if(line.find("translation")!=string::npos) {
                transformtype = TRANSFORM_TRANSLATION;
            }
            if(line.find("rigid")!=string::npos) {
                transformtype = TRANSFORM_VERSOR3D;
            }
            if(line.find("quaternion")!=string::npos) {
                transformtype = TRANSFORM_QUATERNION;
            }
            if(line.find("affine")!=string::npos) {
                transformtype = TRANSFORM_AFFINE;
            }
            if(line.find("bspline")!=string::npos) {
                transformtype = TRANSFORM_BSPLINE;
            }
         }

        if(line.find("metric")!=string::npos) {
            if(line.find("mse")!=string::npos) {
                metrictype = METRIC_MSE;
            }
            if(line.find("mi")!=string::npos) {
                metrictype = METRIC_MI;
            }
            if(line.find("mattes")!=string::npos) {
                metrictype = METRIC_MATTESMI;
            }
         }

        if(line.find("interpolator")!=string::npos) {
            if(line.find("linear")!=string::npos) {
                interpolatortype = INTERPOLATOR_LINEAR;
            }
            if(line.find("nearest")!=string::npos) {
                interpolatortype = INTERPOLATOR_NEARESTNEIGHBOUR;
            }
            if(line.find("bspline")!=string::npos) {
                interpolatortype = INTERPOLATOR_BSPLINEINTER;
            }
         }

        if(line.find("optimizer")!=string::npos) {
            if(line.find("versor")!=string::npos) {
                optimizertype = OPTIMIZER_VERSOR3D;
            }
            if(line.find("quaternion")!=string::npos) {
                optimizertype = OPTIMIZER_QUATERNION;
            }
            if(line.find("amoeba")!=string::npos) {
                optimizertype = OPTIMIZER_AMOEBA;
            }
            if(line.find("rsgd")!=string::npos) {
                optimizertype = OPTIMIZER_RSGD;
            }
            if(line.find("lbfgsb")!=string::npos) {
                optimizertype = OPTIMIZER_LBFGSB;
            } else if(line.find("lbfgs")!=string::npos){
                    optimizertype = OPTIMIZER_LBFGS;
            }
         }

        if(line.find("max_iter")!=string::npos) {
            pos = line.find("=");
            line.erase(0,pos+1);
            line = trim(line, " \t");
            maximumiteration = atoi(line.c_str());
         }

        if(line.find("grid_spacing")!=string::npos) {
            pos = line.find("=");
            line.erase(0,pos+1);
            line = trim(line, " \t");
            sscanf(line.c_str(), "%d%d%d", &sx, &sy, &sz);

            gridspacing.clear();
            gridspacing.push_back(sx);
            gridspacing.push_back(sy);
            gridspacing.push_back(sz);
        }

    }
}




