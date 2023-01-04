/**********************************************************

**********************************************************/
class WeTag:public WeBean{
  public:
    USED WeTag();
    USED virtual ~WeTag();

    USED String echo(cchar *value);
    USED void   setStr(cchar *name,cchar *value);
    USED void   setInt(cchar *name,int value);
};
