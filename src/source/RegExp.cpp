/*=========================================================

=========================================================*/
void RegExp::initRegExp(void){
  regra=0;
}
RegExp::RegExp(){
  initRegExp();
}
RegExp::~RegExp(){
  reset();
}
RegExp &RegExp::reset(void){
  source.reset();
  if(regra)
    free(regra);
  initRegExp();
  return *this;
}
/*=========================================================

=========================================================*/
RegExp &RegExp::create(const String &value){
  reset();
  source=value;
  regra=load(value);
  return *this;
}
/*=========================================================

=========================================================*/
REGEXP *RegExp::load(const String &value){
  String
    compile=value,
    buffer;
  char
    *p,*p0;
  int
    len=0;
  REGEXP
    *regra=0;

  compile=compile
    .replace('\t')
    .replace('\r')
    .replace('\n')
    .replace("| ","|")
    .replace(" |","|")
    .replace("& ","&")
    .replace(" &","&")
    .replace(") ",")")
    .replace(" )",")")
    .replace("( ","(")
    .replace(" (","(")
    .trim();
  p=p0=compile.ptr();


  while(p&&*p){
    if(*p=='|'){
      //vbmid(p0,0,p-p0,buffer);
      buffer=p0;buffer[p-p0]=0;
      p0=p+1;
      regra=(REGEXP*)cMemRealloc(regra,(len+1)*sizeof(REGEXP));
      regra[0].len=len+1;
      regra[len].type=REG_EXP_OR;
      regra[len].value=cStrCopy(buffer.trim().ptr(),0);
      len++;
    }
    if(*p=='&'){
      //vbmid(p0,0,p-p0,buffer);
      buffer=p0;buffer[p-p0]=0;
      p0=p+1;
      regra=(REGEXP*)cMemRealloc(regra,(len+1)*sizeof(REGEXP));
      regra[0].len=len+1;
      regra[len].type=REG_EXP_AND;
      regra[len].value=cStrCopy(buffer.trim().ptr(),0);
      len++;
    }
    if(*p=='('){
      while(p&&*p&&*p!=')')
        p++;
      //vbmid(p0,1,p-p0-1,buffer);
      buffer=String(p0).mid(1,p-p0-1);
      regra=(REGEXP*)cMemRealloc(regra,(len+1)*sizeof(REGEXP));
      regra[0].len=len+1;
      regra[len].type=REG_EXP_COM;
      regra[len].value=cStrCopy(buffer.ptr(),0);
      regra[len].composition=load((char*)regra[len].value);
      {
        char
          next=*(p+1);

        if(!next)
          regra[len].type=REG_EXP_END;
        else{
          p+=1;
          switch(next){
            case '|':
              regra[len].type=REG_EXP_OR;
              break;
            case '&':
              regra[len].type=REG_EXP_AND;
              break;
          }
        }
      }
      len++;
      p0=p+1;
    }
    p++;
  }
  if(*(p-1)!=')'){
    regra=(REGEXP*)cMemRealloc(regra,(len+1)*sizeof(REGEXP));
    regra[0].len=len+1;
    regra[len].type=REG_EXP_END;
    regra[len].value=cStrCopy(p0,0);
    len++;
  }
  return regra;
}
/*=========================================================

=========================================================*/
RegExp &RegExp::debug(void){
  dump(regra);
  return *this;
}
/*=========================================================

=========================================================*/
void RegExp::dump(REGEXP *regra){
  if(regra){
    int
      i;

    for(i=0;i<regra[0].len;i++){
      cPrintf("%d %s\n",regra[i].type,regra[i].value);
      if(regra[i].composition){
        REGEXP
          *composition=(REGEXP*)regra[i].composition;
        int
          i;

        for(i=0;i<composition[0].len;i++)
          cPrintf("  %d %s\n",composition[i].type,composition[i].value);
      }
    }
  }
}
/*=========================================================

=========================================================*/
REGEXP *RegExp::free(REGEXP *regra){
  if(regra){
    int
      len=regra[0].len;

    for(int i=0;i<len;i++){
      cMemFree(regra[i].value);
      free(regra[i].composition);
    }
    cMemFree(regra);
  }
  return 0;
}
/*=========================================================

=========================================================*/
char RegExp::matchProcess(REGEXP *regra){
  int
    res,i,len=regra[0].len;

  if(len==1)
    res=regra[0].res;
  else
    for(i=1;i<len;i++){
      char
        resa=regra[i-1].res,
        resb=regra[i+0].res,
        resc=regra[i-1].type==REG_EXP_AND
          ?resa&&resb
          :resa||resb;

      if(i==1)
        res=resc;
      else
        res=regra[i-1].type==REG_EXP_AND
          ?res&&resc
          :res||resc;
    }
  return res?1:0;
}
/*=========================================================

=========================================================*/
void RegExp::matchComposite(REGEXP *regra,const String &value){
  int
    i,len=regra[0].len;

  for(i=0;i<len;i++)
    if(!regra[i].composition)
      regra[i].res=FOUND(PSTRING(value).inStr((char*)regra[i].value));
    else{
      matchComposite(regra[i].composition,value);
      regra[i].res=matchProcess(regra[i].composition);
    }
}
/*=========================================================

=========================================================*/
char RegExp::match(const String &value){
  if(!regra)
    return 0;
  matchComposite(regra,value);
  return matchProcess(regra);
}
