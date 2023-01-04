
#define MY_HAND_SHAKE_V10             0x0A
#define MY_PACKET_OK                  0x00
#define MY_PACKET_ERR                 0xFF
#define MY_PACKET_EOF                 0xFE
#define MY_CLIENT_PLUGIN_AUTH         0x00080000
#define MY_SERVER_MORE_RESULTS_EXISTS 0x0008
#define MYSQL_TYPE_DECIMAL      0
#define MYSQL_TYPE_TINY         1
#define MYSQL_TYPE_SHORT        2
#define MYSQL_TYPE_LONG         3
#define MYSQL_TYPE_FLOAT        4
#define MYSQL_TYPE_DOUBLE       5
#define MYSQL_TYPE_NULL         6
#define MYSQL_TYPE_TIMESTAMP    7
#define MYSQL_TYPE_LONGLONG     8
#define MYSQL_TYPE_INT24        9
#define MYSQL_TYPE_DATE        10
#define MYSQL_TYPE_TIME        11
#define MYSQL_TYPE_DATETIME    12
#define MYSQL_TYPE_YEAR        13
#define MYSQL_TYPE_NEWDATE     14
#define MYSQL_TYPE_VARCHAR     15
#define MYSQL_TYPE_BIT         16
#define MYSQL_TYPE_TIMESTAMP2  17
#define MYSQL_TYPE_DATETIME2   18
#define MYSQL_TYPE_TIME2       19
#define MYSQL_TYPE_LONG_BLOB  251
#define MYSQL_TYPE_NEWDECIMAL 246
#define MYSQL_TYPE_BLOB       252
#define MYSQL_TYPE_VAR_STRING 253 //varchar
#define MYSQL_TYPE_STRING     254 //char[]


#include "MyByteArray.h"
#include "MySend.h"
#include "MyAction.h"
#include "MyPacketExecute.h"
#include "MyHandshakeV10.h"
#include "MyLoginPacket.h"
#include "MyPacket.h"
#include "MyPacketOk.h"
#include "MyPacketEof.h"
#include "MyPacketError.h"
#include "MyPacketQuery.h"
#include "MyPacketCols.h"
#include "MySql.h"

#include "MySend.cpp"
#include "MyHandshakeV10.cpp"
#include "MyPacketExecute.cpp"
#include "MyLoginPacket.cpp"
#include "MyPacket.cpp"
#include "MySql.cpp"
#include "MyPacketCols.cpp"
#include "MyByteArray.cpp"
#include "MyPacketOk.cpp"
#include "MyPacketEof.cpp"
#include "MyPacketError.cpp"
#include "MyPacketQuery.cpp"
