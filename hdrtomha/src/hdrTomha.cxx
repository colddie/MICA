#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;



int main (int argc, char* argv[])
{

    if (argc != 3) {
        printf("Usage:hdrTomha infile outfile");
    }

    string dataFilename;
    string imageType;
    string nx,ny,nz;
    string sx,sy,sz;
    string ox,oy,oz;

    /* read the header file */
    ifstream file1;
    file1.open(argv[1]);
    if (!file1) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }

    string line;
    while (!file1.eof()) {
        getline(file1,line,'\n');
        if (line.find("name of data file :=")!=string::npos) {
            line.erase(0,line.find("=")+2);
            dataFilename = line;
        } else if (line.find("number format :=")!=string::npos) {
            if (line.find("char")!=string::npos) {
                imageType = "MET_CHAR";
            } else if (line.find("unsigned char")!=string::npos) {
                imageType = "MET_UCHAR";
            } else if (line.find("short")!=string::npos) {
                imageType = "MET_SHORT";
            } else if (line.find("unsigned short")!=string::npos) {
                imageType = "MET_USHORT";
            } else if (line.find("int")!=string::npos) {
                imageType = "MET_INT";
            } else if (line.find("unsigned int")!=string::npos) {
                imageType = "MET_UINT";
            }  else if (line.find("float")!=string::npos) {
                imageType = "MET_FLOAT";
            }  else if (line.find("double")!=string::npos) {
                imageType = "MET_DOUBLE";
            }

        } else if (line.find("matrix size")!=string::npos) {
            if (line.find("[1]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                nx = line;
            } else if (line.find("[2]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                ny = line;
            } else if (line.find("[3]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                nz = line;
            }
        } else if (line.find("scaling factor (mm/pixel)")!=string::npos) {
            if (line.find("[1]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                sx = line;
            } else if (line.find("[2]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                sy = line;
            } else if (line.find("[3]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                sz = line;
            }
        } else if (line.find("first pixel offset")!=string::npos) {
            if (line.find("[1]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                ox = line;
            } else if (line.find("[2]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                oy = line;
            } else if (line.find("[3]")!=string::npos) {
                line.erase(0,line.find("=")+2);
                oz = line;
            }

        }
    }

    cout<<"data type\t"<<imageType<<endl;
    cout<<"dimension number\t"<<nx<<ny<<nz<<endl;
    cout<<"pixel spacing\t"<<sx<<sy<<sz<<endl;
    cout<<"origin offset\t"<<ox<<oy<<oz<<endl;


    /* write the meta header */
    ofstream file3;
    file3.open(argv[2], ios::binary);
    if (!file3) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }
    file3.flush();

    file3<<"ObjectType = Image\n";
    file3<<"NDims = 3\n"
         <<"BinaryData = True\n"
         <<"BinaryDataByteOrderMSB = False\n"
         <<"TransformMatrix = 1 0 0 0 1 0 0 0 1\n"
         <<"Offset = "<<ox<<" "<<oy<<" "<<oz<<endl
         <<"ElementSpacing = "<<sx<<" "<<sy<<" "<<sz<<endl
         <<"DimSize = "<<nx<<" "<<ny<<" "<<nz<<endl
         <<"AnatomicalOrientation = RPI\n"
         <<"ElementType = "<<imageType<<endl
         <<"ElementDataFile = LOCAL\n";


    /* write the meat binary */
    ifstream file2;
    file2.open(dataFilename.c_str(),ios::binary);

    unsigned char *c;
    // read 1 byte from inputfile
    file2.read((char*)&c, 1);
    while(!(file2.eof())) {
    // write 1 byte to outputfile
        file3.write(reinterpret_cast<const char*>(&c), 1);

    // read again
        file2.read((char*)&c, 1);
    }


    file1.close();
    file2.close();
    file3.close();




}
