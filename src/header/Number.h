/**********************************************************

**********************************************************/
class Number:public Object{
  protected:
    double
      value;

    void init(void);
  public:
    Number();
    Number(double value);
    Number(cchar *value);
    ~Number();

    int      asInt(void);
    double   asDouble(void);
    void    *asPtr(void);
    Number   copy(void);
    void     debug();
    Number  &endian(int size);
    Number  &fromBinary(cchar *value);
    Number  &fromHex(cchar *value);
    Number  &fromPtr(void *value);
    double   get(void);
    char     isNumber(cchar *value);
    Number  &parse(cchar *value);
    Number  &set(double value);
    String   toBinary(int minSize=-1);
    String   toHex(void);
    String   toString(void);

    Number  &operator=(double value);
    Number  operator+(double value);
    Number  operator-(double value);
    Number  operator*(double value);
    Number  operator/(double value);
    Number  operator%(double value);
    Number  &operator+=(double value);
    Number  &operator-=(double value);
    Number  &operator*=(double value);
    Number  &operator/=(double value);
    Number  &operator%=(double value);
    char     operator==(double value);
    char     operator==(int value);
    char     operator!=(double value);
    char     operator>(double value);
    char     operator>=(double value);
    char     operator<(double value);
    char     operator<=(double value);
    Number  &operator++(void);
    Number  &operator--(void);

    operator int(void);
    operator dbl64(void);
    operator uchar(void);
    operator char(void);
    operator short(void);
    operator int64(void);
    operator long(void);
    operator float(void);
    operator double(void);
    operator bool(void);

    Number *instance(void);
};
