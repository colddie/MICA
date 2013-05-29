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
#ifndef Viewer_H
#define Viewer_H

#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>

using namespace std;

// Forward Qt class declarations
class Ui_Viewer;
class vtkRenderer;
class vtkVolume;

// Forward VTK class declarations
//class vtkQtTableView;


class Viewer : public QMainWindow
{
  Q_OBJECT

public:

    // Constructor/Destructor
    //    Viewer(int windowhigh, int windowlow, string filename);
    Viewer();
    ~Viewer();


signals:


public slots:

    virtual void slotOpenFile();
    virtual void slotExit();
    virtual void slotSlider1Change();
    virtual void slotSlider2Change();

protected:

    void Initialize();
    void SetVolumeProperty();
    void UpdateWidget();

    vtkSmartPointer<vtkRenderer> ren1;
    vtkSmartPointer<vtkVolume> volume;

protected slots:

private:

    int m_windowhigh;
    int m_windowlow;
    string m_filename;

    //  vtkSmartPointer<vtkQtTableView>         TableView;
    
    // Designer form
    Ui_Viewer *ui;


};

#endif // SimpleView_H
