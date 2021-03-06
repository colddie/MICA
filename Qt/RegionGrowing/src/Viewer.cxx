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

    this->ui->label_3->setText("Radius");
    this->ui->label_4->setText("Confidence Interval");
    this->ui->label_5->setText("Iterations");

    this->ui->label_6->setText("3");
    this->ui->label_7->setText("3");

    connect(this->ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(changeSlice(int)));
    connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(Mainfunction()));
    connect(this->ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(updateValueDisplay(int)));
    connect(this->ui->horizontalScrollBar_2, SIGNAL(valueChanged(int)), this, SLOT(updateValueDisplay(int)));
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


    //    VTK_CREATE(vtkImageFlip, flipper);
    //    flipper->SetFilteredAxis(1);
    //    flipper->SetInput(vtkReader->GetOutput());
    //    flipper->Update();

    this->m_viewer = vtkSmartPointer<vtkImageViewer2>::New();

    this->m_viewer->SetInput(vtkReader->GetOutput());
    this->m_viewer->SetSliceOrientationToXY();

    // Fixed invert y axis
    double pos[2], foc[2];
    this->m_viewer->GetRenderer()->GetActiveCamera()->GetPosition(pos);
    this->m_viewer->GetRenderer()->GetActiveCamera()->GetFocalPoint(foc);
    pos[2] = -1;
    this->m_viewer->GetRenderer()->GetActiveCamera()->SetPosition(pos);
    //    std::cout<<pos[0]<<pos[1]<<pos[2]<<std::endl<<foc[0]<<foc[1]<<foc[2]<<std::endl;

    this->m_viewer->GetRenderer()->GetActiveCamera()->SetViewUp(0, -1, 0);
    this->m_viewer->GetRenderer()->ResetCamera();
    this->m_viewer->UpdateDisplayExtent();
    this->m_viewer->SetupInteractor(this->ui->qvtkWidget->GetInteractor());
    this->ui->qvtkWidget->SetRenderWindow(m_viewer->GetRenderWindow());

    // Disable deafult style, must done after set interactor
    this->ui->qvtkWidget->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget->GetInteractor()->Disable();
    VTK_CREATE(MouseInteractorStyle, interactorStyle);
    this->ui->qvtkWidget->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget->GetInteractor()->Initialize();

    this->m_viewer->SetSlice(0);
    this->m_viewer->Render();

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

    //    segmenter->SetInitialNeighborhoodRadius(3);
    //    segmenter->SetMultiplier(3);  //the confidence interval is the mean plus or minus the "Multiplier" times the standard deviation
    //    segmenter->SetNumberOfIterations(5);
    //    segmenter->SetReplaceValue(255);

    segmenter->SetInitialNeighborhoodRadius(this->ui->horizontalScrollBar->value());
    segmenter->SetMultiplier(this->ui->horizontalScrollBar_2->value()/10.0);
    segmenter->SetNumberOfIterations(this->ui->spinBox->value());
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

    //  Connect itk with vtk
    typedef itk::ImageToVTKImageFilter< ImageType > ConnectorType;
    ConnectorType::Pointer connector = ConnectorType::New();

    connector->SetInput( segmenter->GetOutput() );
    connector->Update();
    VTK_CREATE(vtkImageData, imageData);
    imageData->DeepCopy( connector->GetOutput() );   // must copy mem or read empty

    // display the sgemented image
    m_segviewer = vtkSmartPointer<vtkImageViewer2>::New();
    m_segviewer->SetInput(imageData);
    m_segviewer->SetSliceOrientationToXY();
    // Fixed invert y axis
    double pos[2], foc[2];
    m_segviewer->GetRenderer()->GetActiveCamera()->GetPosition(pos);
    m_segviewer->GetRenderer()->GetActiveCamera()->GetFocalPoint(foc);
    pos[2] = -1;
    m_segviewer->GetRenderer()->GetActiveCamera()->SetPosition(pos);
    //    std::cout<<pos[0]<<pos[1]<<pos[2]<<std::endl<<foc[0]<<foc[1]<<foc[2]<<std::endl;

    m_segviewer->GetRenderer()->GetActiveCamera()->SetViewUp(0, -1, 0);
    m_segviewer->GetRenderer()->ResetCamera();
    m_segviewer->UpdateDisplayExtent();
    m_segviewer->SetupInteractor(this->ui->qvtkWidget_2->GetInteractor());
    this->ui->qvtkWidget_2->SetRenderWindow(m_segviewer->GetRenderWindow());

    // Disable deafult style, must done after set interactor
    this->ui->qvtkWidget_2->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget_2->GetInteractor()->Disable();
    VTK_CREATE(MouseInteractorStyle, interactorStyle);
    this->ui->qvtkWidget_2->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget_2->GetInteractor()->Initialize();

    m_segviewer->SetSlice(this->ui->verticalScrollBar->value());
    m_segviewer->Render();


    // write to image
    typedef itk::ImageFileWriter<ImageType> ImageWriterType;
    ImageWriterType::Pointer writer = ImageWriterType::New();

    writer->SetFileName("seg.mha");
    writer->SetInput(segmenter->GetOutput());
    writer->Update();
}


