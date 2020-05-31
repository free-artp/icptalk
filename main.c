
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "comm.h"
#include "proto.h"

int main(int argc, char* argv[]) {

	pthread_t twriter, treader, tscheduler;
	int result;

	openlog("talker", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "start");

	if (config(argc, argv)) {
		perror("config troubles");
		exit(1);
	}

	init_port();

	//----------------------------------------------------------
	
	result = pthread_create(&treader, NULL, reader, NULL);
	if ( result != 0 ) {
		syslog(LOG_ERR, "could not create thread reader");
		exit(0);
	}
	syslog(LOG_INFO, "reader created");

	result = pthread_create(&twriter, NULL, writer, NULL);
	if ( result != 0 ) {
		syslog(LOG_ERR, "could not create thread writer");
		exit(0);
	}
	syslog(LOG_INFO, "writer created");

	result = pthread_join(treader, NULL);
	if (result != 0) {
		perror("Joining the reader thread");
		return EXIT_FAILURE;
	}
	result = pthread_join(twriter, NULL);
	if (result != 0) {
		perror("Joining the writer thread");
		return EXIT_FAILURE;
	}



	// result = pthread_create(&tscheduler, NULL, scheduler_executor, NULL);
	// if ( result != 0 ) {
	// 	syslog(LOG_ERR, "could not create thread sheduler_executor");
	// 	exit(0);
	// }
	// syslog(LOG_INFO, "scheduler_executor created");
	//----------------------------------------------------------


	// result = pthread_join(tscheduler, NULL);
	// if (result != 0) {
	// 	perror("Joining the 3 thread");
	// 	return EXIT_FAILURE;
	// }

//	writer((void*)0);

	printf("Done\n");
	exit(0);

}
