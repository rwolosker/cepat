/**********************************************************

**********************************************************/
class CharRef:public Object{
  protected:
    char
      *ptr;
  public:
    CharRef();
    ~CharRef();
    void set(char *ptr);
    char get(void);
    operator char(void);
    char operator=(char c);
    
    char operator==(char value);
    char operator!=(char value);
    char operator>=(char value);
    char operator>(char value);
    char operator<=(char value);
    char operator<(char value);

    char operator==(int value);
    char operator!=(int value);
    char operator>=(int value);
    char operator>(int value);
    char operator<=(int value);
    char operator<(int value);
};

