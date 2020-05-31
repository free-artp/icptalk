#include <string.h>
#include <syslog.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>

#include "comm.h"

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

char cmd_string[20], hex_string[40];

int icp_talk(int addr, int icp_cmd, char* odata)
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
    tohex(cmd_string, strlen(cmd_string), hex_string, sizeof(hex_string));
    syslog(LOG_INFO, "sent: %s", hex_string);

    // // ---

    // poll_fd.fd = fd_port;
    // poll_fd.events = POLLIN;
    // poll_fd.revents = 0;
    // n = poll(&poll_fd, 5, 500); // wait here
    
    // if (n < 0) {
    //     syslog(LOG_ERR, "reader error %d %s", errno, strerror (errno) );
    //     return 1;
    // }
    // if (n == 0) {
    //     syslog(LOG_ERR, "nothing to read" );
    //     return 2;
    // }

    // c = memset(cmd_string, 0, sizeof(cmd_string));

    // if (poll_fd.revents & POLLIN) {
    //     n = read(poll_fd.fd, c, 1 );
    //     if (n <= 0) return 3;
    //     if ( *c != Commands[ icp_cmd ].rpref) return 3;
    //     c++;
    //     if ( !Commands[icp_cmd].anonymous ) {
    //         n = read(poll_fd.fd, c, 2 );
    //         if (n<2) return 4;
    //         // проверку алреса ответившего
    //         c += n;
    //     }
    //     // if (strlen(c)>0) {
    //     //     memcpy(data, c, strlen(c)-1);
    //     // }
    // }	// if poll



    return 0;
}
