/**********************************************************

**********************************************************/
class WeExec:public Object,public ExtMap{
  protected:
    char executeInt(WeParam &param,C_CALL call,String &res);
    char executeVoid(WeParam &param,C_CALL call,String &res);
    char executeStr(WeParam &param,C_CALL call,String &res);
    char executeEvaluate(WeParam &param,C_CALL call,String &res);
    char executeSet(WeCommand &wcmd);
    void executeSetToMap(String &res,char type,String &key);
  public:
    WeExec();
    ~WeExec();
    char execute(WeCommand &wcmd,String &res);
};
