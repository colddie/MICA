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


//    int main_fun(const char *a, std::string &imagetype, unsigned int &dim, unsigned int &channel);
    itk::ImageIOBase::Pointer Create_IO(const char *a);
    std::string Return_ByteOrder(itk::ImageIOBase::Pointer imageio);
    std::string Return_ImageType(itk::ImageIOBase::Pointer imageio);
    std::string Return_PixelType(itk::ImageIOBase::Pointer imageio);
    unsigned int Return_DimensionNumber(itk::ImageIOBase::Pointer imageio);
    unsigned int Return_ChannelNumber(itk::ImageIOBase::Pointer imageio);
    std::vector<unsigned int> Return_DimensionSize(itk::ImageIOBase::Pointer imageio);
    std::vector<double> Return_Spacing(itk::ImageIOBase::Pointer imageio);
    std::vector<double> Return_Origin(itk::ImageIOBase::Pointer imageio);








    IO_type(){};
    ~IO_type(){};

protected:

private:



};






#endif
