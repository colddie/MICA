/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SimpleView.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright 2013 Julio Sun.
  Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef _PanelViewer_H
#define _PanelViewer_H

#include "vtkSmartPointer.h"    // Required for smart pointer internal vars.
#include "QVTKInteractor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleImage.h"
#include "vtkLineSource.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"

#include <QMainWindow>


// Forward vtk Qt class declarations
class vtkImageViewer2;
class vtkRenderer;
class vtkObject;
class vtkCommand;
class vtkEventQtSlotConnect;
class vtkRenderWindowInteractor;



// Add mainwindow to ui struct
namespace Ui {
class panelViewer;
}

//
class Viewer : public QMainWindow
{
  Q_OBJECT

public:

    // Constructor/Destructor
    //    Viewer(int windowhigh, int windowlow, string filename);
    Viewer(QMainWindow *parent = 0);
    ~Viewer();


signals:


public slots:

    virtual void slotOpenFile();
    virtual void slotExit();
    virtual void ResetView();
    virtual void EnableCrosshair(bool clicked);
    virtual void SetSlice(QWidget *widget);
    virtual void SetWindowLevel(int index);

//    virtual void vtkEventCallBack(vtkObject *caller, unsigned long vtkEvent,
//                                  void *clientData, void *callData, vtkCommand *command);

    void mouseEnterCallback_1();
    void mouseEnterCallback_2();
    void mouseEnterCallback_3();
    void mouseLeaveCallback();

    void mouseWheelForwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*);
    void mouseWheelBackwardCallback(vtkObject*,ulong,void*,void*,vtkCommand*);
    void mouseMoveCallback(vtkObject*,ulong,void*,void*,vtkCommand*);
    void mouseClickCallback(vtkObject*,ulong,void*,void*,vtkCommand*);


protected:

    void LoadVolume(std::string filename);

    void UpdateWidget();
    void AddCrosshair();
    void RedrawCrosshair();

    template <typename T>
    void GetIntensity();

protected slots:

private:

    vtkSmartPointer<vtkImageViewer2> viewerAxial,
                                    viewerCoronal,
                                    viewerSagittal;
    //    string m_filename;
    //    vtkSmartPointer<vtkRenderer> render1;
    //    vtkSmartPointer<vtkRenderer> render2;
    //    vtkSmartPointer<vtkRenderer> render3;

    // must declare as class member instead define in the function
    vtkSmartPointer<vtkEventQtSlotConnect> connection;

    // Designer form
    Ui::panelViewer *ui;

    //
    QList<unsigned int> imageDimension;
    QList<double> imageSpacing;
    QVector<unsigned int> currentSlice;
    QVector<double> currentPosition;
    QString currentWidget;
    QList<vtkSmartPointer<vtkLineSource> > horizontalLine;
    QList<vtkSmartPointer<vtkLineSource> > verticalLine;
    QList<vtkSmartPointer<vtkPolyDataMapper> > horizontalLineMapper;
    QList<vtkSmartPointer<vtkPolyDataMapper> > verticalLineMapper;
    QList<vtkSmartPointer<vtkActor> > horizontalLineActor;
    QList<vtkSmartPointer<vtkActor> > verticalLineActor;

    //
    bool loadImage;
    bool enabledCrosshair;
    //    bool mouseOverWidget;

};



// Crosshair implementation
//class Crosshair
//{
//};


// Overwrite default interaction style
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera// Or vtkInteractorImage
{
  public:
    static MouseInteractorStyle* New();
    vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
      std::cout << "Pressed left mouse button." << std::endl;
      // Forward events
//      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

//    virtual void OnMiddleButtonDown()
//    {
//      std::cout << "Pressed middle mouse button." << std::endl;
//      // Forward events
//      // vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
//    }

//    virtual void OnRightButtonDown()
//    {
//      std::cout << "Pressed right mouse button." << std::endl;
//      // Forward events
//      vtkInteractorStyleTrackballCamera::OnRightButtonDown();
//    }

    virtual void OnMouseWheelForward()
    {
        std::cout << "Forward scroll mouse wheel." << std::endl;
        // Forward events
//        vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    }

    virtual void OnMouseWheelBackward()
    {
        std::cout << "Forward scroll mouse wheel." << std::endl;
        // Forward events
//        vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    }

};

#endif // SimpleView_H
