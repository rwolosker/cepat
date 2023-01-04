/**********************************************************
Received 212 bytes from the socket
00000000  10 01 00 D4  00 00 00 00  CC 00 00 00  01 00 00 71  ...............q
00000010  00 10 00 00  06 83 F2 F8  22 6A 00 00  00 00 00 00  ........"j......
00000020  E0 03 00 00  88 FF FF FF  36 04 00 00  56 00 02 00  ........6...V...
00000030  5A 00 07 00  68 00 06 00  74 00 07 00  82 00 0B 00  Z...h...t.......
00000040  00 00 00 00  98 00 0B 00  AE 00 0A 00  C2 00 05 00  ................
00000050  00 E0 29 2D  40 B2 CC 00  00 00 CC 00  00 00 69 00  ..)-@.........i.
00000060  37 00 72 00  69 00 63 00  61 00 72 00  64 00 6F 00  7.r.i.c.a.r.d.o.
00000070  B6 A5 86 A5  96 A5 E6 A5  F6 A5 C6 A5  63 00 6F 00  ............c.o.
00000080  6D 00 61 00  6E 00 64 00  6F 00 31 00  37 00 32 00  m.a.n.d.o.1.7.2.
00000090  2E 00 31 00  36 00 2E 00  35 00 30 00  2E 00 31 00  ..1.6...5.0...1.
000000A0  54 00 44 00  53 00 2D 00  4C 00 69 00  62 00 72 00  T.D.S.-.L.i.b.r.
000000B0  61 00 72 00  79 00 75 00  73 00 5F 00  65 00 6E 00  a.r.y.u.s._.e.n.
000000C0  67 00 6C 00  69 00 73 00  68 00 69 00  70 00 6C 00  g.l.i.s.h.i.p.l.
000000D0  61 00 6E 00                                         a.n.

./configure CFLAGS=-m32 --build=i386-linux --without-openssl --disable-libiconv


src/apps/tsql -H 172.16.50.1 -p 8850 -U ricardo -P 123456 -D iplan -a comando
 TDSVER="7.1"   src/apps/tsql -H 172.16.50.1 -p 8850 -U ricardo -P 123456 -D iplan -a comando
socat    -v -x   -4 tcp-listen:8850,fork tcp:193.0.0.18:1447
src/netcat -L 193.0.0.18:1447 -p 8850 -x


http://www.freetds.org/tds.html#login7
https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-tds/773a62b6-ee89-4c02-9e5e-344882630aac

msgSize             : int32 [cc]
tdsVersion          : int32 0x01000071
packetSize          : int32 4096
clientProgramVersion: int32 0
pid                 : int32 0
connectionId        : int32 0
flag1               : int8  0xe0
        fSetLang  : 1
        fDatabase : 1
        fUseDB    : 1
        fDumpLoad : 0
        fFloat    : 0
        fChar     : 0
        fByteOrder: 0

flag2               : int8  0x03
        fIntSecurity : 0
        fUserType    : 0
        fCacheConnect: 0
        fTranBoundary: 0
        fODBC        : 1
        fLanguage    : 1

flag3               : int8  0x0
sqlType             : int8  0x0
        fSQLType: 0 (4bits)
        fOLEDB:   1
timeZone            : int32 0
collation           : int32 0x
hostPos             : int16
hostLen             : int16
userPos             : int16
userLen             : int16
passPos             : int16
passLen             : int16
appPos              : int16
appLen              : int16
serverPos           : int16
serverLen           : int16
srvPaPos            : int16
srvPaLen            : int16
libPos              : int16
libLen              : int16
langPos             : int16
langLen             : int16
databasePos         : int16
databaseLen         : int16
mac                 : char[6]
ntAuthPos           : int16 == msgSize
ntAuthLen           : int16 == 0
nextPos             : int16 == msgSize
zero2               : int16 == 0

**********************************************************/
typedef struct PACK TDS_LOGIN_7{
  int msgSize;
  int tdsVersion;
  int packetSize;
  int clientProgramVersion;
  int pid;
  int connectionId;
  char flag1;
  char flag2;
  char flag3;
  char sqlType;
  int timeZone;
  int collation;
  short hostPos;
  short hostLen;
  short userPos;
  short userLen;
  short passPos;
  short passLen;
  short appPos;
  short appLen;
  short serverPos;
  short serverLen;
  short srvPaPos;
  short srvPaLen;
  short libPos;
  short libLen;
  short langPos;
  short langLen;
  short databasePos;
  short databaseLen;
  char mac[6];
  short ntAuthPos;
  short ntAuthLen;
  short nextPos;
  short zero2;
}TDS_LOGIN_7;

class TdsLoginPacket:public Object{
  protected:
    String
      user,
      pass,
      server,
      database;

    char bitReverse(char c);
    void password(ByteArray &pass);
  public:
    TdsLoginPacket();
    ~TdsLoginPacket();
    ByteArray       create(void);
    String          getDatabase(void);
    String          getPass(void);
    String          getServer(void);
    String          getUser(void);
    TdsLoginPacket &setDatabase(const String &database);
    TdsLoginPacket &setPass(const String &pass);
    TdsLoginPacket &setServer(const String &server);
    TdsLoginPacket &setUser(const String &user);
};


