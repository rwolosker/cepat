
//http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat.htm
//http://www-inf.int-evry.fr/~hennequi/CoursDNS/NOTES-COURS_eng/msg.html
typedef struct PACK VB_DNS_HEADER{
  ushort size;//be
  ushort id;
  ushort flag;
  ushort QDCount;//be
  ushort ANCount;//be
  ushort NSCount;//be
  ushort ARCount;//be
  char *dnsName;
  ushort type;
  ushort clazz;
}VB_DNS_HEADER;
#define VB_DNS_REVERSE 0x2
#define VB_DNS_REQUEST 0x3

extern int
  dnsDebug,
  dnsTimeout;

long   dnsHost(char *ip);
char  *dnsHostTxt(ulong host,cchar *buffer);
char   dnsIpIs(char *value);
void  *dnsMessageCreate(void *query,int type);
ulong  dnsResolve(char *query,char *nameserverip,int port);
ulong  dnsResolveTry(char *query,void *nameserverip,int port);
void  *dnsReverse(void *query,void *nameserverip,int port,char *out);

#include "dns.cpp"
