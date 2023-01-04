/**********************************************************

**********************************************************/
class ExtFactory{
  protected:
    Factory
      *factory;
    
    void initExtFactory(void);
  public:
    ExtFactory();
    virtual ~ExtFactory();
    Factory &getFactory();
    void setFactory(Factory &factory);
};
