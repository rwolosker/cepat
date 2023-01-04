/**********************************************************

**********************************************************/
class FieldDouble:public Field{
  public:
    FieldDouble();
    ~FieldDouble();

    virtual Field &instance(void);
    virtual Field &setInt(int value);
    virtual Field &setDouble(double value);
    virtual Field &setText(cchar *value);
    virtual Field &setBool(char value);
    virtual Field &setChar(char value);
    virtual Field &setNumber(const Number &value);
    virtual Field &setString(const String &value);
    virtual Field &setDate(const Date &value);

    virtual int    getInt(void);
    virtual double getDouble(void);
    virtual char  *getText(void);
    virtual char   getBool(void);
    virtual char   getChar(void);
    virtual Number getNumber(void);
    virtual String getString(void);
    virtual Date   getDate(void);

    virtual String toString(void);
};
