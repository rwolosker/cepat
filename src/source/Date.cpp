/*=========================================================

=========================================================*/
Date::Date(){
  init();
}
Date::Date(double value){
  init();
  fromDouble(value);
}
Date::Date(const Date &value){
  init();
  fromDouble(*(Date*)&value);
}
/*=========================================================

=========================================================*/
Date::~Date(){
  if(julia)
    cJulianFree(julia);
}
/*=========================================================

=========================================================*/
void Date::init(void){
  cDateTzConfig();
  julia=cJulianAlloc();
}
/*=========================================================

=========================================================*/
void Date::update(void){
  cJulianTo(julia->julian,*julia);
}
/*=========================================================

=========================================================*/
Date &Date::now(void){
  cJulianNow(*julia);
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::debug(){
  cJulianDebug(*julia);
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::addSecond(int value){
  cJulianAddSecond(value,*julia);
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setSecond(int value){
  julia->second=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setMinute(int value){
  julia->minute=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setHour(int value){
  julia->hour=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setDay(int value){
  julia->day=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setMonth(int value){
  julia->month=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::setYear(int value){
  julia->year=value;
  cJulianFrom(*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::set(int year,int month,int day,int hour,int minute,int second){
  cJulianSet(year,month,day,hour,minute,second,*julia);
  update();
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::addMinute(int value){
  return addSecond(value*60);
}
/*=========================================================

=========================================================*/
Date &Date::addHour(int value){
  return addMinute(value*60);
}
/*=========================================================

=========================================================*/
Date &Date::addDay(int value){
  return setDay(julia->day+value);
}
/*=========================================================

=========================================================*/
Date &Date::addMonth(int value){
  return setMonth(julia->month+value);
}
/*=========================================================

=========================================================*/
Date &Date::addYear(int value){
  return setYear(julia->year+value);
}
/*=========================================================

=========================================================*/
Date &Date::fromJulian(double value){
  cJulianTo(value,*julia);
  return *this;
}
/*=========================================================

=========================================================*/
Date &Date::fromDouble(double value){
  double
    resto=fmod(value,1);

  value-=resto;
  resto=(float)(resto*pow(10,6));
  if(resto-(long)resto>.5)
    resto+=1;
  resto=long(resto);

  julia->year=(int)(value/pow(10,4));
  julia->month=(int)((value-(julia->year*pow(10,4)))/pow(10,2));
  julia->day=(int)(value-julia->year*pow(10,4)-julia->month*pow(10,2));
  julia->hour=(int)(resto/pow(10,4));
  julia->minute=(int)((resto-(julia->hour*pow(10,4)))/pow(10,2));
  julia->second=(int)(resto-julia->hour*pow(10,4)-julia->minute*pow(10,2));

  return set(
    julia->year,
    julia->month,
    julia->day,
    julia->hour,
    julia->minute,
    julia->second
  );
}
/*=========================================================

=========================================================*/
double Date::get(void){
  return 0
    +julia->year*pow(10,4)
    +julia->month*pow(10,2)
    +julia->day
    +julia->hour*pow(10,-2)
    +julia->minute*pow(10,-4)
    +julia->second*pow(10,-6)
  ;
}
/*=========================================================

=========================================================*/
int Date::getYear(void){return julia->year;}
int Date::getMonth(void){return julia->month;}
int Date::getDay(void){return julia->day;}
int Date::getHour(void){return julia->hour;}
int Date::getMinute(void){return julia->minute;}
int Date::getSecond(void){return julia->second;}
/*=========================================================

=========================================================*/
Date ::operator double(){
  return get();
}
/*=========================================================

=========================================================*/
Date &Date::operator=(double value){
  return fromDouble(value);
}
/*=========================================================

=========================================================*/
Date &Date::operator=(const Date &value){
  return fromDouble(*(Date*)&value);
}
/*=========================================================

=========================================================*/
JULIAN &Date::getJulian(void){
  return *julia;
}
/*=========================================================

=========================================================*/
char Date::equals(const Date &value){
  return !compare(value);
}
/*=========================================================

=========================================================*/
char Date::compare(const Date &value){
  double
    da=julia->julian,
    db=value.julia->julian;

  return da==db?0:da<db?-1:1;
}
/*=========================================================

=========================================================*/
int64 Date::diff(const Date &past){
  return cJulianDiff(*julia,((Date*)&past)->getJulian());
}
/*=========================================================

=========================================================*/
Date &Date::fromUtc(int64 value){
  cJulianTo(cJulianFromUtc(value),*julia);
  return *this;
}
/*=========================================================

=========================================================*/
int64 Date::toUtc(void){
  return cJulianToUtc(*julia);
}
/*=========================================================

=========================================================*/
Date &Date::reset(void){
  return fromDouble(0);
}
/*=========================================================

=========================================================*/
Date &Date::parse(const String &value){
  DateFormat
    df;

  reset();
  #define TEST(f)\
    if(df.parse(*this,PSTRING(value),f))goto FIM;

  TEST("dd/MM/yyyy HH:mm:ss")
  TEST("dd/MM/yyyy HH:mm")
  TEST("dd/MM/yyyy")
  TEST("yyyy-MM-dd HH:mm:ss")
  TEST("yyyy-MM-dd HH:mm")
  TEST("yyyy-MM-dd")

  reset();
  FIM:
  #undef TEST

  return *this;
}
/*=========================================================

=========================================================*/
String Date::format(const String &format){
  return DateFormat().format(*this,PSTRING(format));
}
