#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <gnuradio/analog/sig_source_c.h>
#include <gnuradio/top_block.h>
#include <gnuradio/uhd/usrp_sink.h>
#include <uhd/usrp/multi_usrp.hpp>
#include <string.h>

#include "gps_source.h"
//#include "mnetwork.h"

#include <QTimer>
#include <QString>
class transmitter
{
public:
    transmitter(std::string device_add, const char *loc,std::string path_file);
    ~transmitter();

    void    start();
    void    stop();

    int     gpsSetLocal(const char *loc);
    int     getDayNum(void);

    int     gpsSetNavFile(std::string navfile);

private:
    double                      llh[3];
    bool                        d_running;

    gr::top_block_sptr          tb;
    gps_source_sptr             gps_s;
    gr::uhd::usrp_sink::sptr    uhdSink;

};

#endif // TRANSMITTER_H
