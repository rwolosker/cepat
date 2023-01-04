/**********************************************************

**********************************************************/
typedef struct PACK WE_PARSE{
  String
    *text;
  char
    *p0, //inicio stream
    *p,  //carro
    *ps, //inicio {
    *pe, //fim }
    in   //in string
  ;
}WE_PARSE;

class WeParse:public Object,public ExtMap,public ExtConfigure,public ExtFactory{
  protected:
    WeCommand
      *cmd;
    WeExec
      *exe;
    GcObject
      *gco;

    void init(void);
    void paramEnd(WE_PARSE &p);
  public:
    USED  WeParse();
    USED ~WeParse();
    USED void     configure(Map &map);
    USED void     configureFast(Map &map,WeCommand &cmd,WeExec &exe,GcObject &gco);
    USED WeParse &debug(void);
    USED String  &parse(String &text);
};
