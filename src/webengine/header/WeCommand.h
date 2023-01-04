/**********************************************************

**********************************************************/
class WeCommand:public Object,public ExtMap,public ExtFactory{
  protected:
    GcObject
      gco;
    char
      noFactory;
  
    char *paramEnd(char *p,char **p0,String &buffer);
    char  paramAdd(String &buffer);
    char  paramAddNoFactory(String &buffer);
    void  resolveEvaluate(WeParam &param);
    void  initWeCommand(void);
  public:
    WeCommand();
    ~WeCommand();
    void       debug(void);
    WeParam   &get(int x);
    int        len(void);
    void       parse(const String &cmd);
    WeCommand &remove(WeParam &param);
    void       reset(void);
    WeCommand &setNoFactory(char noFactory);
};
