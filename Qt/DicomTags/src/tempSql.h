#ifndef TEMPSQL_H
#define TEMPSQL_H


#include <QObject>
#include <QSqlDatabase>


//class TagData
//{

//public:
//    TagData();
//    ~TagData();

//    QString TagID;
//    QString Name;

//};



class DatabaseManager : public QObject
{
public:
    DatabaseManager(QObject *parent = 0) : TagID(""), Name("")
    {};
    ~DatabaseManager(){};

public:
    bool openDB();
    bool getTag(QString str);

    QString TagID;
    QString Name;

private:
    //        QSqlDatabase db;
};











#endif // TEMPSQL_H
