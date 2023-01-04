/*=========================================================

=========================================================*/
WeExec::WeExec(){
}
WeExec::~WeExec(){
}
/*=========================================================

=========================================================*/
char WeExec::executeInt(WeParam &param,C_CALL call,String &res){
  if(param.getType()==C_FUNCTION){
    int
      (*f)(C_CALL)=0,
      rtn;
    
    *(void**)&f=param.getNumValue().asPtr();
    rtn=f(call);
    res.sprintf("%d",rtn);
    return 1;
  }
  if(param.getType()==C_METHOD){
    int
      (Object::*f)(C_CALL)=0,
      rtn;

    *(void**)&f=param.getNumValue().asPtr();
    rtn=(param.getBean()->*f)(call);
    res.sprintf("%d",rtn);
    return 1;
  }
  return 0;
}
/*=========================================================

=========================================================*/
char WeExec::executeVoid(WeParam &param,C_CALL call,String &res){
  if(param.getType()==C_FUNCTION){
    void
      (*f)(C_CALL)=0;
      
    
    *(void**)&f=param.getNumValue().asPtr();
    f(call);
    res.reset();
    return 1;
  }
  if(param.getType()==C_METHOD){
    void
      (Object::*f)(C_CALL)=0;

    *(void**)&f=param.getNumValue().asPtr();
    (param.getBean()->*f)(call);
    res.reset();
    return 1;
  }
  return 0;
}
/*=========================================================

=========================================================*/
char WeExec::executeStr(WeParam &param,C_CALL call,String &res){
  if(param.getType()==C_FUNCTION){
    String
      (*f)(C_CALL)=0;

    *(void**)&f=param.getNumValue().asPtr();
    res=f(call);
    return 1;
  }
  if(param.getType()==C_METHOD){
    String
      (Object::*f)(C_CALL)=0;

    *(void**)&f=param.getNumValue().asPtr();
    res=(param.getBean()->*f)(call);
    return 1;
  }

  return 0;
}
/*=========================================================

=========================================================*/
char WeExec::executeEvaluate(WeParam &param,C_CALL call,String &res){
  res.reset();
  switch(param.getType()){
    case C_PCHAR:
      res=param.getStrValue();
      break;
    case C_INT:
      res.sprintf("%d",param.getNumValue().asInt());
      break;
  }
  return 1;
}
/*=========================================================

=========================================================*/
char WeExec::executeSet(WeCommand &wcmd){
  WeParam
    &param=wcmd.get(0);
  String
    key=param.getStrValue(),
    res;
  char
    ok=execute(wcmd.remove(param),res);

  if(!ok)
    return 0;
  else{
    executeSetToMap(res,wcmd.get(0).getTypeReturn(),key);
    return 1;
  }
}
/*=========================================================

=========================================================*/
void WeExec::executeSetToMap(String &res,char type,String &key){
  switch(type){
  case C_PCHAR:
    getMap().setValue(key,res);
    break;
  case C_INT:
    getMap().setValue(key,Number().parse(res).asInt());
    break;
  default:
    cPrintf("ERRO: WeExec.executeSetToMap nao sei.\n");
    break;
  }
}
/*=========================================================

=========================================================*/
char WeExec::execute(WeCommand &wcmd,String &res){
  res.reset();
  if(!wcmd.len())
    return 0;
  if(wcmd.len()==1&&wcmd.get(0).getType()==C_NULL)
    return 0;
  if(wcmd.get(0).getType()==C_SET)
    return executeSet(wcmd);
  {
    C_CALL
      call;
    char
      *p0=PCHAR(&call),
      *p=p0;
    int
      x=-1;
    
    cMemReset(p,sizeof(C_CALL));
    for(int i=0;i<wcmd.len();i++){
      if(wcmd.get(i).getType()==C_NULL){//parametro nulo.
        p+=4;
        continue;
      }
      p=wcmd.get(i).toCall(p);
      if(wcmd.get(i).getTypeReturn())
        x=i;
    }
    if(!FOUND(x))
      return 0;
    {
      WeParam
        &param=wcmd.get(x);
      char
        ok=0;
      
      switch(param.getTypeReturn()){
        case C_INT:
          ok=executeInt(param,call,res);
          break;
        case C_VOID:
          ok=executeVoid(param,call,res);
          break;
        case C_PCHAR:
          ok=executeStr(param,call,res);
          break;
        case C_EVALUATE:
          ok=executeEvaluate(param,call,res);
          break;
        default:
          cPrintf("ERRO: WeExec.execute nao sei.\n");
          break;
      }
      if(!ok)
        return 0;
    }
  }
  return 1;
}
