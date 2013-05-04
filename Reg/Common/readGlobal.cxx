#include "readGlobal.h"

#include "toolBase.h"

#include <iostream>
#include <fstream>



using namespace std;




void Global::ReadFileNames(const char *parfile)
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

    for (unsigned int i=0; i<60; i++) {
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
            downsample.reserve(levelnumber*3); continue;
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

    }

}
