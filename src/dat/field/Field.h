/**********************************************************

**********************************************************/
class Field:public Object{
  protected:
    ByteArray
      *stream;
    String
      name;
    char
      type;
    int
      size,
      start,
      col;
    GcObject
      gco;
    void   initField(void);
    void  *pstream(void);
  public:
    Field();
    ~Field();
    Field &reset(void);
    Field &setName(cchar *name);
    String getName(void);
    char   getType(void);
    Field &setSize(int size);
    int    getSize(void);
    Field &setStart(int start);
    int    getStart(void);
    Field &setCol(int col);
    int    getCol(void);
    Field &setStream(ByteArray &stream);
    Field &debug(void);
    Field &clone(void);
    String toDef(void);

    virtual Field &instance(void)=0;
    virtual Field &setInt(int value)=0;
    virtual Field &setDouble(double value)=0;
    virtual Field &setText(cchar *value)=0;
    virtual Field &setBool(char value)=0;
    virtual Field &setChar(char value)=0;
    virtual Field &setNumber(const Number &value)=0;
    virtual Field &setString(const String &value)=0;
    virtual Field &setDate(const Date &value)=0;

    virtual int    getInt(void)=0;
    virtual double getDouble(void)=0;
    virtual char  *getText(void)=0;
    virtual char   getBool(void)=0;
    virtual char   getChar(void)=0;
    virtual Number getNumber(void)=0;
    virtual String getString(void)=0;
    virtual Date   getDate(void)=0;

    virtual char  *getPtr(void);

    virtual String toString()=0;
};
