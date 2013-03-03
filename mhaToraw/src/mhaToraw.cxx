
#include <iostream>
#include <fstream>

using namespace std;



int main (int argc, char* argv[])
{


    string key[] = {"Comment", "ObjectType", "TransformType", "TransformMatrix",
    "BinaryData", "ElementByteOderMSB", "ElementDataByteOderMSB", "Color",
    "Position", "AnatomicalOrientation", "ElementSpacing","HeaderSize",
    "Modality", "SequenceID", "ElementMin", "ElementMax",
    "ElementNumberOfChannels", "ElementSize", "ElementType", "ElementDataFile",
    "OffSet", "CenterOfRotation", "CompressedData", "DimSize"};

    int i,j;
    int nx,ny,nz,nb = 0;
    float sx,sy,sz = 0;
    long bytez = 0;



    if (argc != 3) {
        cout<<"Usage:"<<argv[0]<<"infile"<<" "<<"outfile"<<endl;
        return -1;
    }

    ifstream file1;
    file1.open(argv[1], ios::binary);
    if (!file1) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }


    /* 30 lines of MHA should have passed the header */
    string line;
    for (i=0; i<30; i++) {
        //read one line from fp1 to buf
        getline(file1,line,'\n');

        for(j=0; j<(sizeof(key)/sizeof(char*)); j++) {
            if(line.find(string(key[j]))!=string::npos) {
                cout<<line<<endl;
            }

        }

        /* read in parameters */
        if(line.find("DimSize")!=string::npos) {
            line.erase(0,9);
            sscanf(line.c_str(), "%d%d%d", &nx, &ny, &nz);
        } else if(line.find("ElementSpacing")!=string::npos) {
            line.erase(0,16);
            sscanf(line.c_str(), "%f%f%f", &sx, &sy, &sz);
        } else if(line.find("ElementType")!=string::npos) {
            if(line.find("MET_SHORT")!=string::npos) {
                nb = 2;
            } else if(line.find("MET_USHORT")!=string::npos) {
                nb = 2;
            } else if(line.find("MET_CHAR")!=string::npos) {
                nb = 1;
            } else if(line.find("MET_UCHAR")!=string::npos) {
                nb = 1;
            } else if(line.find("MET_FLOAT")!=string::npos) {
                nb = 4;
            } else if(line.find("MET_DOUBLE")!=string::npos) {
                nb = 8;
            }
        }
        }

    /* write to output raw image */
    ofstream file2;
    file2.open(argv[2], ios::binary);
    if (!file2) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }
    file2.flush();

    // image buffer allocate
    unsigned char* c;
    cout<<"Writing..."<<endl;    
    bytez =(long)(nx * ny * nz * nb);
    cout<<bytez<<"bytes of image data!"<<nx<<" "<<ny<<" "<<nz<<" "<<nb<<endl;

    // locate buffer length backward from end
    file1.seekg(-bytez, ios::end);

    // read 1 byte from inputfile
    file1.read((char*)&c, 1);
    while(!(file1.eof())) {
    // write 1 byte to outputfile
        file2.write(reinterpret_cast<const char*>(&c), 1);

    // read again
        file1.read((char*)&c, 1);
    }



    file1.close();
    file2.close();

}
