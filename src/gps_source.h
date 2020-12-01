#ifndef GPS_SOURCE_H
#define GPS_SOURCE_H

#include <string>
#include <gnuradio/sync_block.h>
#include "gpssim.h"

#define SAMPLE_FREQ         1600000
#define SAMPLES_PER_BUFFER  1
#define SAMPLES_PER_0_1SEC  (SAMPLE_FREQ/10)

using namespace std;

class gps_source;

typedef boost::shared_ptr<gps_source> gps_source_sptr;

gps_source_sptr make_gps_source(const char *nfile, double *loc);


class gps_source : public gr::sync_block
{
    friend gps_source_sptr make_gps_source(const char *nfile, double *loc);

public:
    gps_source(const char *nfile, double *loc);
    ~gps_source();

    int init(void);
    int setLocal(double *loc);

    int work(int noutput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    const char* navfile;

private:
    int allocatedSat[MAX_SAT];
    int dataCnt;
    int dataTransmitted;

    bool s_generateSignal;
    bool s_init;

    FILE *fp;

    int sv;
    int neph, ieph;
    ephem_t eph[EPHEM_ARRAY_SIZE][MAX_SAT];
    gpstime_t g0;

    double llh[3];

    channel_t chan[MAX_CHAN];
    double elvmask;

    int ip, qp;
    int iTable;

    gpstime_t grx;
    double delt;
    int isamp;

    int iumd;
    int numd;
    double xyz[3];

    int nmeaGGA;

    double samp_freq;

    int result;

    int gain[MAX_CHAN];
    double path_loss;
    double ant_gain;
    double ant_pat[37];
    int ibs; // boresight angle index

    datetime_t t0,tmin,tmax;
    gpstime_t gmin,gmax;
    double dt;
    int igrx;

    ionoutc_t ionoutc;

    int allocateChannel(channel_t *chan, ephem_t *eph, ionoutc_t ionoutc,
                         gpstime_t grx, double *xyz, double elvMask);

};

#endif // GPS_SOURCE_H
