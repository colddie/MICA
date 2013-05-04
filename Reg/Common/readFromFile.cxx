#include "readFromFile.h"
#include "toolBase.h"

#include <iostream>
#include <fstream>


using namespace std;

//void readParameters(char *argv[], unsigned int &levelnumber,
//string &inputfile1, string &inputfile2, string &outputfile, string &outputdeformfile,
//vector<unsigned int> &gridspacing, vector<unsigned int> &gridnode,
//vector<unsigned int> &downsample, vector<unsigned int> &maximumiteration,
//vector<Transform_Type> &transformtype, vector<Metric_Type> &metrictype,
//vector<InterPolator_Type> &interpolatortype, vector<Optimizer_Type> &optimizertype)

void Parameters::SetInitialParameters()
{

}


void Parameters::CheckParameters()
{
//    std::cout<<gridspacing<<downsample;
//    for (unsigned int i=0; i<levelnumber; i++) {
//        // default gridspacing
//        if (gridspacing[i*3]==0) {
//            gridspacing[i*3]=20;
//            gridspacing[i*3+1]=20;
//            gridspacing[i*3+2]=20;
//        }
//        // default gridnode
//        if (gridnode[i]==0) {
//            gridnode[i]=8;
//        }
//        // default downsample
//        if (downsample[i*3]==0) {
//            downsample[i*3]=1;
//            downsample[i*3+1]=1;
//            downsample[i*3+2]=1;
//        }

//    }







}


void Parameters::ReadParameters(const char *parfile)
{

string line;
unsigned int pos;
unsigned int sx,sy,sz;

ifstream file1;
file1.open(parfile, ios::binary);
if (!file1) {
    cout<<"Error opening file"<<parfile<<"for read"<<endl;
    exit(1);
}

for (unsigned int i=0; i<100; i++) {
    //read one line from fp1 to buf
    getline(file1,line,'\n');

    if(line.find("fixed_image")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        line = trim(line,"\r");
        inputfile1 = line; continue;
     }

    if(line.find("moving_image")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        line = trim(line,"\r");
        inputfile2 = line; continue;
     }

    if(line.find("warped_image")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        line = trim(line,"\r");
        outputfile = line; continue;
     }

    if(line.find("deformation_field")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        line = trim(line,"\r");
        outputdeformfile = line; continue;
     }

    if(line.find("level_number")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        levelnumber = atoi(line.c_str());
        gridspacing.reserve(levelnumber*3);
        gridnode.reserve(levelnumber);
        downsample.reserve(levelnumber*3);
        maximumiteration.reserve(levelnumber);
        metrictype.reserve(levelnumber);
        interpolatortype.reserve(levelnumber);
        optimizertype.reserve(levelnumber); continue;
     }

    if(line.find("transform")!=string::npos) {
        if(line.find("translation")!=string::npos) {
            transformtype.push_back(TRANSFORM_TRANSLATION); continue;
        }
        if(line.find("rigid")!=string::npos) {
            transformtype.push_back(TRANSFORM_VERSOR3D); continue;
        }
        if(line.find("quaternion")!=string::npos) {
            transformtype.push_back(TRANSFORM_QUATERNION); continue;
        }
        if(line.find("affine")!=string::npos) {
            transformtype.push_back(TRANSFORM_AFFINE); continue;
        }
        if(line.find("bspline")!=string::npos) {
            transformtype.push_back(TRANSFORM_BSPLINE); continue;
        }
     }

    if(line.find("metric")!=string::npos) {
        if(line.find("mse")!=string::npos) {
            metrictype.push_back(METRIC_MSE); continue;
        }
        if(line.find("mi")!=string::npos) {
            metrictype.push_back(METRIC_MI); continue;
        }
        if(line.find("mattes")!=string::npos) {
            metrictype.push_back(METRIC_MATTESMI); continue;
        }
     }

    if(line.find("interpolator")!=string::npos) {
        if(line.find("linear")!=string::npos) {
            interpolatortype.push_back(INTERPOLATOR_LINEAR); continue;
        }
        if(line.find("nearest")!=string::npos) {
            interpolatortype.push_back(INTERPOLATOR_NEARESTNEIGHBOUR); continue;
        }
        if(line.find("bspline")!=string::npos) {
            interpolatortype.push_back(INTERPOLATOR_BSPLINEINTER); continue;
        }
     }

    if(line.find("optimizer")!=string::npos) {
        if(line.find("versor")!=string::npos) {
            optimizertype.push_back(OPTIMIZER_VERSOR3D); continue;
        }
        if(line.find("quaternion")!=string::npos) {
            optimizertype.push_back(OPTIMIZER_QUATERNION); continue;
        }
        if(line.find("amoeba")!=string::npos) {
            optimizertype.push_back(OPTIMIZER_AMOEBA); continue;
        }
        if(line.find("rsgd")!=string::npos) {
            optimizertype.push_back(OPTIMIZER_RSGD); continue;
        }
        if(line.find("lbfgsb")!=string::npos) {
            optimizertype.push_back(OPTIMIZER_LBFGSB); continue;
        } else if(line.find("lbfgs")!=string::npos){
                optimizertype.push_back(OPTIMIZER_LBFGS); continue;
        }
     }

    if(line.find("down_sample")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        sscanf(line.c_str(), "%d%d%d", &sx, &sy, &sz);
        downsample.push_back(sx);
        downsample.push_back(sy);
        downsample.push_back(sz); continue;
     }

    if(line.find("max_iter")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        maximumiteration.push_back(atoi(line.c_str())); continue;
     }

    if(line.find("grid_spacing")!=string::npos) {
        pos = line.find("=");
        line.erase(0,pos+1);
        line = trim(line, " \t");
        sscanf(line.c_str(), "%d%d%d", &sx, &sy, &sz);
        gridspacing.push_back(sx);
        gridspacing.push_back(sy);
        gridspacing.push_back(sz); continue;
    }

}

}
