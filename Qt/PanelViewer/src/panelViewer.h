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

#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>



// Forward Qt class declarations
class vtkImageViewer2;
class vtkRenderer;

namespace Ui {
class panelViewer;
}

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

    virtual void SetSlice(QWidget *widget);
//    virtual void SetSlice_1(int slice);
//    virtual void SetSlice_2(int slice);
//    virtual void SetSlice_3(int slice);
    virtual void SetWindowLevel(int index);

protected:

    void LoadVolume(std::string filename);
    void UpdateWidget();


protected slots:

private:

    vtkSmartPointer<vtkImageViewer2> viewerAxial,
                                    viewerCoronal,
                                    viewerSagittal;
    //    string m_filename;
    vtkSmartPointer<vtkRenderer> render1;
    vtkSmartPointer<vtkRenderer> render2;
    vtkSmartPointer<vtkRenderer> render3;

    // Designer form
    Ui::panelViewer *ui;


};




#endif // SimpleView_H
