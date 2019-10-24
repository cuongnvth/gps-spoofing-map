#include "gps_source.h"
#include <qdebug.h>
#include <gnuradio/io_signature.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#ifdef _WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif

static int sinTable512[] = {
       2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
      50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
      97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
     140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
     178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
     209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
     232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
     245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250,
     250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
     245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
     230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
     207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
     176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
     138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
      94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
      47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
      -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
     -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
     -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
    -140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
    -178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
    -209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
    -232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
    -245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
    -250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
    -245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
    -230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
    -207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
    -176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
    -138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
     -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
     -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2
};

static int cosTable512[] = {
     250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
     245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
     230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
     207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
     176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
     138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
      94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
      47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
      -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
     -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
     -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
    -140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
    -178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
    -209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
    -232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
    -245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
    -250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
    -245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
    -230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
    -207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
    -176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
    -138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
     -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
     -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2,
       2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
      50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
      97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
     140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
     178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
     209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
     232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
     245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250
};

// Receiver antenna attenuation in dB for boresight angle = 0:5:180 [deg]
static double ant_pat_db[37] = {
     0.00,  0.00,  0.22,  0.44,  0.67,  1.11,  1.56,  2.00,  2.44,  2.89,  3.56,  4.22,
     4.89,  5.56,  6.22,  6.89,  7.56,  8.22,  8.89,  9.78, 10.67, 11.56, 12.44, 13.33,
    14.44, 15.56, 16.67, 17.78, 18.89, 20.00, 21.33, 22.67, 24.00, 25.56, 27.33, 29.33,
    31.56
};

gps_source_sptr make_gps_source(const char *nfile, double *loc)
{
    return gnuradio::get_initial_sptr(new gps_source(nfile,loc));
}

