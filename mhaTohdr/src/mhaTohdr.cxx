
#include <iostream>
#include <fstream>

using namespace std;



int main (int argc, char* argv[])
{


    //    string key[] = {"Comment", "ObjectType", "TransformType", "TransformMatrix",
    //    "BinaryData", "ElementByteOderMSB", "ElementDataByteOderMSB", "Color",
    //    "Position", "AnatomicalOrientation", "ElementSpacing","HeaderSize",
    //    "Modality", "SequenceID", "ElementMin", "ElementMax",
    //    "ElementNumberOfChannels", "ElementSize", "ElementType", "ElementDataFile",
    //    "Offset", "CenterOfRotation", "CompressedData", "DimSize"};



    if (argc != 3) {
        cout<<"Usage:"<<argv[0]<<"infile"<<" "<<"outfile"<<endl;
        return -1;
    }

    int nx,ny,nz,nb = 0;
    float sx,sy,sz,ox,oy,oz = 0;
    long bytez = 0;

    ifstream file1;
    file1.open(argv[1], ios::binary);
    if (!file1) {
        cout<<"Error opening file"<<argv[1]<<"for read"<<endl;
        return -1;
    }


    /* 30 lines of MHA should have passed the header */
    string line;
    string type = "";
    for (int i=0; i<30; i++) {
        //read one line from fp1 to buf
        getline(file1,line,'\n');

        //        for(j=0; j<(sizeof(key)/sizeof(char*)); j++) {
        //            if(line.find(string(key[j]))!=string::npos) {
        //                cout<<line<<endl;
        //            }

        //        }

        /* read in parameters */        
        if(line.find("DimSize")!=string::npos) {
            line.erase(0,9);
            sscanf(line.c_str(), "%d%d%d", &nx, &ny, &nz);
        } else if(line.find("ElementSpacing")!=string::npos) {
            line.erase(0,16);
            sscanf(line.c_str(), "%f%f%f", &sx, &sy, &sz);
        } else if(line.find("Offset")!=string::npos) {
            line.erase(0,8);
            sscanf(line.c_str(), "%f%f%f", &ox, &oy, &oz);
        } else if(line.find("ElementType")!=string::npos) {
            if(line.find("MET_SHORT")!=string::npos) {
                nb = 2;
                type = "short";
            } else if(line.find("MET_USHORT")!=string::npos) {
                nb = 2;
                type = "unsigned short";
            } else if(line.find("MET_CHAR")!=string::npos) {
                nb = 1;
                type = "char";
            } else if(line.find("MET_UCHAR")!=string::npos) {
                nb = 1;
                type = "unsigned char";
            } else if(line.find("MET_FLOAT")!=string::npos) {
                nb = 4;
                type = "float";
            } else if(line.find("MET_DOUBLE")!=string::npos) {
                nb = 8;
                type = "double";
            }
        }
    }


    /* write to output raw image */
    ofstream file2;
    string headerFilename = string(argv[2]);
    string outputPrefix = headerFilename.substr(0,headerFilename.find_first_of("."));
    string dataFilename = outputPrefix + string(".img");
    file2.open(dataFilename.c_str(), ios::binary);
    file2.flush();

    unsigned char* c;
    cout<<"Writing..."<<endl;
    // image buffer allocate
    bytez =(long)(nx * ny * nz * nb);
    cout<<bytez<<"bytes of image data!"<<nx<<" "<<ny<<" "<<nz<<" "<<nb<<endl;
    // locate buffer length backward from end
    file1.seekg(-bytez, ios::end);
    // read 1 byte from inputfile

    file1.read((char*)&c, nb);
    while(!(file1.eof())) {
    // write nb byte to outputfile
        file2.write(reinterpret_cast<const char*>(&c), nb);

    // read again
        file1.read((char*)&c, nb);
    }

    /* write to output header file */
    ofstream file3;
    // string headerFilename = outputprefix + string(".hdr");
    file3.open(headerFilename.c_str());
    file3.flush();

    file3 << "!INTERFILE  := " << endl;
    file3 << "name of data file := " << outputPrefix << ".img " << endl;
    file3 << "!GENERAL DATA :=  " << endl;
    file3 << "!GENERAL IMAGE DATA := " << endl;
    file3 << "!type of data := PET " << endl;
    file3 << "imagedata byte order := LITTLEENDIAN " << endl;
    file3 << "!PET STUDY (General) :=  " << endl;
    file3 << "!PET data type := Image " << endl;
    file3 << "process status := Reconstructed " << endl;
    file3 << "!number format := " << type << endl;
    file3 << "!number of bytes per pixel := " << nb << endl;
    file3 << "number of dimensions := 3 " << endl;
    file3 << "matrix axis label [1] := x " << endl;
    file3 << "!matrix size [1] := " << nx << endl;
    file3 << "scaling factor (mm/pixel) [1] := " << sx << endl;
    file3 << "matrix axis label [2] := y " << endl;
    file3 << "!matrix size [2] := " << ny << endl;
    file3 << "scaling factor (mm/pixel) [2] := " << sy << endl;
    file3 << "matrix axis label [3] := z " << endl;
    file3 << "!matrix size [3] := " << nz << endl;
    file3 << "scaling factor (mm/pixel) [3] := " << sz << endl;
    file3 << "first pixel offset (mm) [1] := " << ox << endl;
    file3 << "first pixel offset (mm) [2] := " << oy << endl;
    file3 << "first pixel offset (mm) [3] := " << oz << endl;
    file3 << "number of time frames := 1 " << endl;
    file3 << "image scaling factor[1] := 1 " << endl;
    file3 << "data offset in bytes[1] := 0 " << endl;
    file3 << "quantification units := 1 " << endl;
    file3 << "!END OF INTERFILE :=" <<endl;



    file1.close();
    file2.close();
    file3.close();


}


