/*=========================================================

=========================================================*/
WeCommand::WeCommand(){
  initWeCommand();
}
WeCommand::~WeCommand(){
  reset();
}
void WeCommand::initWeCommand(void){
  noFactory=0;
}
/*=========================================================

=========================================================*/
WeCommand &WeCommand::setNoFactory(char noFactory){
  this->noFactory=noFactory;
  return *this;
}
/*=========================================================

=========================================================*/
void WeCommand::reset(void){
  gco.reset();
}
/*=========================================================

=========================================================*/
char *WeCommand::paramEnd(char *p,char **p0,String &buffer){
  int
    len=p-*p0;
  
  if(buffer.size()<=len)
    buffer.resize(len);
  cMemCpy(*p0,len,buffer.ptr());
  buffer[len]=0;
  *p0=p+1;
  
  paramAdd(buffer);
  
  return p;
}
/*=========================================================
MODIFICADORES
  % int
  $ pchar
  . double
  98  int
  98. double
 
=========================================================*/
void WeCommand::parse(const String &cmd){
  String
    buffer;
  char
    *p0=PSTRING(cmd).ptr(),
    *p=p0,
    dentro=0;
  
  reset();
  //cPrintf("cmd: %s\n",p);
  while(p){
    if(!*p){
      p=paramEnd(p,&p0,buffer);
      break;
    }
    if(*p=='\'')
      dentro=!dentro;
    if(*p==','&&!dentro){
      p=paramEnd(p,&p0,buffer);
      goto NEXT;
    }
    NEXT:
    p++;
  }
  if(len()==1)
    resolveEvaluate(get(0));
}
/*=========================================================

=========================================================*/
void WeCommand::resolveEvaluate(WeParam &param){
  switch(param.getType()){
    case C_METHOD:
    case C_FUNCTION:
      break;
    default:
      param.setTypeReturn(C_EVALUATE);
      break;
  }
}
/*=========================================================

=========================================================*/
char WeCommand::paramAddNoFactory(String &buffer){
  WeParam
    &param=*WeParam().instance();

  gco.add(&param);
  param.setMap(getMap());
  param.set(buffer);
  return 1;
}
/*=========================================================

=========================================================*/
char WeCommand::paramAdd(String &buffer){
  if(noFactory)
    return paramAddNoFactory(buffer);
  else{
    BEAN(WeParam,param,0)

    gco.add(&param);
    param.set(buffer);

    return 1;
  }
}
/*=========================================================

=========================================================*/
void WeCommand::debug(void){
  cPrintf("params: %d\n",gco.len());
  for(int i=0;i<gco.len();i++){
    WeParam
      &param=*(WeParam*)gco.get(i);
    String
      value;
    
    cPrintf("[%d]\n",i);
    cPrintf("  type  : %s\n",Type().name(param.getType()).ptr());
    if(param.getTypeReturn())
      cPrintf("  return: %s\n",Type().name(param.getTypeReturn()).ptr());
    cPrintf("  mode  : %c\n",param.getMode()?param.getMode():'0');
    switch(param.getType()){
      case C_FUNCTION:
      case C_METHOD:
        value=param.getStrValue();
        break;
      case C_PCHAR:
        value=param.getStrValue();
        break;
      case C_INT:
        value=String().sprintf("%d",param.getNumValue().asInt());
        break;
      case C_DOUBLE:
      case C_NUMBER:
        value=param.getNumValue().toString();
        break;
      case C_OBJ:
      case C_PTR:
        value=param.getNumValue().toHex();
        break;
      case C_SET:
        value=param.getStrValue();
        break;
    }
    cPrintf("  value: %s\n",value.ptr());
  }
}
/*=========================================================

=========================================================*/
int WeCommand::len(void){
  return gco.len();
}
/*=========================================================

=========================================================*/
WeParam &WeCommand::get(int x){
  return*(WeParam*)gco.get(x);
}
/*=========================================================

=========================================================*/
WeCommand &WeCommand::remove(WeParam &param){
  gco.remove(&param);
  return *this;
}
