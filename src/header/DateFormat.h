/**********************************************************

**********************************************************/
class DateFormat:public Object{
  protected:
    void init(void);
    char parseResolve(String &parte,String &txt,Date &date);
    void formatResolve(String &parte,String &txt,Date &date);
    char diaSet(String &parte,String &txt,Date &date);
    char mesSet(String &parte,String &txt,Date &date);
    char anoSet(String &parte,String &txt,Date &date);
    char horaSet(String &parte,String &txt,Date &date);
    char minutoSet(String &parte,String &txt,Date &date);
    char segundoSet(String &parte,String &txt,Date &date);
    char separadorSet(char s,String &parte,String &txt,Date &date);
    char diaPut(String &parte,String &txt,Date &date);
    char mesPut(String &parte,String &txt,Date &date);
    char anoPut(String &parte,String &txt,Date &date);
    char horaPut(String &parte,String &txt,Date &date);
    char minutoPut(String &parte,String &txt,Date &date);
    char segundoPut(String &parte,String &txt,Date &date);
  public:
    DateFormat();
    ~DateFormat();
    char   isDate(Date &date);
    char   isDate(double date);
    String format(const Date &date,cchar *format="dd/MM/yyyy HH:mm:ss");
    char   parse(Date &date,cchar *text,cchar *format="dd/MM/yyyy HH:mm:ss");
};
