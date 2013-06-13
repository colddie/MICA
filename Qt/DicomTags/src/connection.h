/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query(db);
    query.exec("create table person (Tags int primary key, "
               "Name varchar(20), Value varchar(20))");
//    query.exec("insert into person values(101, 'Danny', 'Young')");
//    query.exec("insert into person values(102, 'Christine', 'Holand')");
//    query.exec("insert into person values(103, 'Lars', 'Gordon')");
//    query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
//    query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");



    return true;
}
//! [0]

#endif
