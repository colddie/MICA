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
#ifndef _DicomTags_H
#define _DicomTags_H

#include "itkImageSeriesReader.h"
#include "itkMetaDataDictionary.h"

#include <QMainWindow>
#include <QtSql>
#include <QMap>

// Forward vtk Qt class declarations




// Add mainwindow to ui struct
namespace Ui {
class dicomTags;
}

//
class Viewer : public QMainWindow
{
  Q_OBJECT

public:

    // Constructor/Destructor
    Viewer(QMainWindow *parent = 0);
    ~Viewer();

    void ReadDicom(const std::string dir);
    void DisplayTag();

signals:


public slots:
    virtual void slotOpenFile();
    virtual void slotChangeTag(const QModelIndex &topLeft,
                               const QModelIndex &bottomRight);
    virtual void slotApplyChange();
    virtual void slotGetCurrentFile(const QModelIndex &fileIndex);
protected:


protected slots:

private:

    Ui::dicomTags *ui;
    QSqlTableModel *model;
    QMap<QString, QString> toChange;

    typedef signed short PixelType;
    typedef itk::Image<PixelType, 3> ImageType;
    typedef itk::ImageSeriesReader<ImageType> ReaderType;
    ReaderType::Pointer reader;

    typedef itk::MetaDataDictionary DictionaryType;
    DictionaryType m_dictionary;
};





#endif // DicomTags_H
