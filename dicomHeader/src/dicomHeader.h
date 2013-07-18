#ifndef _dicomHeader_H_
#define _dicomHeader_H_

#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

namespace itk {


class DicomHeader : public itk::Object
{

public:

    void ReadDicom(const std::string inputdicom);
    void PrintHeader(const unsigned int slidenum);
    void ModifyHeader(const std::string tagid, const std::string tagvalue);
    void WriteDicom(const std::string outputdicom);

    DicomHeader(){};
    ~DicomHeader(){};

    typedef signed short PixelType;
    typedef itk::Image<PixelType, 3> ImageType;
    typedef itk::ImageSeriesReader<ImageType> ReaderType;

    ReaderType::DictionaryRawPointer m_dictionary;
    ReaderType::DictionaryArrayType m_outputArray;

    ReaderType::Pointer m_reader;

    typedef itk::GDCMImageIO ImageIOType;
    ImageIOType::Pointer m_dicomIO;
    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer m_nameGenerator;
    //    typedef std::vector<std::string> FileNamesContainer;
    //    FileNamesContainer m_fileNames;

protected:

private:

};


}







#endif
