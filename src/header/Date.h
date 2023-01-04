/**********************************************************

**********************************************************/
class Date:public Object{
  protected:
    JULIAN
      *julia;

    void init(void);
    void update(void);
  public:
    Date();
    Date(double value);
    Date(const Date &value);
    ~Date();

    Date &addSecond(int value);
    Date &addMinute(int value);
    Date &addHour(int value);
    Date &addDay(int value);
    Date &addMonth(int value);
    Date &addYear(int value);

    Date &debug(void);
    Date &now(void);
    Date &reset(void);

    Date &set(int year=0,int month=0,int day=0,int hour=0,int minute=0,int second=0);

    Date &setSecond(int value);
    Date &setMinute(int value);
    Date &setHour(int value);
    Date &setDay(int value);
    Date &setMonth(int value);
    Date &setYear(int value);

    int getYear(void);
    int getMonth(void);
    int getDay(void);
    int getHour(void);
    int getMinute(void);
    int getSecond(void);

    Date &fromDouble(double value);
    Date &fromJulian(double value);
    Date &fromUtc(int64 value);

    JULIAN &getJulian(void);
    double  get(void);
    char    equals(const Date &value);
    char    compare(const Date &value);
    int64   diff(const Date &past);
    int64   toUtc(void);

    operator double();
    Date &operator=(double value);
    Date &operator=(const Date &value);

    Date &parse(const String &value);
    String      format(const String &format="dd/MM/yyyy HH:mm:ss");
};
