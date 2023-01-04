/*=========================================================

=========================================================*/
WeParse::WeParse(){
  init();
}
WeParse::~WeParse(){
}
/*=========================================================

=========================================================*/
void WeParse::init(void){
  gco=0;
  cmd=0;
  exe=0;
}
/*=========================================================

=========================================================*/
void WeParse::configure(Map &map){
  getMap().get("app.gco").pObj(&gco);
  getFactory().getBean(0,"WeCommand",&cmd);
  gco->add(cmd);
  getFactory().getBean(0,"WeExec",&exe);
  gco->add(exe);
}
/*=========================================================

=========================================================*/
void WeParse::configureFast(Map &map,WeCommand &cmd,WeExec &exe,GcObject &gco){
  this->map=&map;
  this->cmd=&cmd;
  this->exe=&exe;
  this->gco=&gco;
}
/*=========================================================

=========================================================*/
void WeParse::paramEnd(WE_PARSE &p){
  if(!p.ps||p.in)
    return;
  p.pe=p.p;
  {
    int
      e=p.ps-p.p0,
      d=p.pe-p.p0;
    String
      &text=*p.text,
      buffer;

    buffer=text.mid(e+2,d-e-2);
    cmd->reset();
    cmd->parse(buffer);
    exe->execute(*cmd,buffer);

    text=String()
      .append(text.mid(0,e))
      .append(buffer)
      .append(text.mid(d+1));

    p.p0=text;
    p.p=p.p0+e+buffer.len()-1;
    p.pe=p.ps=0;
  }
}
/*=========================================================

=========================================================*/
WeParse &WeParse::debug(void){
  cmd->debug();
  return *this;
}
/*=========================================================

=========================================================*/
String &WeParse::parse(String &text){
  WE_PARSE
    &p=*(WE_PARSE*)cMemAlloc(sizeof(WE_PARSE));

  p.text=&text;
  p.p0=text;
  p.p=p.p0;

  while(p.p){
    if(!*p.p){
      paramEnd(p);
      break;
    }
    if(*p.p=='\'')
      p.in=!p.in;
    if(!p.in&&!p.ps&&*p.p=='#'&&*(p.p+1)=='{'){
      p.ps=p.p;
      goto NEXT;
    }
    if(!p.in&&p.ps&&*p.p=='}'){
      paramEnd(p);
      goto NEXT;
    }
    NEXT:
      p.p++;
  }

  cMemFree(&p);
  return text;
}
