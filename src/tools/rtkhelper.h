#ifndef RTKHELPER_H_
#define RTKHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RTCM3_PREAMBLE                     (0xD3)
#define RTCM_INITIAL_BUFFER_LENGTH         (1024)
#define RTCM_NONPAYLOAD_LEN                (6)
#define RTCM_PREPAYLOAD_LEN                (3)
#define RTCM_MIN_LENGTH                    (RTCM_NONPAYLOAD_LEN + 10)

#pragma pack(1)
/* RX ESF-STATUS (ubx10) */
typedef struct {
    uint32_t iTOW;          /**< GPS Time of Week [ms] */
    uint8_t version;        /**< Message Version(0x02) */
    uint8_t reserved0[7];
    uint8_t fusionMode;     /**< Fusion mode: 0:initialization 1:Fusion mode 2:Suspended fusion mode, 3: Disabled fusion mode */
    uint8_t reserved1[2];
    uint8_t numSens;        /**< Number of sensors */
} ubx_payload_rx_esf_status_t;

typedef struct {
    uint8_t sensStatus1;    /**< [0:5] data type, [6] if set, this use for fusion solution [7] if set, it is set up but not used for fusion now*/
    uint8_t sensStatus2;    /**< [0:1] 00:sensor is not calibrated, 01:sensor is calibrating 10/11: sensor is calibrated, [2:3] ...*/
    uint8_t freq;           /**< Ovservation frequency */
    uint8_t faults;         /**< [0] bad measurements [1]  bad measurement time-tags [2] Mission ot time-misaligned measurements [3] High measurement noise-level */
} ubx_payload_rx_esf_sensor_status_t;


/* Rx ESF-INS (ubx10)*/
typedef struct {
    uint32_t bitfield;      /**< [0:7]msg version(0x01) [8] xAngRateValid (0:not valid) [9] yAngRateValid [10] zAngRateValid [11] xAccelValid [12] yAccelValid [13] zAccelValid */
    uint32_t reserved0;
    uint32_t iTOW;          /**< GPS Time of Week [ms] */
    int32_t xAngRate;       /**< Compensated x-axis angular rate 1e-3 [deg/s] */
    int32_t yAngRate;       /**< Compensated y-axis angular rate 1e-3 [deg/s] */
    int32_t zAngRate;       /**< Compensated z-axis angular rate 1e-3 [deg/s] */
    int32_t xAccel;         /**< Compensated x-axis acceleration 1e-2 [m/s^2] */
    int32_t yAccel;         /**< Compensated y-axis acceleration 1e-2 [m/s^2] */
    int32_t zAccel;         /**< Compensated z-axis acceleration 1e-2 [m/s^2] */
} ubx_payload_rx_esf_ins_t;

/* Rx NAV-CLOCK (ubx10) */
typedef struct {
    uint32_t iTOW;          /**< GPS Time of Week [ms] */
    int32_t clkB;           /**< Clock bias [ns] */
    int32_t clkD;           /**< Clock drift [ns/s] */
    uint32_t tAcc;          /**< Time accuracy estimate [ns] */
    uint32_t fAcc;          /**< Frequency accuracy estimate [ps/s] */
} ubx_payload_rx_nav_clock_t;

/* Rx NAV-PVT (ubx8) */
typedef struct {
    uint32_t iTOW;          /**< GPS Time of Week [ms] */
    uint16_t year;          /**< Year (UTC)*/
    uint8_t  month;         /**< Month, range 1..12 (UTC) */
    uint8_t  day;           /**< Day of month, range 1..31 (UTC) */
    uint8_t  hour;          /**< Hour of day, range 0..23 (UTC) */
    uint8_t  min;           /**< Minute of hour, range 0..59 (UTC) */
    uint8_t  sec;           /**< Seconds of minute, range 0..60 (UTC) */
    uint8_t  valid;         /**< Validity flags (see UBX_RX_NAV_PVT_VALID_...) */
    uint32_t tAcc;          /**< Time accuracy estimate (UTC) [ns] */
    int32_t  nano;          /**< Fraction of second (UTC) [-1e9...1e9 ns] */
    uint8_t  fixType;       /**< GNSSfix type: 0 = No fix, 1 = Dead Reckoning only, 2 = 2D fix, 3 = 3d-fix, 4 = GNSS + dead reckoning, 5 = time only fix */
    uint8_t  flags;         /**< Fix Status Flags (see UBX_RX_NAV_PVT_FLAGS_...) */
    uint8_t  reserved1;
    uint8_t  numSV;         /**< Number of SVs used in Nav Solution */
    int32_t  lon;           /**< Longitude [1e-7 deg] */
    int32_t  lat;           /**< Latitude [1e-7 deg] */
    int32_t  height;        /**< Height above ellipsoid [mm] */
    int32_t  hMSL;          /**< Height above mean sea level [mm] */
    uint32_t hAcc;          /**< Horizontal accuracy estimate [mm] */
    uint32_t vAcc;          /**< Vertical accuracy estimate [mm] */
    int32_t  velN;          /**< NED north velocity [mm/s]*/
    int32_t  velE;          /**< NED east velocity [mm/s]*/
    int32_t  velD;          /**< NED down velocity [mm/s]*/
    int32_t  gSpeed;        /**< Ground Speed (2-D) [mm/s] */
    int32_t  headMot;       /**< Heading of motion (2-D) [1e-5 deg] */
    uint32_t sAcc;          /**< Speed accuracy estimate [mm/s] */
    uint32_t headAcc;       /**< Heading accuracy estimate (motion and vehicle) [1e-5 deg] */
    uint16_t pDOP;          /**< Position DOP [0.01] */
    uint16_t reserved2;
    uint32_t reserved3;
    int32_t  headVeh;       /**< (ubx8+ only) Heading of vehicle (2-D) [1e-5 deg] */
    uint32_t reserved4;     /**< (ubx8+ only) */
} ubx_payload_rx_nav_pvt_t;

