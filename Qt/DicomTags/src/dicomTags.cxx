#include "ui_dicomTags.h"
#include "dicomTags.h"
#include "connection.h"
#include "tempSql.h"

#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

#include <QFileDialog>
#include <QStandardItemModel>

Viewer::Viewer(QMainWindow *parent)
    :QMainWindow(parent), ui(new Ui::dicomTags)
{

    this->ui->setupUi(this);

    connect(this->ui->actionOpen,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(this->ui->actionApply_Change,SIGNAL(triggered()),this,SLOT(slotApplyChange()));

    createConnection();
    //    model = new QStandardItemModel(this);
    model = new QSqlTableModel(this);
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Tags"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Value"));
    qDebug() << model->lastError().text();
//    this->ui->tableView->setModel(model);
//    this->ui->tableView->update();
};





Viewer::~Viewer()
{





}



void Viewer::ReadDicom(const std::string dir)
{
    typedef signed short PixelType;
    const unsigned int Dimension = 3;
    typedef itk::Image<PixelType, Dimension> ImageType;

    typedef itk::ImageSeriesReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();

    typedef itk::GDCMImageIO ImageIOType;
    ImageIOType::Pointer dicomIO = ImageIOType::New();

    reader->SetImageIO(dicomIO);

    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetInputDirectory(dir);

    typedef std::vector<std::string> FileNamesContainer;
    FileNamesContainer fileNames = nameGenerator->GetInputFileNames();
    reader->SetFileNames(fileNames);
    reader->Update();

    //    typedef itk::MetaDataDictionary DictionaryType;
    //    const DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    dictionary = dicomIO->GetMetaDataDictionary();


    typedef itk::MetaDataObject<std::string> MetaDataStringType;
    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();


    DatabaseManager *manager;
    if (!manager->openDB()) {
        qDebug() << "Failed to open dicom tags database!";
        exit(1);
    }

    TagData *tagData;
    tagData->TagId = "";
    tagData->Name = "";
//    manager->getTag(QString("0002,0000"), tagData);
//    qDebug() << tagData->TagId <<" "<< tagData->Name;


    int rowNum;

    while (itr != end)
    {
        rowNum = model->rowCount(); // get current row counts

        itk::MetaDataObjectBase::Pointer entry = itr->second;

        MetaDataStringType::Pointer entryvalue =
                dynamic_cast<MetaDataStringType *>(entry.GetPointer());

        if (entryvalue)
        {
            std::string tagkey = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            //            std::cout << tagkey << " = " << tagvalue << std::endl;

            model->insertRow(rowNum);
            model->setData(model->index(rowNum,0),QString::fromStdString(tagkey));
            model->setData(model->index(rowNum,2),QString::fromStdString(tagvalue));

            if(!manager->getTag(QString::fromStdString(tagkey), tagData))
            {
                qDebug() << "Failed to get tag!";
                tagData->Name = "";
            }
            qDebug() << tagData->TagId<< tagData->Name;
            model->setData(model->index(rowNum,1),tagData->Name);
        }


       ++itr;
    }

    model->submitAll(); // manual submit

    //    qDebug() << model->lastError().text();
    this->ui->tableView->setModel(model);
    this->ui->tableView->update();
    connect(this->model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(slotChangeTag(QModelIndex,QModelIndex)));
}





void Viewer::slotOpen()
{
    QFileDialog dirDialog(this);
    dirDialog.setFileMode(QFileDialog::Directory);
//    QString filename = dirDialog.getOpenFileName(this,
//        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.mha *.jpg *.bmp)"));
    QString fileName = dirDialog.getExistingDirectory(this,tr("Open Directory"),QDir::currentPath());
    qDebug() << fileName;
    ReadDicom(fileName.toStdString());


}


void Viewer::slotChangeTag(const QModelIndex &topLeft,const QModelIndex &bottomRight)
{
//    qDebug() << topLeft << bottomRight;
//    qDebug() << model->data(topLeft,Qt::DisplayRole).toString();
//    qDebug() << model->data(model->index(topLeft.row(),topLeft.column()-1),Qt::DisplayRole).toString();

    QString key = model->data(topLeft,Qt::DisplayRole).toString();
    QString value = model->data(model->index(topLeft.row(),topLeft.column()-1),Qt::DisplayRole).toString();

    toChange.insert(key,value);

}


void Viewer::slotApplyChange()
{

    QMap<QString, QString>::const_iterator i=toChange.constBegin();
    while(i!=toChange.constEnd()) {
        qDebug() << i.key() << i.value();
        std::string entryID = QString(i.key()).toLocal8Bit().constData();
        std::string IDvalue = QString(i.value()).toLocal8Bit().constData();
        itk::EncapsulateMetaData(dictionary, entryID, IDvalue);
        ++i;
    }





}
