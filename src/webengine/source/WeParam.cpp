/*=========================================================

=========================================================*/
void WeParam::init(void){
  size=type=mode=typeReturn=0;
  bean=0;
}
WeParam::WeParam(){
  init();
}
WeParam::~WeParam(){
}
/*=========================================================

=========================================================*/
String  WeParam::getStrValue(void){return strValue;}
Number  WeParam::getNumValue(void){return numValue;}
Object *WeParam::getBean(void){return bean;}
char    WeParam::getType(void){return type;}
char    WeParam::getMode(void){return mode;}
char    WeParam::getTypeReturn(void){return typeReturn;}
void    WeParam::setTypeReturn(char value){typeReturn=value;}
/*=========================================================

=========================================================*/
void WeParam::set(String &buffer){
  if(FOUND(buffer.inStr("'")))
    setString(buffer);
  else
    setLiteral(buffer);
  modeResolve();
  if(type==C_SET)
    strValue=buffer;
}
/*=========================================================

=========================================================*/
void WeParam::setString(String &buffer){
  StringList
    item=buffer.split("'");
  
  type=C_PCHAR;
  strValue=item[1];
}
/*=========================================================

=========================================================*/
void WeParam::modeSet(String &buffer){
  mode=0;
  switch(buffer.at(0)){
    case '%':
      mode='%';
      break;
    case '.':
      mode='.';
      break;
    case '$':
      mode='$';
      break;
    case '=':
      mode='=';
      break;
  }
  if(mode)
    buffer=buffer.mid(1);
}
/*=========================================================

=========================================================*/
WeParam *WeParam::instance(void){
  return new(cMemAlloc(sizeof(WeParam)))WeParam();
}
/*=========================================================

=========================================================*/
void WeParam::literalResolve(String &buffer){
  MapItem
    &item=getMap().get(buffer);
  
  if(item.isNull()){
    functionResolve(buffer);
    return;
  }
  switch(item.getType()){
    case C_PCHAR:
      type=C_PCHAR;
      strValue=item.asStr();
      return;
    case C_NUMBER:
      type=C_NUMBER;
      numValue=item.asNumber();
      return;
    case C_DOUBLE:
      type=C_DOUBLE;
      numValue=item.asNumber();
      return;
    case C_INT:
      type=C_INT;
      numValue=item.asNumber();
      return;
    case C_OBJ:
      type=C_OBJ;
      numValue=item.asNumber();
      return;
    case C_PTR:
      type=C_PTR;
      numValue=item.asNumber();
      return;
  }
}
/*=========================================================

=========================================================*/
void WeParam::setLiteral(String &buffer){
  buffer=buffer
    .replace("\r","")
    .replace("\n","")
    .replace("\t","")
    .trim();
  
  if(!buffer.len()){
    type=C_NULL;
    return;
  }

  modeSet(buffer);
  if(mode=='=')
    return;

  if(Number().isNumber(buffer)){
    type=FOUND(buffer.inStr("."))
      ?C_DOUBLE
      :C_INT;
    numValue.parse(buffer);
    return;
  }
  literalResolve(buffer);
}
/*=========================================================

=========================================================*/
void WeParam::modeResolve(void){
  switch(mode){
    case '%':
      modeToInt();
      break;
    case '.':
      modeToDouble();
      break;
    case '$':
      modeToString();
      break;
    case '=':
      modeToSet();
      break;
  }
}
/*=========================================================

=========================================================*/
void WeParam::modeToInt(void){
  switch(type){
    case C_NULL:
      numValue=0;
      break;
    case C_PCHAR:
      numValue=Number(strValue);
      break;
    case C_INT:
    case C_NUMBER:
    case C_DOUBLE:
      numValue=numValue.asInt();
      break;
    case C_PTR:
    case C_OBJ:
      break;
  }
  type=C_INT;
}
/*=========================================================

=========================================================*/
void WeParam::modeToDouble(void){
  switch(type){
    case C_NULL:
      numValue=0;
      break;
    case C_PCHAR:
      numValue=Number(strValue);
      break;
    case C_INT:
    case C_NUMBER:
    case C_DOUBLE:
    case C_PTR:
    case C_OBJ:
      break;
  }
  type=C_DOUBLE;
}
/*=========================================================

=========================================================*/
void WeParam::modeToString(void){
  switch(type){
    case C_NULL:
    case C_PCHAR:
      break;
    case C_INT:
      strValue=String().sprintf("%d",numValue.asInt());
      break;
    case C_NUMBER:
    case C_DOUBLE:
      strValue=String().sprintf("%f",numValue.asDouble());
      break;
    case C_PTR:
    case C_OBJ:
      strValue=numValue.toHex();
      break;
  }
  type=C_PCHAR;
}
/*=========================================================

=========================================================*/
void WeParam::modeToSet(void){
  type=C_SET;
}
/*=========================================================

=========================================================*/
void WeParam::functionResolve(String &buffer){
  if(!FOUND(buffer.inStr(':')))
    type=C_NULL;
  else{
    Split
      item=Split().split(buffer,':');
    
    typeReturn=Type().type(item[1]);
    strValue=buffer;
    item=Split().split(item[0],'.');
    if(item.len()==1){
      void
        *ptr=0;

      0
        ||(ptr=getMap().get(item[0]).asPtr())
        ||(ptr=getFactory().getReflection().methodAddress(item[0]));
      
      if(ptr){
        numValue.fromPtr(ptr);
        type=C_FUNCTION;
        return;
      }
    }
    if(item.len()==2){
      String
        beanName,
        clazz;
      Object
        *bean;
      void
        *method;
      
      bean=getMap().get(item[0]).asObj();
      if(!bean)
        return;
      beanName=getFactory().getReflection().className(bean);
      if(!beanName.len()){
        cPrintf("ERRO: WeParam:functionResolve [%s].",item[0].ptr());
        exit(1);
      }
      method=getFactory().getReflection().methodAddress(
        String().sprintf(
          "%s::%s",
          beanName.ptr(),
          item[1].ptr()
      ));
      if(!method){
        cPrintf("ERRO: WeParam:functionResolve [%s].",item[1].ptr());
        exit(1);
      }
      numValue.fromPtr(method);
      type=C_METHOD;
      this->bean=bean;
      
      return;
    }
  }
}
/*=========================================================

=========================================================*/
char *WeParam::toCall(char *p){
  switch(type){
    case C_INT:
      *(int*)p=numValue.asInt();
      p+=4;
      break;
    case C_DOUBLE:
      *(double*)p=numValue.asDouble();
      p+=8;
      break;
    case C_PCHAR:
      *(char**)p=strValue.ptr();
      p+=4;
      break;
    case C_FUNCTION:
    case C_METHOD:
     break;
    case C_PTR:
    case C_OBJ:
      *(void**)p=numValue.asPtr();
      p+=4;
      break;
    default:
      cPrintf("ERRO: WeParam.toCall nao sei [%d].",type);
      break;
  }
  return p;
}
