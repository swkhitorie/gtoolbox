#include "rtklib.h"

#define RTCM2PREAMB (0x66)
#define RTCM3PREAMB (0xD3)

/* adjust weekly rollover of GPS time ----------------------------------------*/
static void adjweek(rtcm_t *rtcm, double tow)
{
    double tow_p;
    int week;

    /* if no time, get cpu time */
    if (rtcm->time.time==0) rtcm->time=utc2gpst(timeget());
    tow_p=time2gpst(rtcm->time,&week);
    if      (tow<tow_p-302400.0) tow+=604800.0;
    else if (tow>tow_p+302400.0) tow-=604800.0;
    rtcm->time=gpst2time(week,tow);
}

/* adjust weekly rollover of BDS time ----------------------------------------*/
static int adjbdtweek(int week)
{
    int w;
    (void)time2bdt(gpst2bdt(utc2gpst(timeget())),&w);
    if (w<1) w=1; /* use 2006/1/1 if time is earlier than 2006/1/1 */
    return week+(w-week+512)/1024*1024;
}

/* adjust daily rollover of GLONASS time -------------------------------------*/
static void adjday_glot(rtcm_t *rtcm, double tod)
{
    gtime_t time;
    double tow,tod_p;
    int week;

    if (rtcm->time.time==0) rtcm->time=utc2gpst(timeget());
    time=timeadd(gpst2utc(rtcm->time),10800.0); /* glonass time */
    tow=time2gpst(time,&week);
    tod_p=fmod(tow,86400.0); tow-=tod_p;
    if      (tod<tod_p-43200.0) tod+=86400.0;
    else if (tod>tod_p+43200.0) tod-=86400.0;
    time=gpst2time(week,tow+tod);
    rtcm->time=utc2gpst(timeadd(time,-10800.0));
}

static int decode_msm_head(rtcm_t *rtcm, int sys)
{
    double tow,tod;
    int dow,staid;
    int i=24;
    if (i+169<=rtcm->len*8) {
        staid = getbitu(rtcm->buff,i,12); i+=12;
        if (sys==SYS_GLO) {
            dow = getbitu(rtcm->buff,i,3); i+= 3;
            tod = getbitu(rtcm->buff,i,27)*0.001; i+=27;
            adjday_glot(rtcm,tod);
        } else if (sys==SYS_CMP) {
            tow = getbitu(rtcm->buff,i,30)*0.001; i+=30;
            tow+=14.0; /* BDT -> GPST */
            adjweek(rtcm,tow);
        } else {
            tow = getbitu(rtcm->buff,i,30)*0.001; i+=30;
            adjweek(rtcm,tow);
        }
    }
}

static int decode_msm7(rtcm_t *rtcm, int sys)
{
    decode_msm_head(rtcm, sys);
}

static int decode_teseo_epvt(rtcm_t *rtcm)
{
    int idx = 24;
    double gnss_epoch_time = 0;
    gnss_epoch_time = getbitu(rtcm->buff,idx+145,30) * 0.001f;
    rtcm->tow = gnss_epoch_time;
}

static int decode_teseo(rtcm_t *rtcm)
{
    uint8_t subid;
    int i = 24;
    if (rtcm->msg_number != 999) return -1;
    i += 12;
    subid = getbitu(rtcm->buff,i,8);
    switch (subid) {
    case 21: decode_teseo_epvt(rtcm); break;
    }
}


void decode_rtcm3(rtcm_t *rtcm)
{
    int ret;
    rtcm->msg_number = getbitu(rtcm->buff,24,12);

    switch (rtcm->msg_number) {
#ifdef USE_TESEO_RESULT
    case 999: ret=decode_teseo(rtcm); break;
#else
    case 1077: ret=decode_msm7(rtcm,SYS_GPS); break;
    case 1097: ret=decode_msm7(rtcm,SYS_GAL); break;
    case 1127: ret=decode_msm7(rtcm,SYS_CMP); break;
#endif
    }
}

int input_rtcm3(rtcm_t *rtcm, uint8_t data)
{
    if (rtcm->nbyte==0) {
        if (data!=RTCM3PREAMB) return RTCM3_FRAME_INCOMPLETE;
        rtcm->buff[rtcm->nbyte++]=data;
        return RTCM3_FRAME_INCOMPLETE;
    }
    rtcm->buff[rtcm->nbyte++]=data;

    if (rtcm->nbyte==3) {
        rtcm->len=getbitu(rtcm->buff,14,10)+3;
    }
    if (rtcm->nbyte<3||rtcm->nbyte<rtcm->len+3) return RTCM3_FRAME_INCOMPLETE;
    rtcm->nbyte=0;

    if (rtk_crc24q(rtcm->buff,rtcm->len)!=getbitu(rtcm->buff,rtcm->len*8,24)) {
        return RTMC3_FRAME_BAD_CRC;
    }
    decode_rtcm3(rtcm);

    return RTCM3_FRAME_OK;
}
