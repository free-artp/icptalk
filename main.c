
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "serial.h"
#include "proto.h"

int main(int argc, char* argv[]) {

//	pthread_t twriter, treader, tscheduler;
//	int result;
	char data[20];
	double a;
	int cnt;

	openlog("icptalk", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "start");

	if (config(argc, argv)) {
		perror("config troubles");
		exit(1);
	}

	init_port();
	set_blocking(fd_port,0);

	icp_talk(1, ICP_ReadModuleName, NULL, NULL );
	icp_talk(1, ICP_ReadFirmwareVersion, NULL, NULL );
	icp_talk(1, ICP_SetModuleConf, "01050680", NULL ); // <new addr><+/- 2.5V><9600 baud><50Hz, no CheckSumm, Engineer units>

	while(1) {
		if (!icp_talk(1, ICP_ReadAnalogInput, NULL, data ) )
			a = atof(data);
		if (!icp_talk(1, ICP_ReadEventCounter, NULL, data ))
			cnt = atoi(data);
		printf(": %f %d\n", a, cnt);
	}


	printf("Done\n");
	exit(0);

}
