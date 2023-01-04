
#define TDS_VERSION_71        0x01000071
#define TDS_QUERY             0x1
#define TDS_PACKET_LOGIN_7    0x10
#define TDS_RESULT            0x81
#define TDS_ERROR_MESSAGE     0xAA
#define TDS_NON_ERROR_MESSAGE 0xAB
#define TDS_LOGIN_ACKNOW      0xAD
#define TDS_ROW_RESULT        0xD1
#define TDS_RESULT_SET_DONE   0xFD
#define TDS_ENV_CHANGE        0xE3
#define TDS_ENV_PACKET_SIZE   0x4
#define TDS_DONE_INSERT       0xC3
#define TDS_DONE_DELETE       0xC4
#define TDS_DONE_UPDATE       0xC5


#include "TdsPacket.h"
#include "TdsLoginPacket.h"
#include "TdsSend.h"
#include "TdsAction.h"
#include "TdsPacketExecute.h"
#include "TdsError.h"
#include "TdsQueryPacket.h"
#include "TdsQueryDef.h"
#include "Tds.h"
#include "TdsLoginAcKnow.h"
#include "TdsEnvChange.h"
#include "TdsResultDone.h"

#include "TdsPacket.cpp"
#include "TdsLoginPacket.cpp"
#include "TdsSend.cpp"
#include "TdsPacketExecute.cpp"
#include "TdsAction.cpp"
#include "TdsError.cpp"
#include "TdsQueryPacket.cpp"
#include "TdsQueryDef.cpp"
#include "Tds.cpp"
#include "TdsLoginAcKnow.cpp"
#include "TdsEnvChange.cpp"
#include "TdsResultDone.cpp"
