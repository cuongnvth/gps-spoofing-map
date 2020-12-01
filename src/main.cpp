#include "runapp.h"
#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QString>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>
#include <cstdlib>
#include <QObject>
#include <QJsonObject>
#include "transmitter.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("../src/mapserver.sh");sleep(1);
   system("../src/gethourly.sh");
    runapp w;
    w.start_device();
    return a.exec();
}

