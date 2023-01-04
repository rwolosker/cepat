/*=========================================================

=========================================================*/
void *dnsReverse(void *query,void *nameserverip,int port,char *out){
  if(1){
    int
      ok,
      enable=1,
      addrlen=16,
      s;
    uchar
      *addr=PUCHAR(cMemAlloc(addrlen)),
      *buffer=PUCHAR(dnsMessageCreate(query,VB_DNS_REVERSE)),
      *res=PUCHAR(cMemAllocChar(1024));
    int
      bufferlen=cMemSize(buffer);
    double
      start=Date().now();
    VB_DNS_HEADER
      *h;

    *out=0;
    s=socket(AF_INET,SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK,IPPROTO_IP);
    *(unsigned short*)&addr[0]=AF_INET;
    *(unsigned short*)&addr[2]=Number(port).endian(2).asInt();
    *(unsigned long* )&addr[4]=dnsHost(PCHAR(nameserverip));
    setsockopt(s,SOL_IP,IP_RECVERR,&enable,4);
    ok=connect(s,addr,addrlen);
    if(ok){
      close(s);
      goto FIM;
    }

    ok=sendto(s,VOID(buffer),bufferlen,MSG_NOSIGNAL,0,0);
    if(ok<=0){
      close(s);
      goto FIM;
    }
    ok=-1;
    while(ok==-1){
      //NONBLOCK
      ok=recvfrom(s,res,1024,0,addr,&addrlen);
      if(ok>0)
        break;
      usleep(100);
      if(Date().now().get()-start>0.000010)
        break;
    }
    if(ok<=0){
      close(s);
      goto FIM;
    }
    //vbhexdump(res,ok);
    h=(VB_DNS_HEADER*)(res-2/*o size nao vem*/);
    if(Number(h->ANCount).endian(2).asInt()>=1){
      uchar
        *p=res+bufferlen+12,
        *po=PUCHAR(out);

      //vbhexdump(p,100);
      while(*p){
        po+=cStrLen(cStrMid(PCHAR(p),1,*p,PCHAR(po)));
        *po++='.';
        p+=*p+1;
      }
      po[-1]=0;
    }
    close(s);
    FIM:
    cMemFree(buffer);
    cMemFree(res);
    cMemFree(addr);
    return out;
  }
  return out;
}
/*=========================================================

=========================================================*/
char *dnsHostTxt(ulong host,cchar *buffer){
  uchar
    *ph=PUCHAR(&host);
  String
    tmp;

  for(int i=0;i<4;i++)
    tmp
      .append((int)ph[i])
      .append('.');

  tmp[tmp.len()-1]=0;
  return cStrCopy(tmp,PCHAR(buffer));
}
/*=========================================================

=========================================================*/
ulong dnsResolveTry(char *query,void *nameserverip,int port){
  Split
    dns=Split().split(PCHAR(nameserverip),';');
  ulong
    res=0;

  for(int i=0;i<dns.len();i++)
    if((res=dnsResolve(query,dns[i].ptr(),port)))
      break;

  return res;
}
/*=========================================================

=========================================================*/
ulong dnsResolve(char *query,char *nameserverip,int port){
  if(dnsIpIs(query))
    return dnsHost(query);
  else{
    int
      ok,
      total,
      i,
      flags,
      enable=1,
      addrlen=16,
      s=0;
    uchar
      *addr=PUCHAR(cMemAlloc(addrlen)),
      *buffer=PUCHAR(dnsMessageCreate(query,VB_DNS_REQUEST)),
      *res=PUCHAR(cMemAlloc(1024)),
      *p;
    long
      resded=0,
      t0=time(0);
    ushort
      *ps;

    cDebug(
      dnsDebug,
      "START dnsReverse\n"
      "  query=[%s]\n"
      "  nameserverip=[%s]\n"
      "  port=[%d]",
      query,
      nameserverip,
      port
    );
    s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    *(unsigned short*)&addr[0]=AF_INET;
    *(unsigned short*)&addr[2]=Number(port).endian(2).asInt();
    *(unsigned long* )&addr[4]=dnsHost(nameserverip);

    enable=1;
    setsockopt(s,SOL_IP,IP_RECVERR,&enable,4);
    enable=1;
    setsockopt(s,SOL_IP,IP_RECVTOS,&enable,4);
    ok=connect(s,addr,addrlen);
    flags=fcntl(s,F_GETFL,0);
    fcntl(s,F_SETFL,flags|O_NONBLOCK);
    cDebug(dnsDebug,"  CONNECT\n    s=[%d]\n    ok=[%d]",s,ok);
    if(ok)
      goto FIM;

    ok=sendto(s,buffer,cMemSize(buffer),MSG_NOSIGNAL,0,0);
    cDebug(dnsDebug,"  SEND\n    ok=[%d]",ok);
    if(ok<=0)
      goto FIM;
    ok=-1;
    while(ok==-1){
      //NONBLOCK
      ok=recvfrom(s,res,1024,0,addr,&addrlen);
      cDebug(dnsDebug,"  RECV\n    ok=[%d]",ok);
      if(ok>0)
        break;
      usleep(100);
      if(time(0)-t0>dnsTimeout)
        break;
    }
    if(ok<=0)
      goto FIM;

    p=PUCHAR(res);
    ps=(ushort*)res;

    if(dnsDebug)
      cHexdump(res,ok);

    if(Number((int)ps[3]).endian(2).asInt()<=0)
      goto FIM;
    total=0
      +Number((int)ps[3]).endian(2).asInt()
      +Number((int)ps[4]).endian(2).asInt()
      +Number((int)ps[5]).endian(2).asInt();
    p+=12;
    p+=cStrLen(PCHAR(p))+1;
    p+=4;
    for(i=0;i<total;i++){
      ps=(ushort*)p;
      {
        int
          type=Number((int)ps[1]).endian(2).asInt(),
          size=Number((int)ps[5]).endian(2).asInt();

        p+=2*6;
        if(type==1){
          resded=*(long*)p;
          goto FIM;
        }
        p+=size;
      }
    }
    FIM:
    if(s>0)
      close(s);
    cMemFree(buffer);
    cMemFree(res);
    cMemFree(addr);
    return resded;
  }
}
/*=========================================================

=========================================================*/
void *dnsMessageCreate(void *query,int type){
  Split
   v=Split().split(PCHAR(query),'.');
  char
    *dnsName=PCHAR(cMemAlloc(1024)),
    *p=dnsName,
    *buffer=0,
    *ph;
  int
    len,
    bufferLen,
    dnsNameLen=0;
  VB_DNS_HEADER
    *h=(VB_DNS_HEADER*)cMemAlloc(sizeof(VB_DNS_HEADER));

  ph=(char*)(void*)h;
  if(type==VB_DNS_REQUEST){
    for(int i=0;i<v.len();i++){
      len=cStrLen(v[i]);
      cMemCpy(VOID(&len),1,VOID(p));p+=1;
      cStrCopy(PCHAR(v[i]),PCHAR(p));p+=len;
    }
  }
  if(type==VB_DNS_REVERSE){
    for(int i=3;i>=0;i--){
      len=cStrLen(v[i]);
      cMemCpy(&len,1,p);p+=1;
      cStrCopy(PCHAR(v[i]),PCHAR(p));p+=len;
    }
    len=cStrLen("in-addr");
    cMemCpy(&len,1,p);p+=1;
    cStrCopy("in-addr",p);p+=len;
    len=cStrLen("arpa");
    cMemCpy(&len,1,p);p+=1;
    cStrCopy("arpa",p);p+=len;
  }
  p+=1;

  dnsNameLen=p-dnsName;
  p=PCHAR(cMemCpy(dnsName,dnsNameLen,cMemAlloc(dnsNameLen)));
  cMemFree(dnsName);
  dnsName=p;

  h->id=0x451A;
  h->flag=1;
  h->QDCount=Number(1).endian(2).asInt();
  h->ANCount=0;
  h->NSCount=0;
  h->ARCount=0;
  h->dnsName=dnsName;
  h->type=Number(type==VB_DNS_REQUEST?1:0x0C).endian(2).asInt();
  h->clazz=Number(1).endian(2).asInt();

  p=buffer=PCHAR(cMemAlloc(bufferLen=sizeof(VB_DNS_HEADER)-4/*dnsName*/+dnsNameLen));
  h->size=Number(-2+bufferLen).endian(2).asInt();
  cMemCpy(ph,7*2,p);p+=7*2;
  cMemCpy(h->dnsName,dnsNameLen,p);p+=dnsNameLen;
  *(ushort*)p=h->type;p+=2;
  *(ushort*)p=h->clazz;p+=2;

  cMemFree(dnsName);
  cMemFree(h);

  p=PCHAR(cMemCpy(buffer+2,bufferLen-2,cMemAlloc(bufferLen-2)));
  cMemFree(buffer);
  buffer=p;

  return buffer;
}
/*=========================================================

=========================================================*/
char dnsIpIs(char *value){
  char
    ok=0;
  int
    i;
  Split
    v=Split().split(value,'.');

  if(v.len()!=4)
    goto FIM;
  for(i=0;i<4;i++){
    String
      psv=v[i];
    char
      *pv=psv.ptr();

    if(!psv.len()||psv.len()>3)
      goto FIM;
    while(*pv)
      if(*pv<'0'||*pv>'9')
        goto FIM;
      else
        pv++;
  }
  ok=1;
  FIM:
    return ok;
}
/*=========================================================

=========================================================*/
long dnsHost(char *ip){
  long
    i,
    res=0;
  uchar
    *pres=(uchar*)&res;
  Split
   pip=Split().split(ip,'.');

  for(i=0;i<4;i++)
    pres[i]=Number().parse(pip[i]).asInt();
  return res;
}
