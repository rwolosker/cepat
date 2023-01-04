/**********************************************************

**********************************************************/
class TdsResultDone:public TdsAction{
    protected:
      int
        affectedInsert,
        affectedDelete,
        affectedUpdate;

      void initTdsResultDone(void);
    public:
      TdsResultDone();
      ~TdsResultDone();
      void reset(void);
      char action(uchar type,void **arg);
      int getAffectedInsert(void);
      int getAffectedUpdate(void);
      int getAffectedDelete(void);
};
