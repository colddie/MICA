#include <iostream>
#include <fstream>


using namespace std;

int main(int argc, char *argv[])
{


    if (argc != 3) {
        printf("Usage:mhaToim infile outfile");
        return -1;
    }

    int TYPE_OFFSET = 8321;
    int DIMENSIONNUM_OFFSET = 8325;
    int DIMENSIONSIZE_OFFSET = 8329;
    int PIXELS_OFFSET = 8369;
    int TITLE_OFFSET= 36;
    int MAXMIN_OFFSET = 117;
    int HISTO_OFFSET = 129;
    int DIM = 3;
    int FILL = 1;
    int EMPTY =0;
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
        // read in parameters
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
            } else if(line.find("MET_FLOAT")!=string::npos) {
                nb = 4;
                type = "float";
            }
            else {
                printf("Sorry,.im file only support float data");
                return -1;
            }
        }
    }


    cout<<"image size"<<nx*ny*nz*nb<<endl;
    cout<<"image type"<<type<<endl;
    cout<<"image dimension"<<"3"<<endl;
    cout<<"image dimension size"<<nx<<" "<<ny<<" "<<nz<<endl;


    /* write to the im header */
    ofstream file2;
    file2.open(argv[2], ios::binary);
    if (!file2) {
        cout<<"Error opening file"<<argv[2]<<"for read"<<endl;
        return -1;
    }
    file2.flush();

    file2.write((char*)&MAXMIN_OFFSET, sizeof(int));
    file2.write((char*)&HISTO_OFFSET, sizeof(int));
    file2.write((char*)&TITLE_OFFSET, sizeof(int));
    file2.write((char*)&TYPE_OFFSET, sizeof(int));
    file2.write((char*)&(DIMENSIONNUM_OFFSET), sizeof(int));
    file2.write((char*)&(DIMENSIONSIZE_OFFSET), sizeof(int));
    file2.write((char*)&PIXELS_OFFSET, sizeof(int));
    int DATA_OFFSET = PIXELS_OFFSET+nx*ny*nz*nb;
    file2.write((char*)&DATA_OFFSET, sizeof(int));
    file2.write((char*)&FILL, sizeof(int));
    for(int i=0; i<TYPE_OFFSET-TITLE_OFFSET; i++) {
        file2.write((char*)&EMPTY, sizeof(int));
    }

    file2.seekp(TYPE_OFFSET, ios::beg);
    file2.write((char*)&nb, sizeof(int));
    file2.write((char*)&DIM, sizeof(int));
    file2.write((char*)&nz, sizeof(int));
    file2.write((char*)&nx, sizeof(int));
    file2.write((char*)&ny, sizeof(int));
    for(int i=0; i<28; i++) {
        file2.write((char*)&EMPTY, sizeof(int));
    }

    /* write the data */
    cout<<"Writing..."<<endl;
    bytez =(long)(nx * ny * nz * nb);
    cout<<bytez<<"bytes of image data!"<<endl;
    // locate buffer length backward from end
    file1.seekg(-bytez, ios::end);
    file2.seekp(PIXELS_OFFSET, ios::beg);
    unsigned char *c;
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
