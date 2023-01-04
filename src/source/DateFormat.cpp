/*=========================================================

=========================================================*/
void DateFormat::init(void){
}
DateFormat::DateFormat(){
  init();
}
DateFormat::~DateFormat(){
}
/*=========================================================

=========================================================*/
char DateFormat::isDate(double date){
  Date
    d=date;

  return isDate(d);
}
char DateFormat::isDate(Date &date){
  int
    ok,
    ano=date.getYear(),
    mes=date.getMonth(),
    dia=date.getDay(),
    hora=date.getHour(),
    minuto=date.getMinute(),
    segundo=date.getSecond();

  ok=1
    &&(dia>=1    &&dia<=31)
    &&(mes>=1    &&mes<=12)
    &&(ano>=0    &&ano<=2100)
    &&(hora>=0   &&hora<=23)
    &&(minuto>=0 &&minuto<=59)
    &&(segundo>=0&&segundo<=59)
    &&((dia!=30)  ||(dia==30&&mes!=2))
    &&((dia!=31   ||(dia==31&&(mes==1||mes==3||mes==5||mes==7||mes==8||mes==10||mes==12))))
    &&((dia!=29   ||(dia==29&&mes!=2)||(dia==29&&mes==2&&ano%4==0&&ano%100!=0)))
  ;
  return ok;
}
/*=========================================================
https://docs.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html
=========================================================*/
char DateFormat::parse(Date &date,cchar *text,cchar *format){
  String
    txt=String(text).trim(),
    pic=String(format).trim();
  char
    ok=0;

  date.fromDouble(0);
  if(!pic.len())
    pic="dd/MM/yyyy HH:mm:ss";
  //if(pic.len()!=txt.len())
  //  goto FIM;
  if(!txt.len()||txt.len()!=pic.len())
    goto FIM;
  for(int i=0,p=0,c=0;i<=pic.len();i++){
    if(!c)
      c=pic[i];
    if(pic[i]!=c){
      String
        parte=pic.mid(p,i-p);

      if(!parseResolve(parte,txt,date))
        goto FIM;
      c=pic[i];
      p=i;
    }
  }
  if(!isDate(date))
    goto FIM;
  ok=1;
  FIM:
  if(!ok)
    date.fromDouble(0);
  return ok;
}
/*=========================================================

=========================================================*/
String DateFormat::format(const Date &date,cchar *format){
  int
    len=cStrLen(format);
  String
    txt,
    parte;
    //pic=String(format).trim();
  Date
    &data=PDATE(date);
  char
    *pic;

  if(!data.getJulian().julian)
    return "";

  if(len)
    pic=PCHAR(format);
  else{
    pic=PCHAR("dd/MM/yyyy HH:mm:ss");
    len=19;
  }

  parte.resize(len+1);

  for(int i=0,p=0,c=0;i<=len;i++){
    if(!c)
      c=pic[i];
    if(pic[i]!=c){
      cStrMid(
        pic,
        p,i-p,
        parte.ptr()
      );
      formatResolve(parte,txt,data);
      c=pic[i];
      p=i;
    }
  }

  return txt;
}
/*=========================================================

=========================================================*/
void DateFormat::formatResolve(String &parte,String &txt,Date &date){
  switch(parte.at(0)){
    case 'd':
      diaPut(parte,txt,date);
      break;
    case 'M':
      mesPut(parte,txt,date);
      break;
    case 'y':
      anoPut(parte,txt,date);
      break;
    case 'H':
      horaPut(parte,txt,date);
      break;
    case 'm':
      minutoPut(parte,txt,date);
      break;
    case 's':
      segundoPut(parte,txt,date);
      break;
    default:
      txt.append(parte);
      break;
  }
}
/*=========================================================

=========================================================*/
char DateFormat::parseResolve(String &parte,String &txt,Date &date){
  switch(parte.at(0)){
    case 'd':
      return diaSet(parte,txt,date);
    case 'M':
      return mesSet(parte,txt,date);
    case 'y':
      return anoSet(parte,txt,date);
    case 'H':
      return horaSet(parte,txt,date);
    case 'm':
      return minutoSet(parte,txt,date);
    case 's':
      return segundoSet(parte,txt,date);
    case '/':
      return separadorSet('/',parte,txt,date);
    case '-':
      return separadorSet('-',parte,txt,date);
    case ' ':
      return separadorSet(' ',parte,txt,date);
    case ':':
      return separadorSet(':',parte,txt,date);
  }
  return 0;
}
/*=========================================================

=========================================================*/
char DateFormat::diaSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setDay(Number(tmp).asInt());
  return 1;
}
char DateFormat::mesSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setMonth(Number(tmp).asInt());
  return 1;
}
char DateFormat::anoSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setYear(Number(tmp).asInt());
  return 1;
}
char DateFormat::horaSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setHour(Number(tmp).asInt());
  return 1;
}
char DateFormat::minutoSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setMinute(Number(tmp).asInt());
  return 1;
}
char DateFormat::segundoSet(String &parte,String &txt,Date &date){
  int
    len=parte.len();
  String
    tmp=txt.mid(0,len);

  txt=txt.mid(len);
  if(!Number().isNumber(tmp))
    return 0;
  date.setSecond(Number(tmp).asInt());
  return 1;
}
char DateFormat::separadorSet(char s,String &parte,String &txt,Date &date){
  if(parte[0]!=txt[0])
    return 0;
  else
    txt=txt.mid(1);
  return 1;
}
/*=========================================================

=========================================================*/
char DateFormat::diaPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getDay()));
  return 1;
}
char DateFormat::mesPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getMonth()));
  return 1;
}
char DateFormat::anoPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getYear()));
  return 1;
}
char DateFormat::horaPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getHour()));
  return 1;
}
char DateFormat::minutoPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getMinute()));
  return 1;
}
char DateFormat::segundoPut(String &parte,String &txt,Date &date){
  txt.append(String().sprintf("%0*d",parte.len(),date.getSecond()));
  return 1;
}
