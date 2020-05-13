#include "runapp.h"
#include <QFileSystemWatcher>
#include <QDebug>
#include <QString>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <QPainter>
#include <QGraphicsItem>
#include <QPalette>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QByteArray>
#include <QJsonDocument>
#include <QProcess>
#include <cstdlib>

runapp::runapp(QObject *parent) :
    QObject(parent)
{
    watcher = new QFileSystemWatcher();
    watcher->addPath("../mapserver/views/truyen_du_lieu.json");
    QObject::connect(watcher, SIGNAL(fileChanged (QString)), this, SLOT(runSchedule()));
}

runapp::~runapp(){}

void runapp::runSchedule() // Ham chay chinh
{
    qDebug() << "File change...";
    
    QString state;
    double t_hi;
    double t_lat;
    double t_lng;


    QFile file("../mapserver/views/truyen_du_lieu.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    readConfigFile = file.readAll();
    file.close();
    QJsonDocument newLocation = QJsonDocument::fromJson(readConfigFile.toUtf8());
    QJsonObject sett = newLocation.object();
    t_lat = sett.value(QString("lat")).toDouble();
    t_lng = sett.value(QString("lng")).toDouble();
    t_hi = sett.value(QString("hi")).toDouble();
    state = sett.value(QString("start_stop")).toString();

    if(state == "start"){
        qDebug() << "Power Start";
        tran->start();
        if(temp_la2!=t_lat || temp_lo2!=t_lng){
            qDebug() << "Update Location";
            temp_la2 = t_lat;
            temp_lo2 = t_lng;
            temp_h2 = t_hi;
            local2=QString::number(temp_la2).toStdString()+","+
                    QString::number(temp_lo2).toStdString()+","+QString::number(temp_h2).toStdString();

           tran->gpsSetLocal(local2.c_str());
       }
    }

    if(state == "stop"){
        qDebug() << "Power Stop";
        tran->stop();
    }
}


//*************************************************************************
void runapp::start_device(){ // Ham phat gps

    QFile file("../mapserver/views/truyen_du_lieu.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    readConfigFile = file.readAll();
    file.close();
    qWarning() << readConfigFile;
    if(readConfigFile.isNull() || readConfigFile.isEmpty()){
        local1="10.816883, 106.658084,5"; //SBTSN
        qDebug() << "\nNull File\n";

    }
    else{
        QJsonDocument defaultLocation = QJsonDocument::fromJson(readConfigFile.toUtf8());
        QJsonObject setLocation = defaultLocation.object();
        temp_la1 = setLocation.value(QString("def_lat")).toDouble();
        temp_lo1 = setLocation.value(QString("def_lng")).toDouble();
        temp_h1 = setLocation.value(QString("def_hi")).toDouble();
        local1 = QString::number(temp_la1).toStdString()+","+
                QString::number(temp_lo1).toStdString()+","+QString::number(temp_h1).toStdString();
        qDebug() << "Data File";

    }

    local2 = local1;
//    strIpAdd="serial=31703F3";//B205-mini
//    strIpAdd="serial=30BC5FT";//B210
//    strIpAdd="addr=192.168.10.2";
//    tran = new transmitter(strIpAdd.c_str(),local1.c_str(),"hourly.n");
//    tran->start();

}
//*************************************************************************
