/*=========================================================

=========================================================*/
void cJulianTo(double jd,JULIAN &date){
  if(!jd)
    cMemReset(&date,sizeof(JULIAN));
  else{
    cJulianRev(
      jd,
      &date.year,
      &date.month,
      &date.day,
      &date.hour,
      &date.minute,
      &date.second
    );
    date.julian=jd;
  }
}
/*=========================================================

=========================================================*/
double cJulianFrom(JULIAN &date){
  return date.julian=cJulianCalc(
      date.year,
      date.month,
      date.day,
      date.hour,
      date.minute,
      date.second
  );
}
/*=========================================================

=========================================================*/
JULIAN *cJulianAlloc(void){
  return (JULIAN*)cMemAlloc(sizeof(JULIAN));
}
/*=========================================================

=========================================================*/
JULIAN *cJulianFree(JULIAN *julian){
  if(julian)
    cMemFree(julian);
  return 0;
}
/*=========================================================

=========================================================*/
JULIAN *cJulianCopy(JULIAN *julian){
  return julian
    ?(JULIAN*)cMemCpy(julian, sizeof(JULIAN),cJulianAlloc())
    :cJulianAlloc();
}
/*=========================================================

=========================================================*/
void cJulianDebug(JULIAN &date){
  cPrintf(
    "%02d/%02d/%04d %02d:%02d:%02d => %.7f\n",
    date.day,
    date.month,
    date.year,
    date.hour,
    date.minute,
    date.second,
    date.julian
  );
}
/*=========================================================

=========================================================*/
void cJulianReset(JULIAN &date){
  cMemSet(0,sizeof(JULIAN),&date);
}
/*=========================================================

=========================================================*/
double cJulianCalc(int year,int month,int day,int hour,int minute,int second){
  double
    jy,jm,jd0,jd,
    intgr,gregcal,dayfrac,frac;

  if(month>2){
    jy=year;
    jm=month+1;
  }
  else{
    jy=year-1;
    jm=month+13;
  }
  intgr=floor(0
    +floor(365.25*jy)
    +floor(30.6001*jm)
    +day
    +1720995
  );
  gregcal=15+31*(10+12*1582);
  if(day+31*(month+12*year)>=gregcal){
    double
      ja=floor(0.01*jy);

    intgr+=2-ja+floor(0.25*ja);
  }
  dayfrac=hour/24.0-0.5;
  if(dayfrac<0.0){
    dayfrac+=1.0;
    --intgr;
  }
  frac=dayfrac+(minute+second/60.0)/60.0/24.0;
  jd0=(intgr+frac)*100000;
  jd=floor(jd0);
  if(jd0-jd>0.5)
    ++jd;
  return jd/100000;
}
/*=========================================================

=========================================================*/
void cJulianRev(double julia,int *year,int *month,int *day,int *hour,int *minute,int *second){
  double
    j1,j2,j3,j4,j5,f,
    intgr=floor(julia),
    frac=julia-intgr,
    gregjd=2299161,
    dayfrac=frac + 0.5;

  if(intgr<gregjd)
    j1=intgr;
  else{
    double
      tmp=floor((intgr-1867216-0.25)/36524.25);

    j1=intgr+1+tmp-floor(0.25*tmp);
  }

  if(dayfrac>=1.0){
    dayfrac-=1.0;
    ++j1;
  }

  j2=j1+1524;
  j3=floor(6680.0+(j2-2439870-122.1)/365.25);
  j4=floor(j3*365.25);
  j5=floor((j2-j4)/30.6001);

  *day=floor(j2-j4-floor(j5*30.6001));
  *month=floor(j5-1);
  if(*month>12)
    *month-=12;
  *year=floor(j3-4715);
  if(*month>2)
    --(*year);
  if(*year<=0)
    --(*year);
  if(*year<0)
    *year=-*year;

  *hour=floor(dayfrac*24.0);
  *minute=floor((dayfrac*24.0-*hour)*60.0);
  f=((dayfrac*24.0-*hour)*60.0-*minute)*60.0;
  *second=floor(f);
  f-=*second;
  if(f>0.5)
    ++(*second);
  if(*second==60){
    *minute+=1;
    *second=0;
  }
  if(*minute==60){
    *hour+=1;
    *minute=0;
  }


}
/*=========================================================

=========================================================*/
double cJulianFromUtc(int64 utc){
  double
    time=utc-__timezone;

  return 2440587.5+time/86400.0;
}
/*=========================================================

=========================================================*/
double cJulianFromUtc(int64 utc,JULIAN &date){
  double
    j=cJulianFromUtc(utc);

  cJulianTo(j,date);
  return j;
}
/*=========================================================

=========================================================*/
int64 cJulianToUtc(JULIAN &date){
  dbl64
    j=cJulianFrom(date),
    x=(j-2440587.5)*86400.0+__timezone;

  return floorl(x+.5);
}
/*=========================================================

=========================================================*/
void cJulianSet(int year,int month,int day,int hour,int minute,int second,JULIAN &date){
  if(year+month+day+hour+minute+second==0)
    cMemReset(&date,sizeof(JULIAN));
  else{

    date.year=year;
    date.month=month;
    date.day=day;
    date.hour=hour;
    date.minute=minute;
    date.second=second;
    cJulianFrom(date);
  }
}
/*=========================================================

=========================================================*/
void cJulianAddSecond(int s,JULIAN &date){
  double
    dias=floor(s/86400),
    segundos=(s-dias*86400)/86400.;

  cJulianTo(date.julian+dias+segundos,date);
}
/*=========================================================

=========================================================*/
void cJulianNow(JULIAN &date){
  cJulianFromUtc(time(0),date);
}
/*=========================================================

=========================================================*/
int cJulianDiff(JULIAN &d0,JULIAN &d1){
  int64
    i0=cJulianToUtc(d0),
    i1=cJulianToUtc(d1);


  return llabs(i0-i1);
}