gps_source::gps_source(const char *nfile, double *loc)
    : gr::sync_block("gps_source",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      elvmask(0.0),
      nmeaGGA(FALSE),
      dataCnt(0),
      dataTransmitted(false),
      s_generateSignal(true),
      navfile(nfile),
      s_init(true),
      samp_freq(SAMPLE_FREQ)
{
    /*! ----------------------------------------------------------------------------
     *  Receiver position
     *  ----------------------------------------------------------------------------*/
    llh[0] = loc[0] / R2D;
    llh[1] = loc[1] / R2D;
    llh[2] = loc[2];
    //llh[2] = 100;
    llh2xyz(llh,xyz);
}

gps_source::~gps_source()
{

}

int gps_source::init()
{
    /*! -------------------------------------------------------------------------
     *  Read options
     *  -------------------------------------------------------------------------*/

    // Default options
    qDebug() << "Init GPS block";

//    samp_freq = 2500000;
    g0.week = -1; // Invalid start time
    ionoutc.enable = FALSE;

    if(navfile[0] == 0)
    {
        qDebug() << "ERROR: GPS ephemeris file is not specified.";
        return 0;
    }

    delt = 1.0/samp_freq;

    /*! ----------------------------------------------------------------------------
     *  Read ephemeris
     *  ----------------------------------------------------------------------------*/

//    neph = readRinexNavAll(eph, &ionoutc, navfile);

//    if(neph <= 0)
//    {
//        qDebug() << "ERROR: No ephemeris available.";
//        return 0;
//    }

//    for(sv = 0; sv < MAX_SAT; sv++)
//    {
//        if(eph[0][sv].vflg == 1)
//        {
//            gmin = eph[0][sv].toc;
//            tmin = eph[0][sv].t;
//            break;
//        }
//    }

//    for(sv = 0; sv < MAX_SAT; sv++)
//    {
//        if(eph[neph-1][sv].vflg == 1)
//        {
//            gmax = eph[neph - 1][sv].toc;
//            tmax = eph[neph - 1][sv].t;
//            break;
//        }
//    }

    qDebug() << "GPS: Navfile: " << navfile;

    neph = readRinexNavHourly(eph[0], &ionoutc, navfile);

    qDebug() << "number Eph: " << neph;

    if(neph <= 0)
    {
        qDebug() << "ERROR: No ephemeris available.";
        return 0;
    }

    for(sv = 0; sv < MAX_SAT; sv++)
    {
        if(eph[0][sv].vflg == 1)
        {
            gmin = eph[0][sv].toc;
            tmin = eph[0][sv].t;
            break;
        }
    }

//    for(sv = 0; sv < MAX_SAT; sv++)
//    {
//        if(eph[neph-1][sv].vflg == 1)
//        {
//            gmax = eph[neph - 1][sv].toc;
//            tmax = eph[neph - 1][sv].t;
//            break;
//        }
//    }

    g0 = gmin;
    t0 = tmin;

    /*! ----------------------------------------------------------------------------
     *  Get current time
     *  ----------------------------------------------------------------------------*/
    time_t timer;
    struct tm *gmt;

    time(&timer);
    gmt = gmtime(&timer);

    t0.y = gmt->tm_year+1900;
    t0.m = gmt->tm_mon+1;
    t0.d = gmt->tm_mday;

    t0.hh = gmt->tm_hour;
    t0.mm = gmt->tm_min;
    t0.sec = (double)(gmt->tm_sec);

    date2gps(&t0, &g0);

    /*! ----------------------------------------------------------------------------
     *  Ephemeris time overwrite
     *  ----------------------------------------------------------------------------*/
    int i;
/*    gpstime_t gtmp;
    datetime_t ttmp;
    double dsec;

    gtmp.week = g0.week;
    gtmp.sec = (double)(((int)(g0.sec))/7200)*7200.0;

    dsec = subGpsTime(gtmp,gmin);

    //Overwrite the TOC and TOE to the scenario start time
    for(sv=0; sv<MAX_SAT; sv++)
    {
        for(i=0; i<neph; i++)
        {
            gtmp = incGpsTime(eph[i][sv].toc, dsec);
            gps2date(&gtmp,&ttmp);
            eph[i][sv].toc = gtmp;
            eph[i][sv].t = ttmp;

            gtmp = incGpsTime(eph[i][sv].toe, dsec);
            eph[i][sv].toe = gtmp;
        }
    }
*/
    // Select the current set of ephemerides

    ieph = -1;

//    for(i = 0; i < neph; i++)
//    {
//        for(sv = 0; sv < MAX_SAT; sv++)
//        {
//            if(eph[i][sv].vflg == 1)
//            {
//                dt = subGpsTime(g0, eph[i][sv].toc);
//                if(dt >= -SECONDS_IN_HOUR && dt < SECONDS_IN_HOUR)
//                {
//                    ieph = i;
//                    break;
//                }
//            }
//        }

//        if(ieph >= 0)
//            break;
//    }

    ieph = 0;
    if(ieph == -1)
    {
        qDebug() << "ERROR: No current set of ephemerides has been found.";
        return 0;
    }

    /*! -----------------------------------------------------------------------------
     *  Initialize channels
     *  -----------------------------------------------------------------------------*/

    // Clear all channels
    for(i=0; i<MAX_CHAN; i++)
        chan[i].prn = 0;

    // Clear satellite allocation flag
    for(sv=0; sv<MAX_SAT; sv++)
        allocatedSat[sv] = -1;

    // Initial reception time
    grx = g0;

    // Allocate visible satellites
    allocateChannel(chan, eph[ieph], ionoutc, grx, xyz, elvmask);

    /*! ------------------------------------------------------------------------------
     *  Receiver antenna gain pattern
     *  ------------------------------------------------------------------------------*/

    for(i=0; i<37; i++)
        ant_pat[i] = pow(10.0, -ant_pat_db[i]/20.0);

    // Update receiver time
    grx.sec += 0.1;
    return 1;
}

int gps_source::setLocal(double *loc)
{
    /*! ----------------------------------------------------------------------------
     *  Receiver position
     *  ----------------------------------------------------------------------------*/
    llh[0] = loc[0] / R2D;
    llh[1] = loc[1] / R2D;
    llh[2] = loc[2];
    llh2xyz(llh,xyz);
    return 1;
}

int gps_source::work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items)
{
    (void) input_items;

    gr_complex *out = (gr_complex *) output_items[0];
    int i;
    int dataReaded = 0;

//    qDebug() << "Data request: " << noutput_items/2;
    if(s_init)
    {
        init();
        s_init = false;
    }
    while((noutput_items - dataReaded) >= SAMPLES_PER_BUFFER)
    {
        if(s_generateSignal)
        {
            for(i=0; i<MAX_CHAN; i++)
            {
                if(chan[i].prn > 0)
                {
                    // Refresh code phase and data bit counters
                    range_t rho;
                    sv = chan[i].prn-1;

                    // Current pseudorange
                    computeRange(&rho, eph[ieph][sv], &ionoutc, grx, xyz);
                    chan[i].azel[0] = rho.azel[0];
                    chan[i].azel[1] = rho.azel[1];

                    // Update code phase and data bit counters
                    computeCodePhase(&chan[i], rho, 0.1);
                    chan[i].carr_phasestep = (int)(512 * 65536.0 * chan[i].f_carr * delt);

                    // Path loss
                    path_loss = 20200000.0/rho.d;

                    // Receiver antenna gain
                    ibs = (int)((90.0-rho.azel[1]*R2D)/5.0);
                    ant_gain = ant_pat[ibs];

                    // Signal gain
                    gain[i] = (int)(path_loss*ant_gain*100.0);
                }
            }
            s_generateSignal = false;
        }
        for (isamp=0; isamp<SAMPLES_PER_BUFFER; isamp++)
        {
            int i_acc = 0;
            int q_acc = 0;

            for (i=0; i<MAX_CHAN; i++)
            {
                if (chan[i].prn>0)
                {
                    iTable = (chan[i].carr_phase >> 16) & 511;

                    ip = chan[i].dataBit * chan[i].codeCA * cosTable512[iTable] * gain[i];
                    qp = chan[i].dataBit * chan[i].codeCA * sinTable512[iTable] * gain[i];

                    i_acc += (ip + 50)/100;
                    q_acc += (qp + 50)/100;

                    // Update code phase
                    chan[i].code_phase += chan[i].f_code * delt;

                    if (chan[i].code_phase>=CA_SEQ_LEN)
                    {
                        chan[i].code_phase -= CA_SEQ_LEN;

                        chan[i].icode++;

                        if (chan[i].icode>=20) // 20 C/A codes = 1 navigation data bit
                        {
                            chan[i].icode = 0;
                            chan[i].ibit++;

                            if (chan[i].ibit>=30) // 30 navigation data bits = 1 word
                            {
                                chan[i].ibit = 0;
                                chan[i].iword++;

                            }

                            // Set new navigation data bit
                            chan[i].dataBit = (int)((chan[i].dwrd[chan[i].iword]>>(29-chan[i].ibit)) & 0x1UL)*2-1;
                        }
                    }

                    // Set currnt code chip
                    chan[i].codeCA = chan[i].ca[(int)chan[i].code_phase]*2-1;

                    // Update carrier phase
                    chan[i].carr_phase += chan[i].carr_phasestep;
                }
            }

//            qDebug() << "gps:work i value: " << i;
            // Store I/Q samples into buffer
//            out[(dataReaded+isamp)*2] = (short)i_acc;
//            out[(dataReaded+isamp)*2+1] = (short)q_acc;
            out[dataReaded+isamp].real() = i_acc;
            out[dataReaded+isamp].imag() = q_acc;

            dataCnt++;
            dataReaded++;

        } // End of omp parallel for

        //
        // Update navigation message and channel allocation every 30 seconds
        //
//        dataCnt += SAMPLES_PER_BUFFER;
        if((dataCnt%SAMPLES_PER_0_1SEC) == 0)
        {
            igrx = (int)(grx.sec*10.0+0.5);

            if (igrx%300==0) // Every 30 seconds
            {
                // Update navigation message
                for (i=0; i<MAX_CHAN; i++)
                    if (chan[i].prn>0)
                        generateNavMsg(grx, &chan[i], 0);

                // Refresh ephemeris and subframes
                // Quick and dirty fix. Need more elegant way.
           /*     for (sv=0; sv<MAX_SAT; sv++)
                {
                    if (eph[ieph+1][sv].vflg==1)
                    {
                        dt = subGpsTime(eph[ieph+1][sv].toc, grx);
                        if (dt < SECONDS_IN_HOUR)
                        {
                            ieph++;

                            for (i=0; i<MAX_CHAN; i++)
                            {
                                // Generate new subframes if allocated
                                if (chan[i].prn!=0)
                                    eph2sbf(eph[ieph][chan[i].prn-1], ionoutc, chan[i].sbf);
                            }
                        }

                        break;
                    }
                }*/

                // Update channel allocation
                allocateChannel(chan, eph[ieph], ionoutc, grx, xyz, elvmask);
            }

            // Update receiver time
//            incGpsTime(grx,0.1);
            grx.sec += 0.1;
            if(grx.sec > SECONDS_IN_WEEK)
            {
                grx.week++;
                grx.sec -= SECONDS_IN_WEEK;
            }
            s_generateSignal = true;
            dataCnt = 0;
        }
//        dataReaded += SAMPLES_PER_BUFFER;
    }


//    qDebug() << "Data Readed: " << dataReaded << "noutput_items: " << noutput_items;

    return dataReaded;
}


