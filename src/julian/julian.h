
/***********************************************************
https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
https://github.com/JohannesBuchner/libnova/blob/master/src/julian_day.c
https://nimishprabhu.com/tools/jde-julian-date-converter-online.html
https://keisan.casio.com/exec/system/1227779487
https://www.onlineconversion.com/julian_date.htm
dateutils.ddiff --from-zone=America/Fortaleza 1977-10-07 2022-02-12
***********************************************************/
typedef struct PACK JULIAN{
  int
    year,
    month,
    day,
    hour,
    minute,
    second;
  double
    julian;
}JULIAN;

void    cJulianAddSecond(int s,JULIAN &date);
JULIAN *cJulianAlloc(void);
double  cJulianCalc(int year,int month,int day,int hour,int minute,int second);
JULIAN *cJulianCopy(JULIAN *julian);
void    cJulianDebug(JULIAN &date);
int     cJulianDiff(JULIAN &d0,JULIAN &d1);
JULIAN *cJulianFree(JULIAN *julian);
double  cJulianFrom(JULIAN &date);
double  cJulianFromUtc(int64 utc);
double  cJulianFromUtc(int64 utc,JULIAN &date);
void    cJulianNow(JULIAN &date);
void    cJulianReset(JULIAN &date);
void    cJulianRev(double julia,int *year,int *month,int *day,int *hour,int *minute,int *second);
void    cJulianSet(int year,int month,int day,int hour,int minute,int second,JULIAN &date);
void    cJulianTo(double jd,JULIAN &date);
int64   cJulianToUtc(JULIAN &date);

#include "julian.cpp"
//#include "DateJulian.h"
//#include "DateJulianFormat.h"
//#include "DateJulian.cpp"
//#include "DateJulianFormat.cpp"
