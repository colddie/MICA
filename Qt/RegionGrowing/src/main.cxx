/*
 * Copyright 2013 Julio Sun.
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

// QT includes
#include <QApplication>
//#include <QCleanlooksStyle>

#include "Viewer.h"
#include "argumentsParse.h"

extern int qInitResources_icons();


int main( int argc, char** argv )
{


//    vector<int> window;
//    int windowhigh, windowlow;
//    string filename;

//    const char *help[] = {
//        "Usage: render a 3D image, double click to open"
//    };

//    ArgumentParser ap;
//    ap.SetCommandLineArg(argc, argv, help);
////    ap.MarkAsRequired("-window");
////    ap.MarkAsRequired("-input");
//    if (!ap.IsRequiredFound()) { exit(1); }
//    window = ap.MultiParse<int>("-window", "isnum");
//    windowlow = window[0];
//    windowhigh = window[1];
//    filename = ap.OneParse("-input");

    // QT Stuff
    QApplication app( argc, argv );

    //QApplication::setStyle(new QCleanlooksStyle);

//    qInitResources_icons();

    Viewer mySimpleView;
    mySimpleView.show();
//    Viewer mySimpleView(windowhigh, windowlow, filename);
//    Viewer mySimpleView;
//    mySimpleView.Run();
//    mySimpleView.show();

    return app.exec();
}
