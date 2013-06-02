/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

#include "ui_panelViewer.h"
#include "panelViewer.h"
#include "IO_type.h"
#include "toolBase.h"

#include <QSignalMapper>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMetaImageIO.h"
#include "itkTextOutput.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkMetaImageReader.h"
#include "vtkDataSetTriangleFilter.h"
#include "vtkSmartPointer.h"
#include "vtkWin32OutputWindow.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkCommand.h"
#include "vtkCellPicker.h"
#include "QVTKInteractor.h"
#include "vtkInteractorStyle.h"
//#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"
//#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkImageFlip.h"
#include "vtkLineSource.h"
//#include "vtkPolyDataMapper.h"
//#include "vtkActor.h"
//#include "vtkProperty.h"
#include "QVTKWidget.h"

#include "qxtlabel.h"

#include "ctkVTKConnection.h"
#include "ctkVTKMagnifyView.h"



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

vtkStandardNewMacro(MouseInteractorStyle);


// Constructor
//Viewer::Viewer(int windowhigh, int windowlow, string filename)
Viewer::Viewer(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::panelViewer)
{


    //
    loadImage = false;
    currentPosition.reserve(3);
    imageDimension.reserve(3);
    imageSpacing.reserve(3);

    //    this->ui = new Ui::panelViewer;
    this->ui->setupUi(this);

    //
    this->ui->VTKMagnify->observe(this->ui->qvtkWidget_1);
    this->ui->VTKMagnify->observe(this->ui->qvtkWidget_2);
    this->ui->VTKMagnify->observe(this->ui->qvtkWidget_3);
    this->ui->VTKMagnify->setMagnification(2.0);

    // Slider 1st
    //    this->ui->verticalSlider_1->setMinimum(-1024);
    //    this->ui->verticalSlider_1->setMaximum(3024);
    //    this->ui->verticalSlider_1->setSingleStep(10);
    //    this->ui->verticalSlider_1->setPageStep(100);
    this->ui->verticalSlider_1->setRange(0,1);
    this->ui->verticalSlider_1->setValue(0);

    // Slider 2nd
    this->ui->verticalSlider_2->setRange(0,1);
    this->ui->verticalSlider_2->setValue(0);

    // Slider 3st
    this->ui->verticalSlider_3->setRange(0,1);
    this->ui->verticalSlider_3->setValue(0);

    // ComboBox
    QStringList choices;
    choices << "Default" << "CT Abodomen" << "CT Brain" << "CT Extremities"
            << "CT Liver" << "CT Lung" << "CT Pelvis" << "CT Skull"
            << "CT Spline A" << "CT Spline B" << "CT Thorax";
    this->ui->comboBox->insertItems(0, choices);



    // QVtk objects
    //    this->render1 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerAxial = vtkSmartPointer<vtkImageViewer2>::New();
    //    this->ui->qvtkWidget_1->GetRenderWindow()->AddRenderer(render1);

    //    this->render2 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerCoronal = vtkSmartPointer<vtkImageViewer2>::New();
    //    this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(render2);

    //    this->render3 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerSagittal = vtkSmartPointer<vtkImageViewer2>::New();
    //    this->ui->qvtkWidget_3->GetRenderWindow()->AddRenderer(render3);


//    AddCrosshair();

    // Force output to stdout
    // On Windows systems, the output of warnings is normally sent to a
    // window and not the shell
    itk::OutputWindow::SetInstance(itk::TextOutput::New());

    itkGenericOutputMacro(<< "This should appear in the console window, not in a pop up window");


   // Set up action signals and slots
    connect(this->ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->comboBox, SIGNAL(activated(int)), this, SLOT(SetWindowLevel(int)));

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(SetSlice(QWidget*)));

    signalMapper->setMapping(this->ui->verticalSlider_1, this->ui->verticalSlider_1);
    signalMapper->setMapping(this->ui->verticalSlider_2, this->ui->verticalSlider_2);
    signalMapper->setMapping(this->ui->verticalSlider_3, this->ui->verticalSlider_3);

    connect(this->ui->verticalSlider_1, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
    connect(this->ui->verticalSlider_2, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
    connect(this->ui->verticalSlider_3, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));


    // Enable connections between VTK event and Qt slots
    //    VTK_CREATE(vtkEventQtSlotConnect, connection);
    connection = vtkSmartPointer<vtkEventQtSlotConnect>::New();

    // Setup mouse event, called before default vtkinteractor
    // this is dirty to set global qvtkWidget, need to change to qt 5
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::EnterEvent,
                        this,
                        SLOT(mouseEnterCallback_1()));
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::EnterEvent,
                        this,
                        SLOT(mouseEnterCallback_2()));
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::EnterEvent,
                        this,
                        SLOT(mouseEnterCallback_3()));
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::LeaveEvent,
                        this,
                        SLOT(mouseLeaveCallback()));
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::LeaveEvent,
                        this,
                        SLOT(mouseLeaveCallback()));
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::LeaveEvent,
                        this,
                        SLOT(mouseLeaveCallback()));

    //
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::MouseWheelForwardEvent,
                        this,
                        SLOT(mouseWheelForwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::MouseWheelBackwardEvent,
                        this,
                        SLOT(mouseWheelBackwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::MouseMoveEvent,
                        this,
                        SLOT(mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget_1->GetInteractor(),
                        vtkCommand::LeftButtonPressEvent,
                        this,
                        SLOT(mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::MouseWheelForwardEvent,
                        this,
                        SLOT(mouseWheelForwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::MouseWheelBackwardEvent,
                        this,
                        SLOT(mouseWheelBackwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::MouseMoveEvent,
                        this,
                        SLOT(mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget_2->GetInteractor(),
                        vtkCommand::LeftButtonPressEvent,
                        this,
                        SLOT(mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::MouseWheelForwardEvent,
                        this,
                        SLOT(mouseWheelForwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::MouseWheelBackwardEvent,
                        this,
                        SLOT(mouseWheelBackwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,1.0);   // set priority
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::MouseMoveEvent,
                        this,
                        SLOT(mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);
    connection->Connect(this->ui->qvtkWidget_3->GetInteractor(),
                        vtkCommand::LeftButtonPressEvent,
                        this,
                        SLOT(mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
                        NULL,2.0);

    connection->PrintSelf(std::cout, vtkIndent());

//    for (unsigned int i=0; i<3; i++) {
//        // mouse wheel for zoom
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::MouseWheelForwardEvent,
//                            this->ui->verticalSlider_1,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            1.0);
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::MouseWheelBackwardEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            1.0);
//        // mouse dragging and left click
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::LeftButtonPressEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            2.0);
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::MouseMoveEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            2.0);
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::LeftButtonReleaseEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            2.0);
//        // mouse in and out the widget
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::EnterEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            1.0);
//        connection->Connect(this->ui->qvtkWidget_1,
//                            vtkCommand::LeaveEvent,
//                            this,
//                            SLOT(vtkEventCallback(vtkObject*,ulong,void*,void*,vtkCommand*)),
//                            NULL,
//                            1.0);

//    }

};

Viewer::~Viewer()
{
  // The smart pointers should clean up for up

  // Delete pointers
}


void Viewer::LoadVolume(std::string filename)
{

    //
    loadImage = true;

    //    typedef itk::Image< unsigned short, 3 >    	 ImageType;

    //    // Declare a filter
    //    typedef itk::ImageToVTKImageFilter< ImageType >   FilterType;
    //    FilterType::Pointer connector = FilterType::New();

    //    // Declare a reader
    //    typedef itk::ImageFileReader< ImageType > VolumeReaderType;
    //    VolumeReaderType::Pointer reader = VolumeReaderType::New();
    //    reader->SetFileName( filename.c_str() );
    //    reader->Update();

    //    //  Connect itk with vtk
    //    connector->SetInput( reader->GetOutput() );
    //    connector->Update();
    //    VTK_CREATE(vtkImageData, imageData);
    //    imageData->DeepCopy( connector->GetOutput() );   // must copy mem or read empty
    //    imageData->Update();


    VTK_CREATE(vtkMetaImageReader, reader);
    reader->SetFileName(filename.c_str());
    reader->Update();

    VTK_CREATE(vtkImageFlip, flipper);
    flipper->SetFilteredAxis(1);
    flipper->SetInput(reader->GetOutput());
    flipper->Update();


    // Slider
    int *ip = reader->GetDataExtent();
    unsigned int imageWidth = ip[1]-ip[0]+1;
    unsigned int imageHeight = ip[3]-ip[2]+1;
    unsigned int numSlices = ip[5]-ip[4]+1;
    imageDimension[0] = imageWidth;
    imageDimension[1] = imageHeight;
    imageDimension[2] = numSlices;
    double *ip1 = reader->GetOutput()->GetSpacing();
    imageSpacing.push_back(ip1[0]);
    imageSpacing.push_back(ip1[1]);
    imageSpacing.push_back(ip1[2]);
    this->ui->verticalSlider_1->setRange(0, numSlices-1);
    this->ui->verticalSlider_2->setRange(0, imageHeight-1);
    this->ui->verticalSlider_3->setRange(0, imageWidth-1);


    //  Declare vtk renderwindow
    this->viewerAxial->SetInput(flipper->GetOutput());
    this->viewerAxial->UpdateDisplayExtent();
    this->viewerAxial->SetupInteractor(this->ui->qvtkWidget_1->GetInteractor());
    //    this->viewerAxial->SetRenderWindow(this->ui->qvtkWidget_1->GetRenderWindow());
    this->ui->qvtkWidget_1->SetRenderWindow(this->viewerAxial->GetRenderWindow());
    // Disable deafult style
    this->ui->qvtkWidget_1->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget_1->GetInteractor()->Disable();
    VTK_CREATE(MouseInteractorStyle, interactorStyle);
    this->ui->qvtkWidget_1->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget_1->GetInteractor()->Initialize();
    //    this->ui->qvtkWidget_1->GetInteractor()->Start();
    //    this->viewerAxial->SetRenderer(render1);
    this->viewerAxial->GetRenderer()->ResetCamera();

    //  Filp y axis, need to find a better way
    //    this->viewerAxial->GetRenderer()->GetActiveCamera()->SetRoll(180);
    //    this->viewerAxial->GetRenderer()->GetActiveCamera()->SetViewUp(0,-1,0);
    this->viewerAxial->SetSlice(1);
    this->viewerAxial->Render();


    //
    this->viewerCoronal->SetInput(flipper->GetOutput());
    this->viewerCoronal->SetSliceOrientationToXZ();
    this->viewerCoronal->UpdateDisplayExtent();
    this->viewerCoronal->SetupInteractor(this->ui->qvtkWidget_2->GetInteractor());
    this->ui->qvtkWidget_2->SetRenderWindow(this->viewerCoronal->GetRenderWindow());
    // Disable deafult style
    this->ui->qvtkWidget_2->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget_2->GetInteractor()->Disable();
    this->ui->qvtkWidget_2->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget_2->GetInteractor()->Initialize();
    //    this->ui->qvtkWidget_1->GetInteractor()->Start();
    //    this->viewerAxial->SetRenderer(render1);
    this->viewerCoronal->GetRenderer()->ResetCamera();
    this->viewerCoronal->SetSlice(1);
    this->viewerCoronal->Render();



    //
    this->viewerSagittal->SetInput(flipper->GetOutput());
    this->viewerSagittal->SetSliceOrientationToYZ();
    this->viewerSagittal->UpdateDisplayExtent();
    this->viewerSagittal->SetupInteractor(this->ui->qvtkWidget_3->GetInteractor());
    //    this->viewerAxial->SetRenderWindow(this->ui->qvtkWidget_1->GetRenderWindow());
    this->ui->qvtkWidget_3->SetRenderWindow(this->viewerSagittal->GetRenderWindow());

    // Disable deafult style
    this->ui->qvtkWidget_3->GetInteractor()->GetInteractorStyle()->EnabledOff();
    this->ui->qvtkWidget_3->GetInteractor()->Disable();
    this->ui->qvtkWidget_3->GetInteractor()->SetInteractorStyle(interactorStyle);
    this->ui->qvtkWidget_3->GetInteractor()->Initialize();
    //    this->ui->qvtkWidget_1->GetInteractor()->Start();

    //    this->viewerAxial->SetRenderer(render1);
    this->viewerSagittal->GetRenderer()->ResetCamera();
    this->viewerSagittal->SetSlice(1);
    this->viewerSagittal->Render();

    AddCrosshair();
}




void Viewer::UpdateWidget()
{

    //    ren1->SetBackground(0.1,0.21,0.31);

    // VTK/Qt wedded
//    this->ui->qvtkWidget_1->GetRenderWindow()->AddRenderer(this->viewerAxial->GetRenderer());
    this->ui->qvtkWidget_1->GetRenderWindow()->Render();
    this->ui->qvtkWidget_1->update();

//    this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(this->viewerCoronal->GetRenderer());
    this->ui->qvtkWidget_2->GetRenderWindow()->Render();
    this->ui->qvtkWidget_2->update();

//    this->ui->qvtkWidget_3->GetRenderWindow()->AddRenderer(this->viewerSagittal->GetRenderer());
    this->ui->qvtkWidget_3->GetRenderWindow()->Render();
    this->ui->qvtkWidget_3->update();
}



void Viewer::AddCrosshair()
{

    // Create lines
    for (unsigned int i=0; i<3; i++) {
        horizontalLine.push_back(vtkSmartPointer<vtkLineSource>::New());
        horizontalLineMapper.push_back(vtkSmartPointer<vtkPolyDataMapper>::New());
        horizontalLineActor.push_back(vtkSmartPointer<vtkActor>::New());
        verticalLine.push_back(vtkSmartPointer<vtkLineSource>::New());
        verticalLineMapper.push_back(vtkSmartPointer<vtkPolyDataMapper>::New());
        verticalLineActor.push_back(vtkSmartPointer<vtkActor>::New());
    }

    // initilize drawing lines
    for (unsigned int i=0; i<3; i++) {
        horizontalLine[i]->SetPoint1(-256,0,0);
        horizontalLine[i]->SetPoint2(256,0,0);
        horizontalLine[i]->Update();
        horizontalLineMapper[i]->SetInput(horizontalLine[i]->GetOutput());
        horizontalLineMapper[i]->Update();
        horizontalLineActor[i]->SetMapper(horizontalLineMapper[i]);
        horizontalLineActor[i]->GetProperty()->SetOpacity(0); // hide when initializing
        horizontalLineActor[i]->GetProperty()->SetLineWidth(2.0);
        horizontalLineActor[i]->GetProperty()->SetColor(1.0,1.0,0.0);


        verticalLine[i]->SetPoint1(0,-256,0);
        verticalLine[i]->SetPoint2(0,256,0);
        verticalLine[i]->Update();
        verticalLineMapper[i]->SetInput(verticalLine[i]->GetOutput());
        verticalLineMapper[i]->Update();
        verticalLineActor[i]->SetMapper(verticalLineMapper[i]);
        verticalLineActor[i]->GetProperty()->SetOpacity(0); // hide when initializing
        verticalLineActor[i]->GetProperty()->SetLineWidth(2.0);
        verticalLineActor[i]->GetProperty()->SetColor(1.0,1.0,0.0);
    }


    // Add to actors
    this->viewerAxial->GetRenderer()->AddActor(horizontalLineActor[0]);
    this->viewerAxial->GetRenderer()->AddActor(verticalLineActor[0]);
    this->viewerCoronal->GetRenderer()->AddActor(horizontalLineActor[1]);
    this->viewerCoronal->GetRenderer()->AddActor(verticalLineActor[1]);
    this->viewerSagittal->GetRenderer()->AddActor(horizontalLineActor[2]);
    this->viewerSagittal->GetRenderer()->AddActor(verticalLineActor[2]);

}


void Viewer::RedrawCrosshair()
{

    //
    int xWidth = imageDimension[0]/2*imageSpacing[0];
    int yHeight = imageDimension[1]/2*imageSpacing[1];
    int zSlice = imageDimension[2];

    /* Use the Z-range of of the original image to tell help tell us where to place the line.
        *  We need to direction of projection so that we always place the line infront of the original image
        *  from the camera's perspective
        */
    horizontalLine[0]->SetPoint1(-xWidth,currentPosition[1]-yHeight,0);
    horizontalLine[0]->SetPoint2(xWidth,currentPosition[1]-yHeight,0);
    horizontalLineActor[0]->GetProperty()->SetOpacity(0.5);
    horizontalLine[0]->Update();
    verticalLine[0]->SetPoint1(xWidth-currentPosition[0],-yHeight,0);
    verticalLine[0]->SetPoint2(xWidth-currentPosition[0],yHeight,0);
    verticalLineActor[0]->GetProperty()->SetOpacity(0.5);
    verticalLine[0]->Update();

    horizontalLine[1]->SetPoint1(-xWidth,-1000,(currentPosition[2]-zSlice)*imageSpacing[2]);
    horizontalLine[1]->SetPoint2(xWidth,-1000,(currentPosition[2]-zSlice)*imageSpacing[2]);
    horizontalLineActor[1]->GetProperty()->SetOpacity(0.5);
    horizontalLine[1]->Update();
    verticalLine[1]->SetPoint1(xWidth-currentPosition[0],-1000,-zSlice*imageSpacing[2]);  // -1000 to put line in front
    verticalLine[1]->SetPoint2(xWidth-currentPosition[0],-1000,0);
    verticalLineActor[1]->GetProperty()->SetOpacity(0.5);
    verticalLine[1]->Update();

    horizontalLine[2]->SetPoint1(1000,-yHeight,(currentPosition[2]-zSlice)*imageSpacing[2]); // 1000 to put line in front
    horizontalLine[2]->SetPoint2(1000,yHeight,(currentPosition[2]-zSlice)*imageSpacing[2]);
    horizontalLineActor[2]->GetProperty()->SetOpacity(0.5);
    horizontalLine[2]->Update();
    verticalLine[2]->SetPoint1(1000,currentPosition[1]-yHeight,-zSlice*imageSpacing[2]);
    verticalLine[2]->SetPoint2(1000,currentPosition[1]-yHeight,0);
    verticalLineActor[2]->GetProperty()->SetOpacity(0.5);
    verticalLine[2]->Update();


}


//void Viewer::vtkEventCallBack(vtkObject *caller, unsigned long vtkEvent,
//                  void *clientData, void *callData, vtkCommand *command)
//{
//    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(caller);

//    switch()
//    {
//    case :
//    }

//}



// mouse event callbacks
void Viewer::mouseEnterCallback_1()
{
    currentWidget = "qvtkWidget_1";
    qDebug() << "Enter QVtkWidget" << currentWidget;
}

void Viewer::mouseEnterCallback_2()
{
    currentWidget = "qvtkWidget_2";
    qDebug() << "Enter QVtkWidget" << currentWidget;
}

void Viewer::mouseEnterCallback_3()
{
    currentWidget = "qvtkWidget_3";
    qDebug() << "Enter QVtkWidget" << currentWidget;
}


void Viewer::mouseLeaveCallback()
{
    qDebug() << "Leave QVtkWidget" << currentWidget;
    currentWidget = "";
}


void Viewer::mouseWheelForwardCallback(vtkObject * obj, unsigned long,
                                       void * client_data, void *,
                                       vtkCommand * command)
{
    if (currentWidget == "qvtkWidget_1") {
    this->ui->verticalSlider_1->setValue(this->ui->verticalSlider_1->value()+1);
    }
    if (currentWidget == "qvtkWidget_2") {
    this->ui->verticalSlider_2->setValue(this->ui->verticalSlider_2->value()+1);
    }
    if (currentWidget == "qvtkWidget_3") {
    this->ui->verticalSlider_3->setValue(this->ui->verticalSlider_3->value()+1);
    }
    UpdateWidget();
}


void Viewer::mouseWheelBackwardCallback(vtkObject * obj, unsigned long,
                                        void * client_data, void *,
                                        vtkCommand * command)
{
    if (currentWidget == "qvtkWidget_1") {
    this->ui->verticalSlider_1->setValue(this->ui->verticalSlider_1->value()-1);
    }
    if (currentWidget == "qvtkWidget_2") {
    this->ui->verticalSlider_2->setValue(this->ui->verticalSlider_2->value()-1);
    }
    if (currentWidget == "qvtkWidget_3") {
    this->ui->verticalSlider_3->setValue(this->ui->verticalSlider_3->value()-1);
    }
    UpdateWidget();
}


void Viewer::mouseMoveCallback(vtkObject * obj, unsigned long,
                               void * client_data, void *,
                               vtkCommand * command)
{

    if(loadImage) {
    VTK_CREATE(vtkRenderWindowInteractor, iren);
    // get interactor
    iren = vtkRenderWindowInteractor::SafeDownCast(obj);
    // get event position
    int event_pos[2], iren_size[2];
    iren->GetEventPosition(event_pos);
    //    iren->GetSize(iren_size);
    //    //update label
    //    QString str;
    //    str.sprintf("x=%d  y=%d  z=%d", event_pos[0], iren_size[1]-event_pos[1]-1,
    //               this->ui->verticalSlider_1->value());
    //    this->ui->label->setText(str);

    VTK_CREATE(vtkCellPicker, picker);
    // pick on this pixel
    if (currentWidget == "qvtkWidget_1") {
    picker->Pick(event_pos[0],event_pos[1],0,this->viewerAxial->GetRenderer());

    //check positoin in image
    if(picker->GetCellId() != -1) {
        QString str;
        str.sprintf("x=%d  y=%d  z=%d", picker->GetCellIJK()[0]+1,
                    picker->GetCellIJK()[1]+1, this->ui->verticalSlider_1->value()+1);

        this->ui->label->setText(str);
    }
    }
    if (currentWidget == "qvtkWidget_2") {
    picker->Pick(event_pos[0],event_pos[1],0,this->viewerCoronal->GetRenderer());

    //check positoin in image
    if(picker->GetCellId() != -1) {
        QString str;
        str.sprintf("x=%d  y=%d  z=%d", picker->GetCellIJK()[0]+1,
                    this->ui->verticalSlider_2->value()+1, picker->GetCellIJK()[2]+1);
        this->ui->label->setText(str);
    }
    }
    if (currentWidget == "qvtkWidget_3") {
    picker->Pick(event_pos[0],event_pos[1],0,this->viewerSagittal->GetRenderer());

    //check positoin in image
    if(picker->GetCellId() != -1) {
        QString str;
        str.sprintf("x=%d  y=%d  z=%d", this->ui->verticalSlider_3->value()+1,
                    picker->GetCellIJK()[1]+1, picker->GetCellIJK()[2]+1);
        this->ui->label->setText(str);
    }
    }
    }
}


void Viewer::mouseClickCallback(vtkObject * obj, unsigned long,
                                void * client_data, void *,
                                vtkCommand * command)
{

    if(loadImage) {
    VTK_CREATE(vtkRenderWindowInteractor, iren);
    // get interactor
    iren = vtkRenderWindowInteractor::SafeDownCast(obj);

    // get event position
    int event_pos[2], iren_size[2];
    iren->GetEventPosition(event_pos);

    // pick on this pixel
    VTK_CREATE(vtkCellPicker, picker);

    if (currentWidget == "qvtkWidget_1") {
        picker->Pick(event_pos[0],event_pos[1],0,this->viewerAxial->GetRenderer());
        //check positoin in image
        if(picker->GetCellId() != -1) {
            //            QString str;
            //            str.sprintf("x=%d  y=%d  z=%d", imageWidth-1-picker->GetCellIJK()[0]+1,
            //                        picker->GetCellIJK()[1]+1, picker->GetCellIJK()[2]+1);
            //            this->ui->label->setText(str);
            currentPosition[0]=imageDimension[0]-1-picker->GetCellIJK()[0]-1;
            currentPosition[1]=picker->GetCellIJK()[1]+1;            
        }
    }
    if (currentWidget == "qvtkWidget_2") {
        picker->Pick(event_pos[0],event_pos[1],0,this->viewerCoronal->GetRenderer());

        //check positoin in image
        if(picker->GetCellId() != -1) {
            //            QString str;
            //            str.sprintf("x=%d  y=%d  z=%d", picker->GetCellIJK()[0]+1,
            //                        picker->GetCellIJK()[1]+1, picker->GetCellIJK()[2]+1);
            //            this->ui->label->setText(str);
            currentPosition[0]=imageDimension[0]-1-picker->GetCellIJK()[0]-1;
            currentPosition[2]=picker->GetCellIJK()[2]+1;
        }
    }

    if (currentWidget == "qvtkWidget_3") {
        picker->Pick(event_pos[0],event_pos[1],0,this->viewerSagittal->GetRenderer());

        //check positoin in image
        if(picker->GetCellId() != -1) {
            //            QString str;
            //            str.sprintf("x=%d  y=%d  z=%d", picker->GetCellIJK()[0]+1,
            //                        picker->GetCellIJK()[1]+1, picker->GetCellIJK()[2]+1);
            //            this->ui->label->setText(str);
            currentPosition[1]=picker->GetCellIJK()[1]+1;
            currentPosition[2]=picker->GetCellIJK()[2]+1;
        }
    }

    this->ui->verticalSlider_1->setValue(currentPosition[2]);
    this->ui->verticalSlider_2->setValue(currentPosition[1]);
    this->ui->verticalSlider_3->setValue(currentPosition[0]);
    this->viewerAxial->SetSlice(currentPosition[2]);
    this->viewerCoronal->SetSlice(currentPosition[1]);
    this->viewerSagittal->SetSlice(currentPosition[0]);
    RedrawCrosshair();
    UpdateWidget();
    }


}



// Slot functions
 void Viewer::slotOpenFile()
 {
     QString filename = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::currentPath(), tr("Image Files (*.mha * .vtk *.jpg *.bmp)"));

     if (filename == "") {
         return;
     }

     std::string m_filename = filename.toLocal8Bit().constData();

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

     LoadVolume(m_filename);
     UpdateWidget();

 }

 void Viewer::slotExit()
 {
   qApp->exit();
 }


 void Viewer::SetSlice(QWidget *widget)
 {
     QSlider *slider = qobject_cast<QSlider*>(widget);
//     qDebug("Hello");
     if(!slider) {
         return;
     }
     else {
         int slice = slider->value();
         if(!QString::compare(slider->objectName(),"verticalSlider_1")) {
             this->viewerAxial->SetSlice(slice);
             currentPosition[2] = slice;
             UpdateWidget(); qDebug() << slice << "z";
         }

         else if(!QString::compare(slider->objectName(),"verticalSlider_2")) {
             this->viewerCoronal->SetSlice(slice);
             currentPosition[1] = slice;
             UpdateWidget(); qDebug() << slice << "y";
         }

         else if(!QString::compare(slider->objectName(),"verticalSlider_3")) {
             this->viewerSagittal->SetSlice(slice);
             currentPosition[0] = slice;
             UpdateWidget(); qDebug() << slice << "x";
         }
         else
             return;
     }

 }


void Viewer::SetWindowLevel(int index)
{
    int level, window;
    switch(index) {
    case 0:  // default
        level = 128;
        window = 256;
        break;
    case 1:  // abdomen
        level = 40;
        window = 400;
        break;
    case 2:  // brain
        break;
    case 3:  // extremities
        level = 0;
        window = 400;
        break;
    case 4:  // liver
        break;
    case 5:  // lung
        level = -300;
        window = 400;
        break;
    case 6:  // pelvis
        break;
    case 7:  // skull
        level = 1000;
        window = 400;
        break;
    case 8:  // spline 1
        break;
    case 9:  // spline 2
        break;
    case 10: // thorax
        break;
    default:
        level = 128;
        window = 256;
        break;
    }

    this->viewerAxial->SetColorLevel(level);
    this->viewerAxial->SetColorWindow(window);
    this->viewerCoronal->SetColorLevel(level);
    this->viewerCoronal->SetColorWindow(window);
    this->viewerSagittal->SetColorLevel(level);
    this->viewerSagittal->SetColorWindow(window);
    this->viewerAxial->Render();
    UpdateWidget();
}
