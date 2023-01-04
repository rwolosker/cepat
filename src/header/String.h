/**********************************************************
https://www.purebasic.com/documentation/string/index.html
https://docs.microsoft.com/en-us/office/vba/language/reference/functions-visual-basic-for-applications
https://web.archive.org/web/19990128174241/http://msdn.microsoft.com/
**********************************************************/
class String:public Object{
  protected:
    char
      *str,
      needLen;
    int
      length;

    void init(void);
  public:
    String(void);
    String(cchar *value);
    String(cchar value);
    String(int value);
    String(double value);
    String(const String &value);
    String(String &value);
    ~String();

    String &append(const String &value);
    String &appendf(cchar *format,...);
    char    at(int x);
    String &debug(void);
    char    compare(const String &value,char ignoreCase=0);
    char    equals(const String &value,char ignoreCase=0);
    String  copy(void);
    void    fastTrim(String &string);
    String &fromUtf(const String &value);
    String &fromUtf(void);
    String &insert(int pos,const String &value);
    String &insert(int pos,char c,int size=1);
    String *instance(void);
    int     inStr(int start,const String &value,char ignoreCase=0);
    int     inStr(const String &value,char ignoreCase=0);
    int     inStrRev(const String &value,char ignoreCase=0,int start=-1);
    char    isUtf(void);
    String  lCase(void);
    String  lSet(int size);
    String  lTrim(void);
    String  left(int size);
    int     len(void);
    String  mid(int start,int size=-1);
    String  normalize(void);
    char   *ptr(void);
    String &ref(void);
    String &reLen(void);
    String  replace(const String &query,const String &value="");
    String &reset(void);
    String &resize(int size);
    String  right(int size);
    String  rSet(int size);
    String  rTrim(void);
    String &set(cchar *value);
    String &set(cchar value);
    String &set(int value);
    String &set(double value);
    int     size(void);
    String  space(int size);
 StringList split(const String &query);
    String &sprintf(cchar *format,...);
    String &squeeze(void);
    String &squeeze(void* value,char free=1);
    String  toUtf(void);
    String  trim(void);
    String &truncate(int len);
    String  uCase(void);
    String &operator=(const String &value);
    String &operator=(cchar *value);
    
    String operator+(const String &value);
    String operator+=(const String &value);
    char   operator==(const String &value);
    char   operator!=(const String &value);
    char   operator>=(const String &value);
    char   operator>(const String &value);
    char   operator<=(const String &value);
    char   operator<(const String &value);
    char   operator==(cchar *value);
    char   operator!=(cchar *value);
    char   operator>=(cchar *value);
    char   operator>(cchar *value);
    char   operator<=(cchar *value);
    char   operator<(cchar *value);
    
    CharRef operator[](int x);
    operator char*(void);
};
