#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

#include "pinc.h"

extern "C" {
typedef void* unzFile;

typedef struct fileInPack_s {
    unsigned long pos;         // file info position in zip
    char* name;                // name of the file
    struct fileInPack_s* next; // next file in the hash
} fileInPack_t;

typedef struct {                  // Verified
    char pakFilename[MAX_OSPATH]; // c:\quake3\baseq3\pak0.pk3
    char pakBasename[MAX_OSPATH]; // pak0
    char pakGamename[MAX_OSPATH]; // baseq3
    unzFile handle;               // handle to zip file +0x300
    int checksum;                 // regular checksum
    int pure_checksum;            // checksum for pure
    int hasOpenFile;
    int numfiles;              // number of files in pk3
    int referenced;            // referenced file flags
    int hashSize;              // hash table size (power of 2)		+0x318
    fileInPack_t** hashTable;  // hash table	+0x31c
    fileInPack_t* buildBuffer; // buffer with the filenames etc. +0x320
} pack_t;

typedef struct {              // Verified
    char path[MAX_OSPATH];    // c:\quake3
    char gamedir[MAX_OSPATH]; // baseq3
} directory_t;

typedef struct searchpath_s { // Verified
    struct searchpath_s* next;
    pack_t* pack; // only one of pack / dir will be non NULL
    directory_t* dir;
    qboolean localized;
    int val_2;
    int val_3;
    int langIndex;
} searchpath_t;

extern int fs_checksumFeed;

enum svscmd_type {
    SV_CMD_CAN_IGNORE = 0x0,
    SV_CMD_RELIABLE = 0x1,
};

typedef enum {
    // bk001129 - make sure SE_NONE is zero
    SE_NONE = 0, // evTime is still valid
    SE_CONSOLE,  // evPtr is a char*
    SE_PACKET    // evPtr is a netadr_t followed by data bytes to evPtrLength
} sysEventType_t;

enum SCR_TYPES {
    VAR_UNDEFINED = 0x0,
    VAR_BEGIN_REF = 0x1,
    VAR_POINTER = 0x1,
    VAR_STRING = 0x2,
    VAR_ISTRING = 0x3,
    VAR_VECTOR = 0x4,
    VAR_END_REF = 0x5,
    VAR_FLOAT = 0x5,
    VAR_INTEGER = 0x6,
    VAR_CODEPOS = 0x7,
    VAR_PRECODEPOS = 0x8,
    VAR_FUNCTION = 0x9,
    VAR_STACK = 0xA,
    VAR_ANIMATION = 0xB,
    VAR_DEVELOPER_CODEPOS = 0xC,
    VAR_INCLUDE_CODEPOS = 0xD,
    VAR_THREAD = 0xE,
    VAR_NOTIFY_THREAD = 0xF,
    VAR_TIME_THREAD = 0x10,
    VAR_CHILD_THREAD = 0x11,
    VAR_OBJECT = 0x12,
    VAR_DEAD_ENTITY = 0x13,
    VAR_ENTITY = 0x14,
    VAR_ARRAY = 0x15,
    VAR_DEAD_THREAD = 0x16,
    VAR_COUNT = 0x17,
    VAR_THREAD_LIST = 0x18,
    VAR_ENDON_LIST = 0x19
};

void MSG_Init(msg_t* buf, byte* data, int length);
void MSG_InitReadOnly(msg_t* buf, byte* data, int length);
void MSG_InitReadOnlySplit(msg_t* buf, byte* data, int length, byte*, int);
void MSG_Clear(msg_t* buf);
void MSG_BeginReading(msg_t* msg);
void MSG_Copy(msg_t* buf, byte* data, int length, msg_t* src);
void MSG_WriteByte(msg_t* msg, int c);
void MSG_WriteShort(msg_t* msg, int c);
void MSG_WriteLong(msg_t* msg, int c);
void MSG_WriteFloat(msg_t* msg, float c);
void MSG_WriteData(msg_t* buf, const void* data, int length);
void MSG_WriteString(msg_t* sb, const char* s);
void MSG_WriteBigString(msg_t* sb, const char* s);
int MSG_ReadByte(msg_t* msg);
int MSG_ReadShort(msg_t* msg);
int MSG_ReadLong(msg_t* msg);
char* MSG_ReadString(msg_t* msg, char* bigstring, int len);
char* MSG_ReadStringLine(msg_t* msg, char* bigstring, int len);
void MSG_ReadData(msg_t* msg, void* data, int len);
float MSG_ReadFloat(msg_t* msg);
void MSG_ClearLastReferencedEntity(msg_t* msg);
void MSG_WriteBit0(msg_t* msg);
int MSG_WriteBitsNoCompress(int d, byte* src, byte* dst, int size);
void MSG_WriteVector(msg_t* msg, vec3_t c);
void MSG_WriteInt64(msg_t* msg, int64_t c);
int64_t MSG_ReadInt64(msg_t* msg);

void Info_SetValueForKey(char* s, const char* key, const char* value);
char* Info_ValueForKey(const char* s, const char* key);
void Q_strncpyz(char* dest, const char* src, int destsize);
int Q_stricmpn(const char* s1, const char* s2, int n);
int Q_strncmp(const char* s1, const char* s2, int n);
int Q_stricmp(const char* s1, const char* s2);
void SV_UpdateClientConfigInfo(client_t* cl);
void SV_DropClientNoNotify(client_t* drop, const char* reason);
void SV_GameSendServerCommand(int clientNum, svscmd_type type, const char* text);
void SV_SendServerCommand(client_t* cl, const char* fmt, ...);
void SV_SendServerCommandString(client_t* cl, int type, char* message);
void SV_SendReliableServerCommand(client_t* cl, msg_t* msg);
void* Z_TagMalloc(int size, int tag); // NOT 0 filled memory
void* Z_Malloc(int size);             // returns 0 filled memory
void* S_Malloc(int size);             // NOT 0 filled memory only for small allocations
void Com_QueueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void* ptr);
qboolean SV_Map(const char* levelname);
void SV_MapRestart(qboolean fastrestart);
void SV_ScreenshotClient(client_t* cl, const char* name);
}

const char* BG_WeaponName(int weapon);

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_