typedef struct {
    uint32_t iTOW;   /**< GPS Time of Week [ms] */
    int32_t  lon;    /**< Longitude [1e-7 deg] */
    int32_t  lat;    /**< Latitude [1e-7 deg] */
    int32_t  height; /**< Height above ellipsoid [mm] */
    int32_t  hMSL;   /**< Height above mean sea level [mm] */
    uint32_t hAcc;   /**< Horizontal accuracy estimate [mm] */
    uint32_t vAcc;   /**< Vertical accuracy estimate [mm] */
} ubx_payload_rx_nav_posllh_t;

/* Rx NAV-SOL */
typedef struct {
    uint32_t iTOW;     /**< GPS Time of Week [ms] */
    int32_t  fTOW;     /**< Fractional part of iTOW (range: +/-500000) [ns] */
    int16_t  week;     /**< GPS week */
    uint8_t  gpsFix;   /**< GPSfix type: 0 = No fix, 1 = Dead Reckoning only, 2 = 2D fix, 3 = 3d-fix, 4 = GPS + dead reckoning, 5 = time only fix */
    uint8_t  flags;
    int32_t  ecefX;
    int32_t  ecefY;
    int32_t  ecefZ;
    uint32_t pAcc;
    int32_t  ecefVX;
    int32_t  ecefVY;
    int32_t  ecefVZ;
    uint32_t sAcc;
    uint16_t pDOP;      /**< Position DOP [0.01] */
    uint8_t  reserved1;
    uint8_t  numSV;     /**< Number of SVs used in Nav Solution */
    uint32_t reserved2;
} ubx_payload_rx_nav_sol_t;

/* Rx NAV-STATUS */
typedef struct {
    uint32_t iTOW;           /**< GPS Time of Week [ms] */
    uint8_t  gpsFix;         /**< GPSfix Type, range 0..5 */
    uint8_t  flags;          /**< Fix Status Flags */
    uint8_t  fixStat;        /**< Fix Status Information */
    uint8_t  flags2;         /**< Additional Flags */
    uint32_t ttff;           /**< Time to first fix [ms] */
    uint32_t msss;           /**< Milliseconds since startup/reset */
} ubx_payload_rx_nav_status_t;

/* Rx NAV-DOP */
typedef struct {
    uint32_t iTOW; /**< GPS Time of Week [ms] */
    uint16_t gDOP; /**< Geometric DOP [0.01] */
    uint16_t pDOP; /**< Position DOP [0.01] */
    uint16_t tDOP; /**< Time DOP [0.01] */
    uint16_t vDOP; /**< Vertical DOP [0.01] */
    uint16_t hDOP; /**< Horizontal DOP [0.01] */
    uint16_t nDOP; /**< Northing DOP [0.01] */
    uint16_t eDOP; /**< Easting DOP [0.01] */
} ubx_payload_rx_nav_dop_t;

/* NAV RELPOSNED (protocol version 27+) */
typedef struct {
    uint8_t     version;         /**< message version (expected 0x01) */
    uint8_t     reserved0;
    uint16_t    refStationId;    /**< Reference station ID. Must be in the range 0..4095 */
    uint32_t    iTOW;            /**< [ms] GPS time of week of the navigation epoch */
    int32_t     relPosN;         /**< [cm] North component of relative position vector */
    int32_t     relPosE;         /**< [cm] East component of relative position vector */
    int32_t     relPosD;         /**< [cm] Down component of relative position vector */
    int32_t     relPosLength;    /**< [cm] Length of the relative position vector */
    int32_t     relPosHeading;   /**< [1e-5 deg] Heading of the relative position vector */
    uint32_t    reserved1;
    int8_t      relPosHPN;       /**< [0.1 mm] High-precision North component of relative position vector */
    int8_t      relPosHPE;       /**< [0.1 mm] High-precision East component of relative position vector */
    int8_t      relPosHPD;       /**< [0.1 mm] High-precision Down component of relative position vector */
    int8_t      relPosHPLength;  /**< [0.1 mm] High-precision component of the length of the relative position vector */
    uint32_t    accN;            /**< [0.1 mm] Accuracy of relative position North component */
    uint32_t    accE;            /**< [0.1 mm] Accuracy of relative position East component */
    uint32_t    accD;            /**< [0.1 mm] Accuracy of relative position Down component */
    uint32_t    accLength;       /**< [0.1 mm] Accuracy of the length of the relative position vector */
    uint32_t    accHeading;      /**< [1e-5 deg] Accuracy of the heading of the relative position vector */
    uint32_t    reserved2;
    uint32_t    flags;
} ubx_payload_rx_nav_relposned_t;

