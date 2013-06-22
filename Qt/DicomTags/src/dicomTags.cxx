#include "ui_dicomTags.h"
#include "dicomTags.h"
#include "connection.h"
#include "tempSql.h"
#include "tools.h"

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

#include <QFileDialog>
#include <QStandardItemModel>

Viewer::Viewer(QMainWindow *parent)
    :QMainWindow(parent), ui(new Ui::dicomTags)
{

    this->ui->setupUi(this);

    connect(this->ui->actionOpen,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    connect(this->ui->actionApply_Change,SIGNAL(triggered()),this,SLOT(slotApplyChange()));
    connect(this->ui->PathListWidget, SIGNAL(clicked(QModelIndex)),this,SLOT(slotGetCurrentFile(QModelIndex)));

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
    //    typedef signed short PixelType;
    //    const unsigned int Dimension = 3;
    //    typedef itk::Image<PixelType, Dimension> ImageType;

    //    typedef itk::ImageSeriesReader<ImageType> ReaderType;
    reader = ReaderType::New();

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


    //
    QList<QString> list;
    std::transform(fileNames.begin(),fileNames.end(),std::front_inserter(list),&QString::fromStdString);
    this->ui->PathListWidget->addPaths(list);
    this->ui->PathListWidget->update();

    //    typedef itk::MetaDataDictionary DictionaryType;
    //    const DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    ReaderType::DictionaryRawPointer dictionary;
    ReaderType::DictionaryArrayType outputArray;


    dictionary = (*(reader->GetMetaDataDictionaryArray()))[0];
    m_dictionary = *(dictionary);

}



void Viewer::DisplayTag()
{
    typedef itk::MetaDataObject<std::string> MetaDataStringType;
    //    typedef itk::MetaDataDictionary DictionaryType;
    DictionaryType::ConstIterator itr = m_dictionary.Begin();
    DictionaryType::ConstIterator end = m_dictionary.End();


    DatabaseManager *manager = new DatabaseManager(this);
    //    qDebug() << manager->TagID << manager->Name;


    if (!manager->openDB()) {
        qDebug() << "Failed to open dicom tags database!";
        exit(1);
    }


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

            if(!manager->getTag(QString::fromStdString(tagkey)))
            {
                qDebug() << "Failed to get tag!";
            }
            //            qDebug() << manager->TagID<< manager->Name;
            model->setData(model->index(rowNum,1),manager->Name);
        }


       ++itr;
    }

    model->submitAll(); // manual submit

    //    qDebug() << model->lastError().text();
    this->ui->tableView->setModel(model);
    this->ui->tableView->update();
    connect(this->model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(slotChangeTag(QModelIndex,QModelIndex)));

}


void Viewer::slotOpenFile()
{
    QFileDialog dirDialog(this);
    dirDialog.setFileMode(QFileDialog::Directory);
    //    QString filename = dirDialog.getOpenFileName(this,
    //        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.mha *.jpg *.bmp)"));
    QString fileName = dirDialog.getExistingDirectory(this,tr("Open Directory"),QDir::currentPath());
    qDebug() << fileName;
    ReadDicom(fileName.toStdString());
    DisplayTag();


}



void Viewer::slotGetCurrentFile(const QModelIndex &fileIndex)
{
    qDebug() << fileIndex.row() << fileIndex.column();

    m_dictionary = *((*(reader->GetMetaDataDictionaryArray()))[fileIndex.row()]);

    // empty the model
    model->removeRows(0,model->rowCount());
    model->submitAll();
    DisplayTag();

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
        //        itk::EncapsulateMetaData(dictionary, entryID, IDvalue);
        ++i;
    }

}
