#ifndef RUNAPP_H
#define RUNAPP_H

#include "transmitter.h"
#include "gpssim.h"

#include <QTimer>
#include <QObject>
#include <QtCore>
#include <QString>
#include <QFile>
//#include <QDialog>
//#include <QLabel>
//#include <QWidget>


class runapp : public QObject
{
    Q_OBJECT
public:
    runapp(QObject *parent = 0);
    ~runapp();
    void start_device();

private slots:
    void runSchedule();

private:
    std::string local1;
    std::string local2;
    std::string local3;
    std::string strIpAdd;
    std::string temp_local1;
    std::string temp_local2;
    std::string Path_File;

    double temp_la1;
    double temp_lo1;
    double temp_h1;
    double temp_la2;
    double temp_lo2;
    double temp_h2;

    QString readConfigFile;

    transmitter *tran;
    QFileSystemWatcher *watcher;
};

#endif // RUNAPP_H
