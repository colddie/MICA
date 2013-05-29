/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

#include "ui_Viewer.h"
#include "Viewer.h"
#include "IO_type.h"
#include "toolBase.h"

#include <QFileDialog>
#include <QDir>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMetaImageIO.h"
#include "itkTextOutput.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkMetaImageReader.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkThreshold.h"
#include "vtkVolumeProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDataSetTriangleFilter.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolume.h"
#include "vtkWin32OutputWindow.h"


//#include <vtkDataObjectToTable.h>
//#include <vtkElevationFilter.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkQtTableView.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkVectorText.h>
//#include "vtkSmartPointer.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()



// Constructor
//Viewer::Viewer(int windowhigh, int windowlow, string filename)
Viewer::Viewer()
{
  this->ui = new Ui_Viewer;
  this->ui->setupUi(this);

    m_windowhigh = 400;
    m_windowlow = 40;
//    m_windowhigh = windowhigh;
//    m_windowlow = windowlow;
//    m_filename = filename;

    // Text browser
//    QPalette p = this->ui->textBrowser->palette();
//    p.setColor(QPalette::Background, Qt::cyan);
//    this->ui->textBrowser->setPalette(p);

    // Slider 1st
    this->ui->horizontalSlider1->setMinimum(-1024);
    this->ui->horizontalSlider1->setMaximum(3024);
    this->ui->horizontalSlider1->setSingleStep(10);
    this->ui->horizontalSlider1->setPageStep(100);
    this->ui->horizontalSlider1->setValue(40);

    // Slider 2nd
    this->ui->horizontalSlider2->setMinimum(-1024);
    this->ui->horizontalSlider2->setMaximum(3024);
    this->ui->horizontalSlider2->setSingleStep(10);
    this->ui->horizontalSlider2->setPageStep(100);
    this->ui->horizontalSlider2->setValue(400);

    // Text label
    this->ui->label1->setText(QString::number(40));
    this->ui->label2->setText(QString::number(400));

  // Qt Table View
//  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form

//   this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

    // Just a bit of Qt interest: Culling off the
    // point data and handing it to a vtkQtTableView
//    VTK_CREATE(vtkDataObjectToTable, toTable);
//    toTable->SetInputConnection(elevation->GetOutputPort());
//    toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

    // Force output to stdout
    // On Windows systems, the output of warnings is normally sent to a
    // window and not the shell
    itk::OutputWindow::SetInstance(itk::TextOutput::New());

    itkGenericOutputMacro(<< "This should appear in the console window, not in a pop up window");

    VTK_CREATE(vtkWin32OutputWindow, Debug);
    vtkOutputWindow::SetInstance(Debug);

    //    vtkGenericWarningMacro(<<"vtk debug is here");
    \
  // Set up action signals and slots
    connect(this->ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->horizontalSlider1, SIGNAL(valueChanged(int)), this, SLOT(slotSlider1Change()));
    connect(this->ui->horizontalSlider2, SIGNAL(valueChanged(int)), this, SLOT(slotSlider2Change()));
};

Viewer::~Viewer()
{
  // The smart pointers should clean up for up

}


void Viewer::Initialize()
{
    // Here we take the end of the VTK pipeline and give it to a Qt View
//    this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());


    typedef itk::Image< unsigned short, 3 >    	 ImageType;

    // Declare a filter
    typedef itk::ImageToVTKImageFilter< ImageType >   FilterType;
    FilterType::Pointer connector = FilterType::New();

    // Declare a reader
    typedef itk::ImageFileReader< ImageType > VolumeReaderType;
    VolumeReaderType::Pointer reader = VolumeReaderType::New();
    reader->SetFileName( m_filename.c_str() );
    reader->Update();

    //  Connect itk with vtk
    connector->SetInput( reader->GetOutput() );
    connector->Update();
    VTK_CREATE(vtkImageData, imageData);
    imageData->DeepCopy( connector->GetOutput() );   // must copy mem or read empty

//        VTK_CREATE(vtkMetaImageReader, reader);
//        reader->SetFileName(m_filename.c_str());
//        imageData = reader->GetOutput();

    //  Declare vtk renderwindow
//    VTK_CREATE(vtkRenderer, ren1);
    ren1 = vtkRenderer::New();

    // Note that funny arithematic on the
    // SetDesiredUpdateRate - the vtkRenderWindow divides it
    // allocated time across all renderers, and the renderer
    // divides it time across all props. If clip is
    // true then there are two props
    //iren->SetDesiredUpdateRate(frameRate / (1+clip) );

//     Smooth a little before clipping
//    VTK_CREATE(vtkImageGaussianSmooth, smooth);
//    smooth->SetDimensionality(3);
//    smooth->SetStandardDeviation(1.0, 1.0, 1.0);
//    smooth->SetInput(imageData);
//    imageData = smooth->GetOutput();


    //  Raycast mapper
    VTK_CREATE(vtkVolumeRayCastCompositeFunction, compositeFunction);
    VTK_CREATE(vtkVolumeRayCastMapper, volumeMapper);
    volumeMapper->SetVolumeRayCastFunction( compositeFunction );
    //    volumeMapper->SetInput( connector->GetOutput() );
    //    volumeMapper->SetInput(smooth->GetOutput());
    volumeMapper->SetInput(imageData);
    volumeMapper->SetSampleDistance(1);
    volumeMapper->SetBlendModeToComposite();
    volumeMapper->Update();       // must

    //  Set mapper and volumeproperty to volume
//    VTK_CREATE(vtkVolume, volume);
    volume = vtkVolume::New();
    volume->SetMapper( volumeMapper ) ;

//    SetVolumeProperty();
}


