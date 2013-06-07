/*
 * Copyright 2013 Julio Sun.
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

// QT includes
#include <QApplication>
//#include <QCleanlooksStyle>

#include "panelViewer.h"
//#include "argumentsParse.h"

extern int qInitResources_icons();


int main( int argc, char** argv )
{


    // QT Stuff
    QApplication app( argc, argv );

    //QApplication::setStyle(new QCleanlooksStyle);

    qInitResources_icons();

//    Viewer mySimpleView(windowhigh, windowlow, filename);
    Viewer *mySimpleView = new Viewer();
//    mySimpleView.Run();
    mySimpleView->show();

    return app.exec();
}
