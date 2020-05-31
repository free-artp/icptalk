
#ifndef __PROTO_H_
#define __PROTO_H_


typedef enum _icp_commands_type_ {
    ICP_ReadModuleName = 0,
    ICP_ReadFirmwareVersion,
    ICP_SetModuleConf,
    ICP_ReadModuleConf,
    ICP_ReadAnalogInput,
    ICP_ReadEventCounter,
    ICP_ClearEventCounter
} ICP_COMMANDS_ENUM;

int icp_talk(int addr, ICP_COMMANDS_ENUM icp_cmd, char* data);

#endif
