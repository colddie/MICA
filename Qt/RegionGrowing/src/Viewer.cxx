#include "Viewer.h"
#include "ui_Viewer.h"
#include "IO_type.h"
#include "toolBase.h"

#include "itkImage.h"
#include "itkSmartPointer.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkMetaImageReader.h"
#include "vtkImageFlip.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkInteractorStyle.h"
#include "vtkCellPicker.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkPolyDataCollection.h"
#include "QVTKInteractor.h"
#include "QVTKWidget.h"
#include "vtkEventQtSlotConnect.h"

#include <QString>
#include <QFileDialog>
#include <iostream>
#include <sstream>
#include <string>



#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New();

vtkStandardNewMacro(MouseInteractorStyle);


Viewer::Viewer(QMainWindow *parent) :
    QMainWindow(parent), ui(new Ui_Viewer)
{

    this->ui->setupUi(this);


    connect(this->ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(changeSlice(int)));
    connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(Mainfunction()));

}




Viewer::~Viewer()
{
  // The smart pointers should clean up for up
    delete ui;
}




void Viewer::DisplayImage()
{
    VTK_CREATE(vtkMetaImageReader, vtkReader);
    vtkReader->SetFileName(m_filename.c_str());
    vtkReader->Update();


    VTK_CREATE(vtkImageFlip, flipper);
    flipper->SetFilteredAxis(1);
    flipper->SetInput(vtkReader->GetOutput());
    flipper->Update();

    this->m_viewer2 = vtkSmartPointer<vtkImageViewer2>::New();

    this->m_viewer2->SetInput(flipper->GetOutput());
    this->m_viewer2->SetSliceOrientationToXY();
    this->m_viewer2->GetRenderer()->ResetCamera();
    this->m_viewer2->UpdateDisplayExtent();
    this->m_viewer2->SetupInteractor(this->ui->qvtkWidget->GetInteractor());
    this->ui->qvtkWidget->SetRenderWindow(m_viewer2->GetRenderWindow());

    // Disable deafult style, must done after set interactor
    this->ui->qvtkWidget->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget->GetInteractor()->Disable();
    VTK_CREATE(MouseInteractorStyle, interactorStyle);
    this->ui->qvtkWidget->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget->GetInteractor()->Initialize();

    this->m_viewer2->SetSlice(0);
    this->m_viewer2->Render();

    // Enable connections between VTK event and Qt slots
//    VTK_CREATE(vtkEventQtSlotConnect, connection);
    connection = vtkSmartPointer<vtkEventQtSlotConnect>::New();

    connection->Connect(this->ui->qvtkWidget->GetInteractor(),
                        vtkCommand::MouseMoveEvent,
                        this,
                        SLOT(mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget->GetInteractor(),
                        vtkCommand::LeftButtonPressEvent,
                        this,
                        SLOT(mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
}



void Viewer::Mainfunction()
{
    std::cout<<"Writing into Files..."<<std::endl;
    //
    typedef itk::Image<short, 3> ImageType;
    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    ImageReaderType::Pointer reader = ImageReaderType::New();

    reader->SetFileName(m_filename);
    reader->Update();

    //
    typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> SegmenterType;
    SegmenterType::Pointer segmenter = SegmenterType::New();

    segmenter->SetInitialNeighborhoodRadius(3);
    segmenter->SetMultiplier(3);
    segmenter->SetNumberOfIterations(25);
    segmenter->SetReplaceValue(255);


    //  Set Seed
    ImageType::IndexType seed;
    seed[0] = m_index[0];
    seed[1] = m_index[1];
    seed[2] = m_index[2];
    std::cout<<seed[0]<<" "<<seed[1]<<" "<<seed[2]<<std::endl;

    segmenter->SetSeed(seed);
    segmenter->SetInput(reader->GetOutput());
    segmenter->Update();


    //
    typedef itk::ImageFileWriter<ImageType> ImageWriterType;
    ImageWriterType::Pointer writer = ImageWriterType::New();

    writer->SetFileName("seg.mha");
    writer->SetInput(segmenter->GetOutput());
    writer->Update();
}





//
void Viewer::slotOpenFile()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.mha *.jpg *.bmp)"));
    m_filename = filename.toLocal8Bit().constData();

    IO_type io;
    io.Create_IO(m_filename.c_str());
    QString byteOrder = "ByteOrder: "+QString(io.Return_ByteOrder().c_str());
    QString imageType = "ImageType: "+QString(io.Return_ImageType().c_str());
    QString pixelType = "PixelType: "+QString(io.Return_PixelType().c_str());
    QString dimensionNumber = "DimensionNumber: "+QString::number(io.Return_DimensionNumber());
    QString channelNumber = "ChannelNumber: "+QString::number(io.Return_ChannelNumber());
    QString dimensionSize = "DimensionSize: "+QString(std::vectorToString<unsigned int>(io.Return_DimensionSize()).c_str());
    QString spacing = "Spacing: "+QString(std::vectorToString<double>(io.Return_Spacing()).c_str());
    QString origin = "Origin: "+QString(std::vectorToString<double>(io.Return_Origin()).c_str());
    QString directionX = "Direction Matrix:"+QString(std::vectorToString<double>(io.Return_Direction(0)).c_str());
    QString directionY = "\t"+QString(std::vectorToString<double>(io.Return_Direction(1)).c_str());
    QString directionZ = "\t"+QString(std::vectorToString<double>(io.Return_Direction(2)).c_str());

    QString toTextBrowser = byteOrder+QString("\n")+imageType+"\n"+pixelType+"\n"+dimensionNumber
            +"\n"+channelNumber+"\n"+dimensionSize+"\n"+spacing+"\n"+origin+"\n"
            +directionX+"\n"+directionY+"\n"+directionZ;
    this->ui->textBrowser->setText(toTextBrowser);

//    Initialize();
//    SetVolumeProperty();
//    UpdateWidget();

    DisplayImage();

//    Mainfunction();


}


void Viewer::slotExit()
{
    qApp->exit();
}


void Viewer::changeSlice(int slice)
{
    this->m_viewer2->SetSlice(slice);
}




void Viewer::mouseMoveCallback(vtkObject * obj, unsigned long,
                               void * client_data, void *,
                               vtkCommand * command)
{
    VTK_CREATE(vtkRenderWindowInteractor, iren);
    // get interactor
    iren = vtkRenderWindowInteractor::SafeDownCast(obj);
    // get event position
    int event_pos[2], iren_size[2];
    iren->GetEventPosition(event_pos);

    VTK_CREATE(vtkCellPicker, picker);
    picker->Pick(event_pos[0],event_pos[1],0,m_viewer2->GetRenderer());

    //check positoin in image
    if(picker->GetCellId() != -1) {
        //        picker->GetPickPosition(pos);

        QString str;
        str.sprintf("%d  %d  %d", picker->GetCellIJK()[0]+1,
                    picker->GetCellIJK()[1]+1, m_viewer2->GetSlice()+1);
        this->ui->label->setText(str);
    }

}



void Viewer::mouseClickCallback(vtkObject * obj, unsigned long,
                                void * client_data, void *,
                                vtkCommand * command)
{

//    QString str = this->ui->label->text();
//    std::string string = str.toLocal8Bit().constData();
//    std::istringstream ss;
//    ss.str(string);

//    std::vector<double> pos;
//    std::copy(std::istream_iterator<double>(ss), std::istream_iterator<double>(), std::back_inserter(pos));
    VTK_CREATE(vtkRenderWindowInteractor, iren);
    // get interactor
    iren = vtkRenderWindowInteractor::SafeDownCast(obj);
    // get event position
    int event_pos[2];
    iren->GetEventPosition(event_pos);

    VTK_CREATE(vtkCellPicker, picker);
    picker->Pick(event_pos[0],event_pos[1],0,m_viewer2->GetRenderer());

    double pos[3];
    //check positoin in image
    if(picker->GetCellId() != -1) {
        picker->GetPickPosition(pos);

        m_index[0] = picker->GetCellIJK()[0]+1;
        m_index[1] = picker->GetCellIJK()[1]+1;
        m_index[2] = m_viewer2->GetSlice()+1;
    }

//    //Delete existing point
//    vtkPolyDataCollection *actorCollection = this->m_viewer2->GetRenderer();
//    actorCollection->InitTraversal();

//    for (vtkIdType i=0; i<actorCollection->GetNumberOfItems(); i++)
//    {
//        vtkActor *nextActor = actorCollection->GetNextActor();
//        std::string className = nextActor->GetClassName();
//        std::cout<<className<<std::endl;
//        if (className == "vtkOpenGLActor")
//        {
//            nextActor->Delete();
//        }
//    }

    VTK_CREATE(vtkSphereSource, sphereSource);
    sphereSource->SetCenter(pos[0], pos[1], pos[2]);
    sphereSource->SetRadius(10.0);

    //Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0, 0, 1);

    //this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
    this->m_viewer2->GetRenderer()->AddActor(actor);
    this->m_viewer2->Render();

}
