/**********************************************************

**********************************************************/
class WeParam:public Object,public ExtMap,public ExtFactory{
  protected:
    String
      strValue;
    Number
      numValue;
    char
      type,mode,typeReturn;
    int
      size;
    Object
      *bean;
    
    USED void init(void);
    USED void setString(String &buffer);
    USED void setLiteral(String &buffer);
    USED void modeSet(String &buffer);
    USED void literalResolve(String &buffer);
    USED void modeResolve(void);
    USED void modeToInt(void);
    USED void modeToDouble(void);
    USED void modeToString(void);
    USED void modeToSet(void);
    USED void functionResolve(String &buffer);
  public:
    USED WeParam();
    USED ~WeParam();
    USED void    set(String &buffer);
    USED String  getStrValue(void);
    USED Number  getNumValue(void);
    USED char    getType(void);
    USED char    getTypeReturn(void);
    USED char    getMode(void);
    USED char   *toCall(char *p);
    USED Object *getBean(void);
    USED void    setTypeReturn(char value);
    USED WeParam*instance(void);
};
