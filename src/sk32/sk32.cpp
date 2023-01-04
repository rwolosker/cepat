/*=========================================================

=========================================================*/
void sk32setdebug(SK32 *s,char value){
  if(!s)
    return;
  s->debug=value;
}
/*=========================================================

=========================================================*/
void sk32setdns(SK32 *s,char *value){
  if(!s)
    return;
  cMemFree(s->dnstxt);
  s->dnstxt=cStrCopy(value,0);
}
/*=========================================================

=========================================================*/
SK32 *sk32free(SK32 *s){
  if(s){
    if(s->methodtxt)
      s->methodtxt=cMemFree(s->methodtxt);
    if(s->dnstxt)
      s->dnstxt=cMemFree(s->dnstxt);
    if(s->host)
      s->host=cMemFree(s->host);
    if(s->ip)
      s->ip=cMemFree(s->ip);

    if(s->isssl){
      if(s->ssl){
        s->SSL_shutdown(s->ssl);
        s->SSL_free(s->ssl);
      }
      if(s->ctx)
        s->SSL_CTX_free(s->ctx);
    }
    s->ssl=s->ctx=0;

    if(s->close)
      s->close(s);
    if(s->islib){
      if(s->lib)cLibraryFree(s->lib);
      if(s->libcry)cLibraryFree(s->libcry);
    }
    cMemFree(s);
  }
  return 0;
}
/*=========================================================

=========================================================*/
SK32 *sk32create(void){
  return(SK32*)cMemAlloc(sizeof(SK32));
}
/*=========================================================

=========================================================*/
char sk32readdump(SK32 *s,void *out){
  void
    **pout=(void**)out;

  s->error=1;
  *pout=cMemFree(*pout);
  if(!s->contentLen)
    return 0;
  return s->read(s,PCHAR(*pout=cMemAlloc(s->contentLen)),s->contentLen);
}

