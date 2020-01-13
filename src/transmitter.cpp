#include "transmitter.h"

#include <qdebug.h>
#include <QDebug>
#include <stdlib.h>
#include <time.h>

transmitter::transmitter(std::string device_add, const char *loc,std::string path_file)
    : d_running(false)
{

    tb = gr::make_top_block("application");

    uhdSink = gr::uhd::usrp_sink::make(device_add, uhd::stream_args_t("fc32"));
    uhdSink->set_clock_source("external");
    uhdSink->set_samp_rate(SAMPLE_FREQ);
    uhdSink->set_center_freq(1575420000);
    uhdSink->set_gain(50);

    sscanf(loc,"%lf,%lf,%lf",&llh[0], &llh[1], &llh[2]);
    gps_s = make_gps_source("hourly.n",llh);
    qDebug() << "LAT: " << llh[0] << " LON: " << llh[1] << " HIG: " << llh[2];

    tb->connect(gps_s,0,uhdSink,0);

}

transmitter::~transmitter()
{

}

void transmitter::start()
{
    if(!d_running)
    {
//        gps_s->init("/home/nutaq/gps-sdr-sim/brdc2290.16n",llh);
        tb->start();
        d_running = true;
    }
    qDebug() << "Started...";
}

void transmitter::stop()
{
    if(d_running)
    {
        tb->stop();
        tb->wait();
        d_running = false;
    }
}

int transmitter::gpsSetLocal(const char *loc)
{
   qDebug() << "UPDATE GPS LOCAL...";
    sscanf(loc,"%lf,%lf,%lf", &llh[0], &llh[1], &llh[2]);
    return gps_s->setLocal(llh);
}

int transmitter::getDayNum(void)
{
    time_t timer;
    struct tm *gmt;

    time(&timer);
    gmt = gmtime(&timer);

    qDebug() << "Time " << gmt->tm_mday << "/" << gmt->tm_mon + 1 << "  hour " << gmt->tm_hour << ":" << gmt->tm_min;
    gmt->tm_mday = 1;
    gmt->tm_mon = 0;

    return (int)(difftime(timer,mktime(gmt))/(3600*24) + 1);
}

int transmitter::gpsSetNavFile(std::string navfile)
{
    gps_s->navfile = navfile.c_str();
    return 0;
}

