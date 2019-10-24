#include "filechanged.h"
#include <QFileSystemWatcher>
#include "main.h"

FileChanged::FileChanged()
{
    watcher = new QFileSystemWatcher();
    watcher->addPath("/home/mcr/Desktop/map_control_drone/views/truyen_du_lieu.json");
    connect(watcher, SIGNAL(fileChanged (QString)), this, SLOT(runSchedule()));
}