template <typename T>
void Viewer::GetIntensity(unsigned int slicex,
                          unsigned int slicey,
                          unsigned int slicez)
{
    T *intensity = (T*)this->m_viewer->GetInput()
            ->GetScalarPointer(slicex, slicey, slicez);

    this->ui->label_2->setText(QString(vtkVariant(intensity[0]).ToString()));
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
    this->m_viewer->SetSlice(slice);

    // sychronize slicing
    if (this->m_segviewer != NULL)
    {
    this->m_segviewer->SetSlice(slice);
    }
}


void Viewer::updateValueDisplay(int val)
{
    this->ui->label_6->setText(QString::number(
                                   this->ui->horizontalScrollBar->value()));
    this->ui->label_7->setText(QString::number(
                                   this->ui->horizontalScrollBar_2->value()/10.0));
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
    picker->Pick(event_pos[0],event_pos[1],0,m_viewer->GetRenderer());

    //check positoin in image
    if(picker->GetCellId() != -1) {

        //  When the mouse is on the sphere, we need to display the last time value
        //        if (this->m_interactorStyle->InteractionProp == this->m_actor)
        if (picker->GetActor()!=NULL)
        {
            std::cout<<"mouse in the actor!"<<std::endl;
            return;
        }

        QString str;
        str.sprintf("%d  %d  %d", picker->GetCellIJK()[0]+1,
                    picker->GetCellIJK()[1]+1, m_viewer->GetSlice()+1);
        this->ui->label->setText(str);

        switch (this->m_viewer->GetInput()->GetScalarType())
        {
        vtkTemplateMacro(GetIntensity<VTK_TT>(picker->GetCellIJK()[0],
                                              picker->GetCellIJK()[1],
                                              picker->GetCellIJK()[2]));

        default:
            return;
        }
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
    picker->Pick(event_pos[0],event_pos[1],0,m_viewer->GetRenderer());

    double pos[3];
    //check positoin in image
    if(picker->GetCellId() != -1) {
        picker->GetPickPosition(pos);

        m_index[0] = picker->GetCellIJK()[0]+1;
        m_index[1] = picker->GetCellIJK()[1]+1;
        m_index[2] = m_viewer->GetSlice()+1;
    }

//    //Delete existing point
//    vtkPolyDataCollection *actorCollection = this->m_viewer->GetRenderer();
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

    //    VTK_CREATE(vtkSphereSource, sphereSource);
    if (m_sphereSource == NULL)
    {
        m_sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    }
    m_sphereSource->SetCenter(pos[0], pos[1], pos[2]);
    m_sphereSource->SetRadius(10.0);

    //Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(m_sphereSource->GetOutputPort());

//    vtkSmartPointer<vtkActor> actor =
//            vtkSmartPointer<vtkActor>::New();
    m_actor = vtkSmartPointer<vtkActor>::New();
    m_actor->SetMapper(mapper);
    m_actor->GetProperty()->SetColor(0, 0, 1);

    //this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
    this->m_viewer->GetRenderer()->AddActor(m_actor);
    this->m_viewer->Render();


}
