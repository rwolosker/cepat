/*=========================================================

=========================================================*/
TableProtocol::TableProtocol(){
  initTableProtocol();
}
TableProtocol::~TableProtocol(){
}
void TableProtocol::initTableProtocol(void){
  port=lasterror=isopen=0;
  affectedInsert=affectedDelete=affectedUpdate=0;
  timeout=0;
}
/*=========================================================

=========================================================*/
String TableProtocol::getDatabase(void){return database;}
String TableProtocol::getDns(void){return dns;}
String TableProtocol::getHost(void){return host;}
String TableProtocol::getPassword(void){return password;}
int TableProtocol::getPort(void){return port;}
String TableProtocol::getUsername(void){return username;}
String TableProtocol::getError(void){return error;}
char TableProtocol::isOpen(void){return isopen;}
int TableProtocol::lastError(void){return lasterror;}
int TableProtocol::getAffectedInsert(void){return affectedInsert;}
int TableProtocol::getAffectedUpdate(void){return affectedUpdate;}
int TableProtocol::getAffectedDelete(void){return affectedDelete;}
Socket32 &TableProtocol::getSocket(void){return socket;}

/*=========================================================

=========================================================*/
TableProtocol &TableProtocol::setDatabase(const String &database){
  this->database=database;return *this;
}
TableProtocol &TableProtocol::setDns(const String &dns){
  this->dns=dns;return *this;
}
TableProtocol &TableProtocol::setHost(const String &host){
  this->host=host;return *this;
}
TableProtocol &TableProtocol::setPassword(const String &password){
  this->password=password;return *this;
}
TableProtocol &TableProtocol::setPort(int port){
  this->port=port;return *this;
}
TableProtocol &TableProtocol::setTimeout(int timeout){
  this->timeout=timeout;
  if(isopen)
    socket.setTimeout(timeout);
  return *this;
}
TableProtocol &TableProtocol::setUsername(const String &username){
  this->username=username;return *this;
}
/*=========================================================

=========================================================*/
char TableProtocol::openSocket(void){
  String
    ip;

  socket.close();
  isopen=lasterror=0;
  error.reset();
  if(!socket.getTimeout())
    socket.setTimeout(timeout);

  if(dnsIpIs(host))
    ip=host;
  else{
    socket.setDns(dns);
    ip=socket.resolve(host);
  }
  if(ip.equals("0.0.0.0")){
    lasterror=1;
    error="cant resolve host.";
    return 0;
  }
  if(!socket.open(ip,port)){
    lasterror=1;
    error="cant connect host.";
    return 0;
  }
  return 1;
}