/*=========================================================

=========================================================*/
char sk32readchunked(SK32 *s,void *out){
  int
    lenbuf,len,lidos,lenres=0;
  char
    *buffer=cMemAllocChar(lenbuf=4096),
    *res=0;
  void
    **pout=(void**)out;

  s->error=0;
  while(1){
    sk32readline(s,buffer);
    if(s->error)
      goto FIM;
    len=Number().fromHex(buffer).asInt();
    cDebug(s->debug,"sk32readchunked: line=[%s] len=[%d]",buffer,len);
    if(!len){
      sk32readline(s,buffer);
      goto FIM;
    }
    if(len>lenbuf)
      buffer=PCHAR(cMemRealloc(buffer,lenbuf=len));
    while(len){
      lidos=s->read(s,buffer,len);
      if(lidos<=0){
        s->error=1;
        goto FIM;
      }
      res=PCHAR(cMemRealloc(res,lenres+lidos+1-1));
      cMemCpy(VOID(buffer),lidos,VOID(res+lenres));
      lenres+=lidos;
      len-=lidos;
    }
    sk32readline(s,buffer);
  }
  FIM:
  *pout=cMemFree(*pout);
  if(s->error)
    cMemFree(res);
  else
    *pout=res;
  cMemFree(buffer);
  return s->error?0:1;
}
/*=========================================================

=========================================================*/
int sk32contentlength(Split &header,SK32 *s){
  s->contentLen=s->ischuncked=0;
  for(int i=0,x;i<header.len();i++){
    if(!s->ischuncked)
      s->ischuncked=1
        &&FOUND(header[i].inStr("transfer-encoding",1))
        &&FOUND(header[i].inStr("chunked",1));
    if(FOUND(x=header[i].inStr("content-length",1)))
      s->contentLen=Number().parse(
        header[i].mid(x+cStrLen("content-length")));
  }
  cDebug(s->debug,"sk32contentlength: ischuncked=[%d] contentLen=[%d]",s->ischuncked,s->contentLen);
  return s->contentLen;
}
/*=========================================================

=========================================================*/
Split sk32headers(SK32 *s){
  char
    *buffer=cMemAllocChar(4096);
  Split
    list;
  s->contentLen=s->ischuncked=0;
  while(sk32readline(s,buffer)&&!s->error&&*buffer)
    list.add(buffer);
  if(s->debug)
    sk32headersdebug(list);
  cMemFree(buffer);
  return list;
}
/*=========================================================

=========================================================*/
void sk32headersdebug(Split &list){
  for(int i=0;i<list.len();i++)
    cDebug(1,"sk32headers: %s",list[i].ptr());
}
/*=========================================================

=========================================================*/
int sk32readline(SK32 *s,char *buffer){
  char
    c,
    *p=buffer;
  int
    lidos,old,total=0;

  while(1){
    old=s->debug;
    s->debug=0;
    lidos=s->read(s,&c,1);
    s->debug=old;
    cDebug(s->debug,
      "sk32readline: lidos=[%d] c=[%02X]['%c'][%*.*s]",
      lidos,c,c!='\r'&&c!='\n'?c:'.',p-(char*)buffer,p-(char*)buffer,buffer
    );
    if(lidos!=1||s->error)
      return 0;
    else{
      if(c=='\r')
        continue;
      if(c=='\n')
        break;
      *p++=c;
      total+=1;
    }
  }
  *p=0;
  return total;
}
/*=========================================================
o len nao pode ser um valor qualquer. precisa ser exato.
senao vai sempre sair por timeout
read ZERO eh tratato como nao lido e sai por timeout
=========================================================*/
int sk32readnonblock(SK32 *s,char *buffer,int len){
  int
    timeout=sk32gettimeout(s),
    start=time(0),
    espera=1,
    lidos,total=len,count=0;
  char
    *p=buffer;

  s->error=0;
  while(len){
    if(!s->nosleep)
      usleep(espera);
    if(s->isssl)
      lidos=s->SSL_read(s->ssl,p,len);
    else
      lidos=recv(s->s,VOID(p),len,0);
    if(s->debug){
      int
        tempo=time(0);

      cDebug(
        s->debug,
        "sk32readnonblock: total=[%d] lidos=[%d] wait=[%d] count=[%d] timeout=[%d] tempo=[%d]",
        total,lidos,espera,count+lidos,timeout,tempo-start
      );
    }
    if(lidos<=0){
      if(time(0)-start>timeout){
        s->error=1;
        return count;
      }
      espera+=1;
      continue;
    }
    count+=lidos;
    p+=lidos;
    len-=lidos;
    start=time(0);
  }
  s->error=count!=total;
  return count;
}
/*=========================================================

=========================================================*/
int sk32read(SK32 *s,char *buffer,int len){
  if(!s->nosleep)
    usleep(1);
  if(sk32getnonblock(s,1))
    return sk32readnonblock(s,buffer,len);
  else{
    int
      lidos,total=len,parte=sk32readpart,feito=0;
    char
      *p=PCHAR(buffer);

    if(total)
      while(feito<total){
        if(!s->nosleep)
          usleep(1);
        if(total-feito<parte)
          parte=total-feito;
        if(s->isssl)
          lidos=s->SSL_read(s->ssl,p,parte);
        else
          lidos=recv(s->s,p,parte,0);
        cDebug(s->debug,"sk32read: total=[%d] parte=[%d] lidos=[%d] count=[%d]",total,parte,lidos,feito+lidos);
        if(lidos<=0){
          feito=0;
          break;
        }
        p+=lidos;
        feito+=lidos;
      }
    s->error=feito!=total;
    return feito;
  }
}
/*=========================================================

=========================================================*/
int sk32write(SK32 *s,void *buffer,int len){
  int
    lidos,total=len,parte=sk32readpart,feito=0;
  char
    *p=PCHAR(buffer);

  while(feito<total){
    usleep(1);
    if(total-feito<parte)
      parte=total-feito;
    if(s->isssl)
      lidos=s->SSL_write(s->ssl,p,parte);
    else
      lidos=send(s->s,p,parte,0);
    cDebug(s->debug,"sk32write: len=[%d] write=[%d] error=[%d]",len,lidos,s->error);
    if(lidos<=0){
      s->error=1;
      break;
    }
    p+=lidos;
    feito+=lidos;
  }
  s->error=feito!=total;
  //lidos=send(s->s,buffer,len,0);
  //s->error=lidos!=len;
  //cDebug(s->debug,"sk32write: len=[%d] write=[%d] error=[%d]",len,lidos,s->error);
  return feito;
}
/*=========================================================

=========================================================*/
char sk32connect(SK32 *s,char *host,int port,int nodelay,int timeout){
  char
    *buffer=0;
  int
    c=0;
  ulong
    ip;

  if(!s)
    return 0;
  sk32close(s);
  s->error=1;
  buffer=cMemAllocChar(1024);
  cDebug(s->debug,"sk32connect: host=[%s] port=[%d] nodelay=[%d] timeout=[%d]",host,port,nodelay,timeout);
  cMemFree(s->host);
  cMemFree(s->ip);

  s->host=cStrCopy(host,0);
  if(dnsIpIs(host))
    s->ip=cStrCopy(host,0);
  if(!dnsIpIs(host)&&s->dnstxt){
    cDebug(s->debug,"sk32connect: dns=[%s]",s->dnstxt);
    if(!(ip=dnsResolveTry(host,s->dnstxt,53)))
      goto ERRO;
    s->ip=cStrCopy(
     dnsHostTxt(ip,buffer),
     0
    );
  }
  cDebug(s->debug,"sk32connect: ip=[%s]",s->ip);
  c=socket(PF_INET,SOCK_STREAM,0);
  if(c<=0)
    goto FIM;
  s->s=c;
  if(!sk32setnodelay(s,nodelay))
    goto ERRO;
  if(!sk32settimeout(s,timeout))
    goto ERRO;
  {
    uchar
      *addr=PUCHAR(cMemAlloc(16));
    int
      ok;

    *(unsigned short*)&addr[0]=AF_INET;
    *(unsigned short*)&addr[2]=Number(port).endian(2).asInt();
    *(unsigned long* )&addr[4]=sk32iptoaddr(PCHAR(s->ip));

    sk32setnonblock(s,1);
    connect(s->s,addr,16);
    cMemFree(addr);
    ok=sk32isready(s,timeout,0,"-w-");
    cDebug(s->debug,"sk32connect: isready(-w-)=[%d]",ok);
    sk32setnonblock(s,0);
    if(!ok)
      goto ERRO;
  }
  s->timeout=timeout;
  s->port=port;
  s->error=0;
  s->nodelay=nodelay;
  s->read=sk32read;
  s->write=sk32write;
  s->close=sk32close;
  s->error=0;
  goto FIM;
  ERRO:
  close(c);
  s->s=0;
  s->error=1;
  FIM:
  cMemFree(buffer);
  cDebug(s->debug,"sk32connect: ok=[%d]",s->error==0);
  return s->error==0;
}
/*=========================================================
bits rwe  read write error
=========================================================*/
char sk32isready(SK32 *s,int sec,int msec,char *bits){
  int
    *sread=(int*)cMemAlloc(128),
    *swrite=(int*)cMemAlloc(128),
    *serror=(int*)cMemAlloc(128),
    iread,iwrite,ierror,
    *timeout=(int*)cMemAlloc(2*sizeof(long)),
    ok;

  sread[s->s/32]=1<<s->s%32;
  swrite[s->s/32]=1<<s->s%32;
  serror[s->s/32]=1<<s->s%32;
  timeout[0]=sec;
  timeout[1]=msec;
  select(
    1024,
    sread,
    swrite,
    serror,
    timeout
  );
  iread=sread[s->s/32];
  iwrite=swrite[s->s/32];
  ierror=serror[s->s/32];

  ok=1
    &&((bits[0]=='-')||(iread ))
    &&((bits[1]=='-')||(iwrite))
    &&((bits[2]=='-')||(ierror));

  cMemFree(sread);
  cMemFree(swrite);
  cMemFree(serror);
  cMemFree(timeout);

  return ok;
}
/*=========================================================

=========================================================*/
int sk32error(SK32 *s){
  int
    len=sizeof(int),
    erro,ok;

  ok=getsockopt(s->s,SOL_SOCKET,SO_ERROR,&erro,&len);
  return ok?1:erro;
}
/*=========================================================

=========================================================*/
char sk32getnonblock(SK32 *s,int value){
  int
    flags;

  flags=fcntl(s->s,F_GETFL,0);
  if(flags==-1)
    return 0;

  return 0
    ||( value&& (flags&O_NONBLOCK))
    ||(!value&&!(flags&O_NONBLOCK));
}
/*=========================================================

=========================================================*/
char sk32setnonblock(SK32 *s,int value){
  int
    flags,ok;

  flags=fcntl(s->s,F_GETFL,0);
  if(flags==-1)
    return 0;
  else{
    flags=value
      ?flags|O_NONBLOCK
      :flags&~O_NONBLOCK;
    ok=fcntl(s->s,F_SETFL,flags);

    return !ok;
  }
}
/*=========================================================

=========================================================*/
int sk32close(SK32 *s){
  if(s&&s->isssl){
    if(s->ssl){
      s->SSL_shutdown(s->ssl);
      s->SSL_free(s->ssl);
    }
    if(s->ctx)
      s->SSL_CTX_free(s->ctx);

    s->ssl=s->ctx=0;
    s->isssl=0;
  }
  if(s&&s->s>0)
    close(s->s);
  return s->s=0;
}
/*=========================================================

=========================================================*/
int sk32iptoaddr(char *ip){
  int
    res=0;
  char
    *p=PCHAR(&res);
  Split
    split;

  split.split(ip,'.');
  for(int i=0;i<4;i++)
    p[i]=Number().parse(split[i]).asInt();
  return res;
}
/*=========================================================

=========================================================*/
char sk32setnodelay(SK32 *s,int value){
  int
    len=1*sizeof(int),
    ok=setsockopt(s->s,SOL_TCP,TCP_NODELAY,&value,len);

  return !ok;
}
/*=========================================================

=========================================================*/
char sk32settimeout(SK32 *s,int sec){
  int
    len=2*sizeof(int),
    *timeout=(int*)cMemAlloc(len),
    ok1,
    ok2;

  timeout[0]=sec;
  timeout[1]=0;
  ok1=setsockopt(s->s,SOL_SOCKET,SO_RCVTIMEO,timeout,len);
  ok2=setsockopt(s->s,SOL_SOCKET,SO_SNDTIMEO,timeout,len);
  cMemFree(timeout);
  return !ok1&&!ok2;
}
/*=========================================================

=========================================================*/
int sk32gettimeout(SK32 *s){
  int
    len=2*sizeof(int),
    *timeout=(int*)cMemAlloc(len);

  getsockopt(s->s,SOL_SOCKET,SO_RCVTIMEO,timeout,&len);
  len=timeout[0]+(timeout[1]?1:0);
  cMemFree(timeout);
  return len;
}
/*=========================================================

=========================================================*/
SK32 *sk32server(int port){
  SK32
    *s=sk32create();
  int
    enable=1,
    addrLen=48;
  char
    *addr=cMemAllocChar(addrLen);

  s->s=socket(AF_INET,SOCK_STREAM,0);
  if(!FOUND(s->s))
    goto ERRO;
  *(unsigned short*)&addr[0]=AF_INET;
  *(unsigned short*)&addr[2]=Number(port).endian(2).asInt();
  setsockopt(s->s,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
  if(!FOUND(bind(s->s,VOID(addr),addrLen)))
    goto ERRO;
  goto FIM;
  ERRO:
    s=sk32free(s);
  FIM:
    cMemFree(addr);
    return s;
}
/*=========================================================

=========================================================*/
char sk32listen(SK32 *s,int max){
  return s&&s->s&&FOUND(listen(s->s,max))
    ?1
    :0;
}
