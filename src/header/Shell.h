/**********************************************************

**********************************************************/
class Shell:public Object{
  public:
    Shell();
    ~Shell();
    char exec(const String &cmd,File &out);
};