int gps_source::allocateChannel(channel_t *chan, ephem_t *eph, ionoutc_t ionoutc, gpstime_t grx, double *xyz, double elvMask)
{
        int nsat = 0;
        int i,sv;
        double azel[2];

        range_t rho;
        double ref[3]={0.0};
        double r_ref,r_xyz;
        double phase_ini;

        for (sv=0; sv<MAX_SAT; sv++)
        {
            if(checkSatVisibility(eph[sv], grx, xyz, 0.0, azel)==1)
            {
                nsat++; // Number of visible satellites

                if (allocatedSat[sv]==-1) // Visible but not allocated
                {
                    // Allocated new satellite
                    for (i=0; i<MAX_CHAN; i++)
                    {
                        if (chan[i].prn==0)
                        {
                            // Initialize channel
                            chan[i].prn = sv+1;
                            chan[i].azel[0] = azel[0];
                            chan[i].azel[1] = azel[1];

                            // C/A code generation
                            codegen(chan[i].ca, chan[i].prn);

                            // Generate subframe
                            eph2sbf(eph[sv], ionoutc, chan[i].sbf);

                            // Generate navigation message
                            generateNavMsg(grx, &chan[i], 1);

                            // Initialize pseudorange
                            computeRange(&rho, eph[sv], &ionoutc, grx, xyz);
                            chan[i].rho0 = rho;

                            // Initialize carrier phase
                            r_xyz = rho.range;

                            computeRange(&rho, eph[sv], &ionoutc, grx, ref);
                            r_ref = rho.range;

                            phase_ini = (2.0*r_ref - r_xyz)/LAMBDA_L1;
                            phase_ini -= floor(phase_ini);
                            chan[i].carr_phase = (unsigned int)(512 * 65536.0 * phase_ini);

                            // Done.
                            break;
                        }
                    }

                    // Set satellite allocation channel
                    if (i<MAX_CHAN)
                        allocatedSat[sv] = i;
                }
            }
            else if (allocatedSat[sv]>=0) // Not visible but allocated
            {
                // Clear channel
                chan[allocatedSat[sv]].prn = 0;

                // Clear satellite allocation flag
                allocatedSat[sv] = -1;
            }
        }

        return nsat;
}
