/**********************************************************

**********************************************************/
class ExtReflection{
  protected:
    Reflection
      *reflection;

    void initExtReflection(void);
  public:
    ExtReflection();
    virtual ~ExtReflection();
    Reflection &getReflection();
    void setReflection(Reflection &reflection);
};
