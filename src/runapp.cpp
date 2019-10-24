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
    QObject(parent),
    start_condition(false),
    downloadfile(true)
{
    watcher = new QFileSystemWatcher();
    watcher->addPath("../mapserver/views/truyen_du_lieu.json");
    QObject::connect(watcher, SIGNAL(fileChanged (QString)), this, SLOT(runSchedule()));
}

runapp::~runapp(){}

void runapp::runSchedule() // Ham chay chinh
{
    //qDebug() << "File change...";
    QString settings;
    QString state;
    QString state_dowload;
    double heigh;
    double t_lat;
    double t_lon;
    QFile file("../mapserver/views/truyen_du_lieu.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();
    QJsonDocument settdoc = QJsonDocument::fromJson(settings.toUtf8());
    //qDebug() << settdoc.isNull();
    QJsonObject sett2 = settdoc.object();
    //qDebug() << sett2.value(QString("cao"));  // <- print my title

    t_lat = sett2.value(QString("lat")).toDouble();
    t_lon = sett2.value(QString("lng")).toDouble();
    heigh = sett2.value(QString("cao")).toDouble();
    state = sett2.value(QString("start_stop")).toString();
    state_dowload = sett2.value(QString("download_file")).toString();

    if(state_dowload == "dow" && downloadfile == true){
        system("../src/gethourly.sh");
        downloadfile = false;
    }
    if(state_dowload == "no_dow"){
        downloadfile = true;
    }

    if(state == "start"){
        start_device();
    }

    if(state == "start" && start_condition == true && (temp_la2!=t_lat || temp_lo2!=t_lon)){
        qDebug() << "co update";

        temp_la2 = t_lat;
        temp_lo2 = t_lon;

        local2=QString::number(temp_la2).toStdString()+","+
                QString::number(temp_lo2).toStdString()+","+QString::number(heigh).toStdString();

       tran->gpsSetLocal(local2.c_str());

    }

    if(state == "stop" && start_condition == true){
        tran->stop();
        start_condition = false;
    }
}


//*************************************************************************
void runapp::start_device(){ // Ham phat gps
    if(!start_condition){
        local1="10.816883, 106.658084,5"; //SBTSN
        strIpAdd="serial=31703F3";//B205-mini
        //strIpAdd="addr=192.168.10.2";
        local2 = local1;
        tran = new transmitter(strIpAdd.c_str(),local1.c_str(),"hourly.n");
        tran->start();
        start_condition = true;
        }

}
//*************************************************************************
