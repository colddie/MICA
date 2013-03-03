#ifndef _IO_type_h_
#define _IO_type_h_

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <string.h>

class IO_type
{

public:

    int main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel);

    IO_type(){};
    ~IO_type(){};

protected:

private:



};






#endif
