#ifndef RTKLIB_H
#define RTKLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define EXPORT

#define USE_TESEO_RESULT

#define PI          3.1415926535897932  /* pi */
#define D2R         (PI/180.0)          /* deg to rad */
#define R2D         (180.0/PI)          /* rad to deg */
#define CLIGHT      299792458.0         /* speed of light (m/s) */
#define SC2RAD      3.1415926535898     /* semi-circle to radian (IS-GPS) */
#define AU          149597870691.0      /* 1 AU (m) */
#define AS2R        (D2R/3600.0)        /* arc sec to radian */

#define OMGE        7.2921151467E-5     /* earth angular velocity (IS-GPS) (rad/s) */

#define RE_WGS84    6378137.0           /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84    (1.0/298.257223563) /* earth flattening (WGS84) */

#define HION        350000.0            /* ionosphere height (m) */

#define MAXFREQ     7                   /* max NFREQ */

#define FREQ1       1.57542E9           /* L1/E1/B1C  frequency (Hz) */
#define FREQ2       1.22760E9           /* L2         frequency (Hz) */
#define FREQ5       1.17645E9           /* L5/E5a/B2a frequency (Hz) */
#define FREQ6       1.27875E9           /* E6/L6  frequency (Hz) */
#define FREQ7       1.20714E9           /* E5b    frequency (Hz) */
#define FREQ8       1.191795E9          /* E5a+b  frequency (Hz) */
#define FREQ9       2.492028E9          /* S      frequency (Hz) */
#define FREQ1_GLO   1.60200E9           /* GLONASS G1 base frequency (Hz) */
#define DFRQ1_GLO   0.56250E6           /* GLONASS G1 bias frequency (Hz/n) */
#define FREQ2_GLO   1.24600E9           /* GLONASS G2 base frequency (Hz) */
#define DFRQ2_GLO   0.43750E6           /* GLONASS G2 bias frequency (Hz/n) */
#define FREQ3_GLO   1.202025E9          /* GLONASS G3 frequency (Hz) */
#define FREQ1a_GLO  1.600995E9          /* GLONASS G1a frequency (Hz) */
#define FREQ2a_GLO  1.248060E9          /* GLONASS G2a frequency (Hz) */
#define FREQ1_CMP   1.561098E9          /* BDS B1I     frequency (Hz) */
#define FREQ2_CMP   1.20714E9           /* BDS B2I/B2b frequency (Hz) */
#define FREQ3_CMP   1.26852E9           /* BDS B3      frequency (Hz) */

#define EFACT_GPS   1.0                 /* error factor: GPS */
#define EFACT_GLO   1.5                 /* error factor: GLONASS */
#define EFACT_GAL   1.0                 /* error factor: Galileo */
#define EFACT_QZS   1.0                 /* error factor: QZSS */
#define EFACT_CMP   1.0                 /* error factor: BeiDou */
#define EFACT_IRN   1.5                 /* error factor: IRNSS */
#define EFACT_SBS   3.0                 /* error factor: SBAS */

#define SYS_NONE    0x00                /* navigation system: none */
#define SYS_GPS     0x01                /* navigation system: GPS */
#define SYS_SBS     0x02                /* navigation system: SBAS */
#define SYS_GLO     0x04                /* navigation system: GLONASS */
#define SYS_GAL     0x08                /* navigation system: Galileo */
#define SYS_QZS     0x10                /* navigation system: QZSS */
#define SYS_CMP     0x20                /* navigation system: BeiDou */
#define SYS_IRN     0x40                /* navigation system: IRNS */
#define SYS_LEO     0x80                /* navigation system: LEO */
#define SYS_ALL     0xFF                /* navigation system: all */

#define TSYS_GPS    0                   /* time system: GPS time */
#define TSYS_UTC    1                   /* time system: UTC */
#define TSYS_GLO    2                   /* time system: GLONASS time */
#define TSYS_GAL    3                   /* time system: Galileo time */
#define TSYS_QZS    4                   /* time system: QZSS time */
#define TSYS_CMP    5                   /* time system: BeiDou time */
#define TSYS_IRN    6                   /* time system: IRNSS time */

#define MAXLEAPS    64                  /* max number of leap seconds table */

typedef struct {
    time_t time;
    double sec;
} gtime_t;

enum __rtcm3_ret {
    RTCM3_FRAME_INCOMPLETE,
    RTMC3_FRAME_BAD_CRC,
    RTCM3_FRAME_OK,
};

typedef struct {
    int msg_number;
    int ftype;
    gtime_t time;
    gtime_t time_s;
    double tow;

    int nbyte;
    int len;
    uint8_t buff[1200];
} rtcm_t;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t getbitu(const uint8_t *buff, int pos, int len);
int32_t getbits(const uint8_t *buff, int pos, int len);
void setbitu(uint8_t *buff, int pos, int len, uint32_t data);
void setbits(uint8_t *buff, int pos, int len, int32_t data);
uint32_t rtk_crc24q(const uint8_t *buff, int len);

EXPORT double str2num(const char *s, int i, int n);
EXPORT int str2time(const char *s, int i, int n, gtime_t *t);
EXPORT void time2str(gtime_t t, char *str, int n);
EXPORT gtime_t epoch2time(const double *ep);
EXPORT void time2epoch(gtime_t t, double *ep);
EXPORT gtime_t gpst2time(int week, double sec);
EXPORT double time2gpst(gtime_t t, int *week);
EXPORT gtime_t gst2time(int week, double sec);
EXPORT double time2gst(gtime_t t, int *week);
EXPORT gtime_t bdt2time(int week, double sec);
EXPORT double time2bdt(gtime_t t, int *week);
EXPORT char *time_str(gtime_t t, int n);

EXPORT gtime_t timeadd(gtime_t t, double sec);
EXPORT double timediff(gtime_t t1, gtime_t t2);
EXPORT gtime_t gpst2utc(gtime_t t);
EXPORT gtime_t utc2gpst(gtime_t t);
EXPORT gtime_t gpst2bdt(gtime_t t);
EXPORT gtime_t bdt2gpst(gtime_t t);
EXPORT gtime_t timeget(void);
EXPORT void timeset(gtime_t t);
EXPORT void timereset(void);
EXPORT double time2doy(gtime_t t);
EXPORT double utc2gmst(gtime_t t, double ut1_utc);

EXPORT int adjgpsweek(int week);


EXPORT int input_rtcm3(rtcm_t *rtcm, uint8_t data);
EXPORT void decode_rtcm3(rtcm_t *rtcm);

#ifdef __cplusplus
}
#endif
#endif