/* NAV HPPOSLLH (protocol version 27+) */
typedef struct {
    uint8_t     version;         /**< message version (expected 0x00) */
    uint8_t     reserved1[2];
    int8_t      flags;           /**<  invalidLlh: 1 = Invalid lon, lat, height, hMSL, lonHp, latHp, heightHp and hMSLHp */
    uint32_t    iTOW;            /**<  [ms] GPS time of week of the navigation epoch */
    int32_t     lon;             /**<  [1e-7 deg] Longitude */
    int32_t     lat;             /**<  [1e-7 deg] Latitude */
    int32_t     height;          /**<  [mm] Height above Ellipsoid */
    int32_t     hMSL;            /**<  [mm] Height above mean sea level */
    int8_t      lonHp;           /**<  [1e-9 deg] Longitude high precision component, -99 to +99 */
    int8_t      latHp;           /**<  [1e-9 deg] Latitude high precision component, -99 to +99 */
    int8_t      heightHp;        /**<  [0.1 mm] high precision component of height above Ellipsoid, -9 to +9 */
    int8_t      hMSLHp;          /**<  [0.1 mm] high precision component of height above mean sea level, -9 to +9 */
    uint32_t    hAcc;            /**<  [0.1 mm] Horizontal Accuracy Estimate */
    uint32_t    vAcc;            /**<  [0.1 mm] Vertical Accuracy Estimate */
} ubx_payload_rx_nav_hpposllh_t;

/* Rx NAV-TIMEUTC */
typedef struct {
    uint32_t iTOW;           /**< GPS Time of Week [ms] */
    uint32_t tAcc;           /**< Time accuracy estimate (UTC) [ns] */
    int32_t  nano;           /**< Fraction of second, range -1e9 .. 1e9 (UTC) [ns] */
    uint16_t year;           /**< Year, range 1999..2099 (UTC) */
    uint8_t  month;          /**< Month, range 1..12 (UTC) */
    uint8_t  day;            /**< Day of month, range 1..31 (UTC) */
    uint8_t  hour;           /**< Hour of day, range 0..23 (UTC) */
    uint8_t  min;            /**< Minute of hour, range 0..59 (UTC) */
    uint8_t  sec;            /**< Seconds of minute, range 0..60 (UTC) */
    uint8_t  valid;          /**< Validity Flags (see UBX_RX_NAV_TIMEUTC_VALID_...) */
} ubx_payload_rx_nav_timeutc_t;

/* Rx NAV-VELNED */
typedef struct {
    uint32_t iTOW;           /**< GPS Time of Week [ms] */
    int32_t  velN;           /**< North velocity component [cm/s]*/
    int32_t  velE;           /**< East velocity component [cm/s]*/
    int32_t  velD;           /**< Down velocity component [cm/s]*/
    uint32_t speed;          /**< Speed (3-D) [cm/s] */
    uint32_t gSpeed;         /**< Ground speed (2-D) [cm/s] */
    int32_t  heading;        /**< Heading of motion 2-D [1e-5 deg] */
    uint32_t sAcc;           /**< Speed accuracy estimate [cm/s] */
    uint32_t cAcc;           /**< Course / Heading accuracy estimate [1e-5 deg] */
} ubx_payload_rx_nav_velned_t;
#pragma pack()

typedef enum {
    RTKFRAMING_INCOMPLETE = 0,
    RTKFRAMING_OK = 1,
    RTKFRAMING_BAD_CHECK = 2,
} rtkframing_t;

typedef enum {
    RTCM_STATE_UNINIT = 0,
    RTCM_STATE_IDLE,
    RTCM_STATE_GOT_PREAMBLE,
    RTCM_STATE_GOT_SYNC,
    RTCM_STATE_GOT_SYNC_LEN,
    RTCM_STATE_GOT_PAYLOAD,
    RTCM_STATE_GOT_CHECKA,
    RTCM_STATE_GOT_CHECKB,
    RTCM_STATE_GOT_BAD_CHECK,
} rtcm_parse_state_t;

typedef struct _rtcm_msg {
    rtkframing_t state;
    /* [0xd3] [sync|paylenlen] [len] [payload] [cka] [ckb] [ckc] */
    uint8_t buff[RTCM_INITIAL_BUFFER_LENGTH];
    uint16_t len;   /* payload len + (head)3 */
    uint16_t nbyte; /* index of all framebuff */
    uint16_t type; /* message type number */
} rtcm_msg_t;

uint8_t rtcm_frame_char_buffer(rtcm_msg_t *msg, uint8_t c);

#ifdef __cplusplus
}
#endif

#endif
