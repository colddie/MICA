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

#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkMetaImageReader.h"
#include "vtkDataSetTriangleFilter.h"
#include "vtkSmartPointer.h"
#include "vtkWin32OutputWindow.h"
#include "QVTKWidget.h"



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()



// Constructor
//Viewer::Viewer(int windowhigh, int windowlow, string filename)
Viewer::Viewer(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::panelViewer)
{
//    this->ui = new Ui::panelViewer;
    this->ui->setupUi(this);

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
    this->render1 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerAxial = vtkSmartPointer<vtkImageViewer2>::New();
    this->ui->qvtkWidget_1->GetRenderWindow()->AddRenderer(render1);


    this->render2 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerCoronal = vtkSmartPointer<vtkImageViewer2>::New();
    this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(render2);


    this->render3 = vtkSmartPointer<vtkRenderer>::New();
    this->viewerSagittal = vtkSmartPointer<vtkImageViewer2>::New();
    this->ui->qvtkWidget_3->GetRenderWindow()->AddRenderer(render3);


    // Text label
    //    this->ui->label1->setText(QString::number(40));
    //    this->ui->label2->setText(QString::number(400));

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
//    connect(this->ui->verticalSlider_1, SIGNAL(valueChanged(int)), this, SLOT(SetSlice_1(int)));
//    connect(this->ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(SetSlice_2(int)));
//    connect(this->ui->verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(SetSlice_3(int)));
};

Viewer::~Viewer()
{
  // The smart pointers should clean up for up

}


void Viewer::LoadVolume(std::string filename)
{
    // Here we take the end of the VTK pipeline and give it to a Qt View
//    this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());


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

    VTK_CREATE(vtkMetaImageReader, reader);
    reader->SetFileName(filename.c_str());
    reader->Update();

    // Slider
    int *ip = reader->GetDataExtent();
    unsigned int imageWidth = ip[1]-ip[0]+1;
    unsigned int imageHeight = ip[3]-ip[2]+1;
    unsigned int numSlices = ip[5]-ip[4]+1;
    this->ui->verticalSlider_1->setRange(0, numSlices-1);
    this->ui->verticalSlider_2->setRange(0, imageHeight-1);
    this->ui->verticalSlider_3->setRange(0, imageWidth-1);




    //  Declare vtk renderwindow
    this->viewerAxial->SetInput(reader->GetOutput());
    //    VTK_CREATE(vtkCamera, camera);

    this->viewerAxial->UpdateDisplayExtent();
    this->viewerAxial->SetupInteractor(ui->qvtkWidget_1->GetRenderWindow()->GetInteractor());
    this->viewerAxial->SetRenderWindow(this->ui->qvtkWidget_1->GetRenderWindow());
    this->viewerAxial->SetRenderer(render1);
    this->viewerAxial->GetRenderer()->ResetCamera();
    //  Filp y axis
    this->viewerAxial->GetRenderer()->GetActiveCamera()->SetRoll(180.0);
    this->viewerAxial->SetSlice(10);
    this->viewerAxial->Render();

    this->viewerCoronal->SetInput(reader->GetOutput());
    this->viewerCoronal->SetSliceOrientationToXZ();
    this->viewerCoronal->UpdateDisplayExtent();
    this->viewerCoronal->SetupInteractor(this->ui->qvtkWidget_2->GetRenderWindow()->GetInteractor());
    this->viewerCoronal->SetRenderWindow(this->ui->qvtkWidget_2->GetRenderWindow());
    this->viewerCoronal->SetRenderer(render2);
    this->viewerCoronal->GetRenderer()->ResetCamera();
    this->viewerCoronal->SetSlice(10);
    this->viewerCoronal->Render();

    this->viewerSagittal->SetInput(reader->GetOutput());
    this->viewerSagittal->SetSliceOrientationToYZ();
    this->viewerSagittal->UpdateDisplayExtent();
    this->viewerSagittal->SetupInteractor(this->ui->qvtkWidget_3->GetRenderWindow()->GetInteractor());
    this->viewerSagittal->SetRenderWindow(this->ui->qvtkWidget_3->GetRenderWindow());
    this->viewerSagittal->SetRenderer(render3);
    this->viewerSagittal->GetRenderer()->ResetCamera();
    this->viewerSagittal->SetSlice(10);
    this->viewerSagittal->Render();

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


// Slot functions
 void Viewer::slotOpenFile()
 {
     QString filename = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::currentPath(), tr("Image Files (*.mha *.jpg *.bmp)"));

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
             UpdateWidget(); qDebug() << slice << "x";
         }

         else if(!QString::compare(slider->objectName(),"verticalSlider_2")) {
             this->viewerCoronal->SetSlice(slice);
             UpdateWidget(); qDebug() << slice << "x";
         }

         else if(!QString::compare(slider->objectName(),"verticalSlider_3")) {
             this->viewerSagittal->SetSlice(slice);
             UpdateWidget(); qDebug() << slice << "x";
         }
         else
             return;
     }

 }

// void Viewer::SetSlice_1(int slice)
// {
//     this->viewerAxial->SetSlice(slice);
//     UpdateWidget();
// }

// void Viewer::SetSlice_2(int slice)
// {
//     this->viewerCoronal->SetSlice(slice);
//     UpdateWidget();
// }

// void Viewer::SetSlice_3(int slice)
// {
//     this->viewerSagittal->SetSlice(slice);
//     UpdateWidget();
// }


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

    case 3:  // extremities
        level = 0;
        window = 400;
        break;
    case 4:  // liver

    case 5:  // lung
        level = -300;
        window = 400;
        break;
    case 6:  // pelvis

    case 7:  // skull
        level = 1000;
        window = 400;
        break;
    case 8:  // spline 1

    case 9:  // spline 2

    case 10: // thorax

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


// void Viewer::slotSlider1Change()
// {
//     // Prevent over slide
//     m_windowlow = this->ui->verticalSlider_1->value();
//     if (m_windowlow > this->ui->verticalSlider_2->value()) {
//         m_windowlow = m_windowhigh;
//         this->ui->verticalSlider_1->setSliderPosition(m_windowlow);
//         return;
//     }

//     // Update vtk pipeline
//     this->ui->label1->setText(QString::number(m_windowlow));
//     SetVolumeProperty();
//     UpdateWidget();

// }


// void Viewer::slotSlider2Change()
// {
//     // Prevent over slide
//     m_windowhigh = this->ui->verticalSlider_2->value();
//     if (this->ui->verticalSlider_1->value() > m_windowhigh) {
//         m_windowhigh = m_windowlow;
//         this->ui->verticalSlider_2->setSliderPosition(m_windowhigh);
//         return;
//     }

//     // Update vtk pipeline
//     this->ui->label2->setText(QString::number(m_windowhigh));
//     SetVolumeProperty();
//     UpdateWidget();

// }
