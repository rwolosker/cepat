/*=========================================================

=========================================================*/
Ldap::Ldap(){
  initLdap();
}
Ldap::~Ldap(){
}
/*=========================================================

=========================================================*/
void Ldap::initLdap(void){
  port=0;
}
/*=========================================================

=========================================================*/
String Ldap::getError(void){
  return erro;
}
/*=========================================================

=========================================================*/
Ldap &Ldap::clear(void){
  ip=dnUser=dnPass.reset();
  return *this;
}
/*=========================================================

=========================================================*/
Ldap &Ldap::configureImpl(const String &host,const String &baseDn,const String &dns,const String &lib,const String &path,const String &queryFormat,int port){
  this->baseDn=baseDn;
  this->dns=dns;
  this->host=host;
  this->lib=lib;
  this->path=path;
  this->queryFormat=queryFormat;
  this->port=port;
  return *this;
}
/*=========================================================

=========================================================*/
char Ldap::search(const String &query,Map &out){
  String
    cmd=cmdCreate(),
    queryTmp=queryFormat.copy().replace(".user.",query);
  File
    f;
  ByteArray
    buffer;
  Shell
    exe;

  out.reset();
  if(!cmd.len())
    return 0;
  cmd.appendf(" \"%s\"",queryTmp.ptr());

  f.temp();
  exe.exec(cmd,f);
  f.readAll(buffer);

  if(!result(buffer,out))
    return 0;
  return 1;
}
/*=========================================================

=========================================================*/
char Ldap::cmdSearch(const String &query,Map &out){
  String
    cmd=cmdCreate(),
    queryTmp=query;
  File
    f;
  ByteArray
    buffer;
  Shell
    exe;

  out.reset();
  if(!cmd.len())
    return 0;
  cmd.appendf(" \"%s\"",queryTmp.ptr());

  f.temp();
  exe.exec(cmd,f);
  f.readAll(buffer);

  if(!result(buffer,out))
    return 0;
  return 1;
}
/*=========================================================

=========================================================*/
char Ldap::result(ByteArray &buffer,Map &out){
  String
    linha;
  char
    dentro=0;
  Map
    *item=0;
  int
    count=0;

  out.reset();
  buffer.seek(0);
  while(buffer.readLine(linha)){
    linha=linha.trim();
    if(!dentro&&!linha.inStr("dn: ")){
      item=Map().instance();
      out.setValue(linha,item,1);
      dentro=1;
      count+=1;
      continue;
    }
    if(dentro&&!linha.len()){
      dentro=0;
      continue;
    }
    if(dentro){
      int
        x=linha.inStr(':');

      if(FOUND(x))
        item->setValue(
          linha.mid(0,x),
          linha.mid(x+1)
        );
    }
  }
  if(!count){
    buffer.seek(0).readLine(linha);
    erro=String().sprintf("ERRO: %s",linha.ptr());
  }
  return count?1:0;
}
/*=========================================================

=========================================================*/
String Ldap::cmdCreate(void){
  String
    ldLinuxSo,
    ldapSearch,
    cmd;

  if(dnUser.len()&&!dnPass){
    erro="ERRO: pass.";
    return String();
  }

  if(!path.len())
    path=lib;
  if(!port)
    port=389;

  ldLinuxSo=String().sprintf("%s/ld-linux.so.2",lib.ptr());
  ldapSearch=String().sprintf("%s/ldapsearch",path.ptr());

  cmd.appendf(""
      "%s --library-path %s:%s %s -x -LL -s sub",
      ldLinuxSo.ptr(),
      lib.ptr(),
      path.ptr(),
      ldapSearch.ptr()
  );
  if(host.len()){
    String
      tmp;

    if(!dnsResolve(tmp)){
      erro="ERRO: host dns.";
      return String();
    }
    cmd.appendf(" -h %s",tmp.ptr());
  }
  if(port)
    cmd.appendf(" -p %d",port);
  if(baseDn.len())
    cmd.appendf(" -b %s",baseDn.ptr());
  if(dnUser.len())
    cmd.appendf(" -D %s",dnUser.ptr());
  if(dnPass.len())
    cmd.appendf(" -w %s",dnPass.ptr());

  return cmd;
}
/*=========================================================

=========================================================*/
char Ldap::dnsResolve(String &out){
  out.reset();
  if(ip.len()){
    out=ip;
    return 1;
  }
  if(!host.len())
    return 0;
  {
    String
      query=host;
    ulong
      value;

    query.resize(1024);
    if(!dns.len())
      dns="127.0.0.1";
    if(!(value=::dnsResolveTry(query.ptr(),dns.ptr(),53)))
      return 0;
    dnsHostTxt(value,ip.reset().resize(1024));

    out=ip.reLen().squeeze();
    return 1;
  }
}
/*=========================================================
ib/ld-2.5.so --library-path lib:lib/ldap lib/ldap/ldapsearch -h 193.0.0.11 -p 30011 -x -LL  -b "dc=rio,dc=rj,dc=gov,dc=br"  -D "uid=ricardo.wolosker,ou=pessoas,ou=iplanrio,dc=rio,dc=rj,dc=gov,dc=br" -w 22635544  "employeeNumber=26225846" --help
(|(employeeNumber=#user#)(uid=#user#)(cn=#user#)(mail=#user#))
=========================================================*/
char Ldap::login(cchar *user,cchar *pass){
  Map
    out;
  Split
    keys;
  MapItem
    *item=0;
  String
    employeeNumber,
    accountStatus;

  clear();
  if(!search(user,out)){
    erro=String().sprintf("ERRO: não te achei. %s.",erro.ptr());
    return 0;
  }
  keys=out.keys();
  if(keys.len()!=1){
    erro="ERRO: nenhum ou mais de um.";
    return 0;
  }
  item=&out.get(keys[0]);
  employeeNumber=((Map*)item->asObj())->get("employeeNumber")
    .asStr()
    .trim();
  accountStatus=((Map*)item->asObj())->get("accountStatus")
    .asStr()
    .trim();

  if(!accountStatus.equals("Active",1)){
    erro="ERRO: inativo.";
    return 0;
  }
  if(!employeeNumber.len()){
    erro="ERRO: não consegui resolver [].";
    return 0;
  }

  dnUser=Split(keys[0],':')[1].trim();
  dnPass=pass;
  out.reset();
  if(!search(employeeNumber,out))
    return 0;
  if(out.keys().len()!=1){
    erro=String().sprintf("ERRO: não sei o que aconteceu. %s.",erro.ptr());
    return 0;
  }
  return 1;
}
/*=========================================================

=========================================================*/
void Ldap::configure(Map &map){
  host=map.get("ldap.host").asStr();
  port=map.get("ldap.port").asInt();
  baseDn=map.get("ldap.baseDn").asStr();
  dns=map.get("ldap.dns").asStr();
  lib=map.get("ldap.lib").asStr();
  path=map.get("ldap.path").asStr();
  queryFormat=map.get("ldap.query").asStr();

  if(!dns.len())
    dns=map.get("php.dns").asStr();
  if(!dns.len())
    dns="127.0.0.1";
}
/*=========================================================

=========================================================*/
Ldap &Ldap::setQueryFormat(const String  &query){
  queryFormat=query;
  return *this;
}
