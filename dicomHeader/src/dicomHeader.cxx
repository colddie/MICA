#include "dicomHeader.h"
#include "toolBase.h"

#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"

// IOFactory already support multi format, add here to enable multi type
// see IO_type class

using namespace std;



void itk::DicomHeader::ReadDicom(const std::string inputdicom)
{

    m_reader = ReaderType::New();
    m_dicomIO = ImageIOType::New();
    m_reader->SetImageIO(m_dicomIO);

    m_nameGenerator = NamesGeneratorType::New();
    m_nameGenerator->SetInputDirectory(inputdicom);

    m_reader->SetFileNames(m_nameGenerator->GetInputFileNames());
    m_reader->Update();

}



void itk::DicomHeader::PrintHeader(const unsigned int slidenum)
{
    m_dictionary = (*(m_reader->GetMetaDataDictionaryArray()))[slidenum];

    typedef itk::MetaDataDictionary DictionaryType;
    DictionaryType dictionary;
    dictionary = *(m_dictionary);
    typedef itk::MetaDataObject<std::string> MetaDataStringType;

    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();

    while (itr != end)
    {

        itk::MetaDataObjectBase::Pointer entry = itr->second;

        MetaDataStringType::Pointer entryvalue =
                dynamic_cast<MetaDataStringType *>(entry.GetPointer());

        if (entryvalue)
        {
            std::string tagkey = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            std::cout << tagkey << " = " << tagvalue << std::endl;

        }

        ++itr;
    }

}


void itk::DicomHeader::ModifyHeader(const string tagid, const string tagvalue)
{
    unsigned int nbSlices = m_nameGenerator->GetInputFileNames().size();

    for (unsigned int i=0; i<nbSlices; i++) {
        m_dictionary = (*(m_reader->GetMetaDataDictionaryArray()))[nbSlices-i-1]; // why need minus?
        itk::EncapsulateMetaData<std::string>(*m_dictionary, tagid, tagvalue);
        m_outputArray.push_back(m_dictionary);

    }

}


void itk::DicomHeader::WriteDicom(const string outputdicom)
{

    typedef signed short    OutputPixelType;
    const unsigned int      OutputDimension = 2;

    typedef itk::Image< OutputPixelType, OutputDimension >    Image2DType;
    typedef itk::ImageSeriesWriter<ImageType, Image2DType> WriterType;
    WriterType::Pointer dicomWriter = WriterType::New();

    dicomWriter->SetInput(m_reader->GetOutput());
    dicomWriter->SetImageIO(m_dicomIO);
    itksys::SystemTools::MakeDirectory( outputdicom.c_str() );

    // generating name list
    itk::NumericSeriesFileNames::Pointer outputNames =itk::NumericSeriesFileNames::New();
    std::string format(outputdicom.c_str());
    format = format + "/" + "IM%03d.dcm";
    outputNames->SetSeriesFormat(format.c_str());
    outputNames->SetStartIndex(1);
    unsigned int nbSlices = m_nameGenerator->GetInputFileNames().size();
    outputNames->SetEndIndex(nbSlices);

    dicomWriter->SetInput(m_reader->GetOutput());
    dicomWriter->SetImageIO(m_dicomIO);
    dicomWriter->SetFileNames(outputNames->GetFileNames());

    dicomWriter->SetMetaDataDictionaryArray(&m_outputArray);
    dicomWriter->Update();


    //    typedef itk::MetaDataDictionary DictionaryType;
    //    DictionaryType dictionary;
    //    dictionary = *(m_outputArray[0]);
    //    typedef itk::MetaDataObject<std::string> MetaDataStringType;

    //    DictionaryType::ConstIterator itr = dictionary.Begin();
    //    DictionaryType::ConstIterator end = dictionary.End();

    //    while (itr != end)
    //    {

    //        itk::MetaDataObjectBase::Pointer entry = itr->second;

    //        MetaDataStringType::Pointer entryvalue =
    //                dynamic_cast<MetaDataStringType *>(entry.GetPointer());

    //        if (entryvalue)
    //        {
    //            std::string tagkey = itr->first;
    //            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
    //            std::cout << tagkey << " = " << tagvalue << std::endl;

    //        }

    //        ++itr;
    //    }

}
