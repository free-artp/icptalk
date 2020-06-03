#include <string.h>
#include <syslog.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>

#include "config.h"
#include "serial.h"

#define true (1)
#define false (0)


typedef struct _icp_cmd_type {
    // command
    char cpref;
    int  broadcast;
    char cmd[8];
    // response
    char rpref;
    int anonymous;
} icp_command;


icp_command Commands[] =
{
    {   // ICP_ReadModuleName
        .cpref = '$',
        .cmd = "M",
        .rpref = '!'
    },
    {   // ICP_ReadFirmwareVersion
        .cpref = '$',
        .cmd = "F",
        .rpref = '!'
    },
    {   // ICP_SetModuleConf
        .cpref = '%',
        .rpref = '!'
    },
    {   // ICP_ReadModuleConf
        .cpref = '$',
        .cmd = "2",
        .rpref = '!'
    },
    {   // ICP_ReadAnalogInput
        .cpref = '#',
        .rpref = '>',
        .anonymous = true
    },
    {   // ICP_ReadEventCounter
        .cpref = '@',
        .cmd = "RE",
        .rpref = '!'
    },
    {   // ICP_ClearEventCounter
        .cpref = '@',
        .cmd = "CE",
        .rpref = '!'
    }

};

char cmd_string[20], rsp_string[20], hex_string[40];

int icp_talk(int addr, int icp_cmd, char* odata, char* idata)
{

    char * c;
    const char * hex = "0123456789ABCDEF";
   	struct pollfd  poll_fd;
	int n;

    c = memset(cmd_string, 0, sizeof(cmd_string));

    *c = Commands[ icp_cmd ].cpref;
    if ( ! Commands[ icp_cmd ].broadcast ) {
        *(++c) = hex[(addr>>4) & 0xF];
        *(++c) = hex[ addr     & 0xF];
        c++;
    }
    if ( strlen(Commands[ icp_cmd ].cmd) != 0) {
        memcpy( c, Commands[ icp_cmd ].cmd, strlen(Commands[ icp_cmd ].cmd));
        c += strlen(Commands[ icp_cmd ].cmd);
    }
    if (odata != NULL ) {
        if (*odata != 0 ) {
            memcpy(c, odata, strlen(odata));
            c += strlen(odata);
        }
    }
    *c++ = '\x0D';

    write(fd_port, cmd_string, strlen(cmd_string));
    if (verbose >= 3) {
        tohex(cmd_string, strlen(cmd_string), hex_string, sizeof(hex_string));
        syslog(LOG_INFO, "sent: %s (%s)", hex_string, cmd_string);
    }

    usleep(20000);

    c = memset(rsp_string, 0, sizeof(rsp_string));
    read(fd_port, rsp_string, sizeof(rsp_string));
    if (verbose >= 3) {
        tohex(rsp_string, strlen(rsp_string), hex_string, sizeof(hex_string));
        syslog(LOG_INFO, "read: %s (%s)", hex_string, rsp_string);
    }
    
    if ( *c++ != Commands[ icp_cmd ].rpref) return 3;   // проверка префикса ответа
    if ( !Commands[icp_cmd].anonymous ) {               // проверка адреса ответившего
        if (*c++ != cmd_string[1] || *c++ != cmd_string[2] )
            return 4;
    }
    if (idata != NULL) {
        memset( idata, 0, strlen(c)+1);
        memcpy(idata, c, strlen(c));
    }

    return 0;
}
