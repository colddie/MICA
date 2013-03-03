#ifndef _IO_type_h_
#define _IO_type_h_

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaImageIO.h"

#include <string.h>

class IO_type
{

public:

//    enum IOtype {
//        type_UCHAR,
//        type_SHORT,
//        type_USHORT,
//        type_INT,
//        type_UINT,
//        type_FLOAT,
//        type_DOUBLE
//    };

//    int type_fun(const char *b[]);
//    unsigned int dim_fun(int a, char *b[]);
//    int main_fun(const char *a, const char* &type, unsigned int &dim, itk::ImageIOBase::IOComponentType &componentType);
    int main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel);

    IO_type(){};
    ~IO_type(){};

protected:

private:



};






#endif
