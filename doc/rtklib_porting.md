## 1. RTKLIB  files

`v2.4.3 b34`

```bash
[REMOVE] ./src/convgpx.c (gnss数据转换接口)
	extern int convgpx()
[REMOVE] ./src/convkml.c (gnss数据->google earth kml)
	extern int convkml()
[REMOVE] ./src/convrnx.c  (gnss数据->RINEX格式接口)
	extern int convrnx()
[REMOVE] ./src/datum.c  (日本高程系统转换接口)
	extern int loaddatump()
	extern int tokyo2jgd()
	extern int jgd2tokyo()
[REMOVE] ./src/download.c (平台文件IO实现,存储数据)
	extern int execcmd_to()
	extern int dl_readurls()
	extern int dl_readstas()
	extern int dl_exec()
	extern void dl_test()
./src/ephemeris.c (星历数据操作接口)
	extern void alm2pos()
	extern double eph2clk()
	extern void eph2pos()
	extern double geph2clk()
	extern void geph2pos()
	extern double seph2clk()
	extern void seph2pos()
	extern int satpos()
	extern void satposs()
	extern void setseleph()
	extern int getseleph()
./src/geoid.c (#define IERS_MODEL) (大地水准面模型,from file or embedded)
	extern int opengeoid() [FILE]
	extern void closegeoid() [FILE]
	extern double geoidh()
[REMOVE] ./src/gis.c 
	extern int gis_read()
	extern void gis_free()
./src/ionex.c
	extern void readtec() [FILE]
	extern int iontec()
./src/lambda.c  (最小二乘估计)
	extern int lambda()
	extern int lambda_reduction()
	extern int lambda_search()
[REMOVE] ./src/options.c (文件IO或其他)
	extern opt_t *searchopt(...)
	extern int str2opt(...)
	extern int opt2str(...)
	extern int opt2buf(...)
	extern int loadopts(...)
	extern int saveopts(...)
	extern void resetsysopts(...)
	extern void getsysopts(...)
	extern void setsysopts(...)
./src/pntpos.c (定位及校正接口)
	extern int ionocorr(...)
	extern int tropcorr(...)
	extern int pntpos(...)
[REMOVE] ./src/postpos.c (数据后处理定位接口), 用于数据处理软件rnx2rtkp...
	extern int postpos(...)
[REMOVE] ./src/ppp_ar.c (整周模糊度算法)
	extern int ppp_ar(...) none
./src/ppp.c (PPP单点定位接口)
	extern int pppoutstat()
	extern int yaw_angle()
	extern int pppnx()
	extern int pppos()
./src/preceph.c (相位偏移)
	extern void readsp3() [FILE]
	extern int readsap() [FILE]
	extern int readdcb() [FILE]
	extern void satantoff()
	extern int peph2pos()
./src/rcvraw.c
	extern int decode_irn_nav()
	extern int decode_gal_inav()
	extern int decode_gal_fnav()
	extern int decode_bds_d1()
	extern int decode_bds_d2()
	extern int test_glostr()
	extern int decode_glostr()
	extern int decode_frame()
	extern int init_raw()
	extern void free_raw()
	extern int input_raw()
	extern int input_rawf() [FILE]
[REMOVE]  ./src/rinex.c
	extern int readrnxt()
	extern int readrnx()
	extern int readrnxc()
	extern int init_rnxctr()
	extern void free_rnxctr()
	extern int open_rnxctr()
	extern int input_rnxctr()
	extern int outrnxobsh()
	extern int outrnxobsb()
	extern int outrnxnavh()
	extern int outrnxnavb()
	extern int outrnxgnavh()
	extern int outrnxgnavb()
	extern int outrnxhnavh()
	extern int outrnxhnavb()
	extern int outrnxlnavh()
	extern int outrnxqnavh()
	extern int outrnxcnavh()
	extern int outrnxinavh()
./src/rtcm.c
	extern int decode_rtcm2()
	extern int decode_rtcm3()
	extern int encode_rtcm3()
	extern int init_rtcm()
	extern void free_rtcm()
	extern int input_rtcm2()
	extern int input_rtcm3()
	extern int input_rtcm2f() [FILE]
	extern int input_rtcm3f() [FILE]
	extern int gen_rtcm2()
	extern int gen_rtcm3()
./src/rtcm2.c
	extern int decode_rtcm2()
./src/rtcm3.c
	extern int decode_rtcm3()
./src/rtcm3e.c
	extern int encode_rtcm3()
./src/rtkpos.c (计算定位解)
	extern int rtkopenstat() [FILE]
	extern void rtkclosestat() [FILE]
	extern int rtkoutstat()
	extern void rtkinit()
	extern void rtkfree()
	extern int rtkpos()
[REMOVE] ./src/rtksvr.c  (RTK基站/差分站/流动站实时接口实现), 用于实时数据处理软件rtknavi
	extern int rtksvrinit()
	extern void rtksvrfree()
	extern void rtksvrlock()
	extern void rtksvrunlock()
	extern int rtksvrstart()
	extern void rtksvrstop()
	extern int rtksvropenstr()
	extern void rtksvrclosestr()
	extern int rtksvrostat()
	extern void rtksvrsstat()
	extern int rtksvrmark()
./src/sbas.c  (SBAS星基增强相关操作接口)
	extern int sbsupdatecorr()
	extern int sbsreadmsgt() [FILE]
	extern int sbsreadmsg() [FILE]
	extern void sbsoutmsg() [FILE]
	extern int sbsioncorr()
	extern double sbstropcorr()
	extern int sbssatcorr()
	extern int sbsdecodemsg()
./src/solution.c  (定位解相关解析接口(nmea))
	extern int inputsol()
	extern int readsolt() [FILE]
	extern int readsol() [FILE]
	extern int addsol()
	extern sol_t *getsol()
	extern void initsolbuf()
	extern void freesolbuf()
	extern void freesolstatbuf()
	extern int readsolstatt() [FILE]
	extern int readsolstat() [FILE]
	extern int outnmea_rmc()
	extern int outnmea_gga()
	extern int outnmea_gsa()
	extern int outnmea_gsv()
	extern int outprcopts()
	extern int outsolheads()
	extern int outsols()
	extern int outsolexs()
	extern void outprcopt() [FILE]
	extern void outsolhead() [FILE]
	extern void outsol() [FILE]
	extern void outsolex() [FILE]
[REMOVE] ./src/stream.c (平台通信IO实现,从usart/net口获取数据)
	extern void strinitcom()
	extern void strinit()
	extern int stropen()
	extern void strclose()
	extern void strsync()
	extern void strlock()
	extern void strunlock()
	extern int strread()
	extern int strwrite()
	extern int strstat()
	extern int strstatx()
	extern void strsum()
	extern void strsetopt()
	extern void strsettimeout()
	extern void strsetdir()
	extern void strsetproxy()
	extern gtime_t strgettime()
	extern void strsendnmea()
	extern void strsendcmd()
[REMOVE] ./src/streamsvr.c (平台通信IO实现,基站发送数据接口)
	extern strconv_t *strconvnew()
	extern void strconvfree()
	extern void strsvrinit()
	extern int strsvrstart()
	extern void strsvrstop()
	extern void strsvrstat()
	extern int strsvrpeek()
./src/tides.c
	extern void tidedisp()
	extern int dehanttideinel_()
[REMOVE] ./src/tle.c (TLE两行轨道数据格式操作接口)
	extern int tle_read() [FILE]
	extern int tle_name_read() [FILE]
	extern int tle_pos()
./src/rcv/binex.c
	extern int input_bnxf()
	extern int input_bnx()
./src/rcv/crescent.c
	extern int input_cres()
	extern int input_cresf()
./src/rcv/javad.c
	extern int input_javad()
	extern int input_javadf()
./src/rcv/novatel.c
	extern int input_oem4()
	extern int input_oem3()
	extern int input_oem4f()
	extern int input_oem3f()
....

platforms interfaces in rtkcmn
	clear #ifdef WIN32 WIN_DLL judgement
	uncomment // #include <sys/select.h> in rtklib.h
	comment declaration and define:
		EXPORT int execcmd(const char *cmd);
		EXPORT int expath (const char *path, char *paths[], int nmax);
		EXPORT void createdir(const char *path);

		extern void readtec(const char *file, nav_t *nav, int opt)
		extern void readsp3(const char *file, nav_t *nav, int opt)
		extern int readdcb(const char *file, nav_t *nav, const sta_t *sta)
		extern int sbsreadmsg(const char *file, int sel, sbs_t *sbs)
		extern int sbsreadmsgt(const char *file, int sel, gtime_t ts, gtime_t te,
                       sbs_t *sbs)
		extern int rtk_uncompress(const char *file, char *uncfile)

	static int mkdir_r(const char *dir)
	extern gtime_t timeget(void)
	extern uint32_t tickget(void)
	extern void sleepms(int ms)

porting step:
	1. remove files no needed
	2. comment rcv data functions in rcvraw.c

enable:
	define ENAGLO ENAGAL ENAQZS ENACMP
```