void Viewer::SetVolumeProperty()
{
    //  Define false color
    VTK_CREATE(vtkColorTransferFunction, colorTransferFunction);
    colorTransferFunction->AddRGBPoint(-1024, 0, 0, 0);
    colorTransferFunction->AddRGBPoint(m_windowlow, .62, .36, .18);
    colorTransferFunction->AddRGBPoint(m_windowhigh, .88, .60, .29);
    //colorTransferFunction->AddRGBPoint(windowhigh+1, .0, .0, 0);
    colorTransferFunction->ClampingOff();

    //  Define opacity
    VTK_CREATE(vtkPiecewiseFunction, opacityTransferFunction);
//    opacityTransferFunction->AddPoint(-1024,0);
    opacityTransferFunction->AddPoint(m_windowlow, 0);
    opacityTransferFunction->AddPoint(m_windowhigh, 1);
    //opacityTransferFunction->AddPoint(windowhigh+1, 0);
//    opacityTransferFunction->AddPoint(-1024,m_windowlow,(m_windowlow-1024)/2,1);
//    opacityTransferFunction->AddPoint(m_windowlow,m_windowhigh,(m_windowhigh-m_windowlow)/2,0);
//    opacityTransferFunction->AddPoint(m_windowhigh,2000,(2000-m_windowhigh)/2,1);
    opacityTransferFunction->ClampingOff();
    opacityTransferFunction->Update();

    //
    //vtkPiecewiseFunction *gradientTransferFunction = vtkGradientTransferFunction::New();
    //gradientTransferFunction->AddPoint();

    //  Define volumeproperty
    VTK_CREATE(vtkVolumeProperty, volumeProperty);
    volumeProperty->SetColor( colorTransferFunction );
    volumeProperty->SetScalarOpacity( opacityTransferFunction );
    // volumeProperty->SetGradientOpacity();
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();
    //  volumeProperty->SetAmbient(0.4);
    //  volumeProperty->SetDiffuse(0.6);
    //  volumeProperty->SetSpecular(0.2);

    volume->SetProperty ( volumeProperty );
    volume->Update();
}


void Viewer::UpdateWidget()
{
    //  Add volume to renderwindow
    ren1->AddVolume( volume );
    ren1->SetBackground(0.1,0.21,0.31);

    // VTK/Qt wedded
    this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren1);
    this->ui->qvtkWidget->update();
}


// Slot functions
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
     QString dimensionSize = "DimensionSize: "+QString(vectorToString<unsigned int>(io.Return_DimensionSize()).c_str());
     QString spacing = "Spacing: "+QString(vectorToString<double>(io.Return_Spacing()).c_str());
     QString origin = "Origin: "+QString(vectorToString<double>(io.Return_Origin()).c_str());
     QString directionX = "Direction Matrix:"+QString(vectorToString<double>(io.Return_Direction(0)).c_str());
     QString directionY = "\t"+QString(vectorToString<double>(io.Return_Direction(1)).c_str());
     QString directionZ = "\t"+QString(vectorToString<double>(io.Return_Direction(2)).c_str());

     QString toTextBrowser = byteOrder+QString("\n")+imageType+"\n"+pixelType+"\n"+dimensionNumber
             +"\n"+channelNumber+"\n"+dimensionSize+"\n"+spacing+"\n"+origin+"\n"
             +directionX+"\n"+directionY+"\n"+directionZ;
     this->ui->textBrowser->setText(toTextBrowser);

     Initialize();
     SetVolumeProperty();
     UpdateWidget();

 }

 void Viewer::slotExit()  {
   qApp->exit();
 }


 void Viewer::slotSlider1Change()
 {
     // Prevent over slide
     m_windowlow = this->ui->horizontalSlider1->value();
     if (m_windowlow > this->ui->horizontalSlider2->value()) {
         m_windowlow = m_windowhigh;
         this->ui->horizontalSlider1->setSliderPosition(m_windowlow);
         return;
     }

     // Update vtk pipeline
     this->ui->label1->setText(QString::number(m_windowlow));
     SetVolumeProperty();
     UpdateWidget();

 }


 void Viewer::slotSlider2Change()
 {
     // Prevent over slide
     m_windowhigh = this->ui->horizontalSlider2->value();
     if (this->ui->horizontalSlider1->value() > m_windowhigh) {
         m_windowhigh = m_windowlow;
         this->ui->horizontalSlider2->setSliderPosition(m_windowhigh);
         return;
     }

     // Update vtk pipeline
     this->ui->label2->setText(QString::number(m_windowhigh));
     SetVolumeProperty();
     UpdateWidget();

 }
