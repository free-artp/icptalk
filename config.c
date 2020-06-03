#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <getopt.h>


#include "serial.h"

int verbose = 0;

// return	1 - fail
//			0 - success
int config(int argc, char* argv[]) {
	int opt;
	int n;
	char buf[8];
	for(opt=getopt(argc, argv, "p:b:v:"); opt != -1; opt=getopt(argc, argv, "p:b:v:")) {
		switch (opt) {
            case 'b' : { /* set baud rate */
			  baud = atoi(optarg);
			  if (baud == 0 || get_baud(baud) < 0) {
				syslog(LOG_ERR, "baud rate [%s] not supported", optarg);
				return 1;
			  }
			}; break;
			case 'p' : { /* serial device */
			  n = snprintf(fn_port, sizeof(fn_port), "%s", optarg);
			  if (n < 0 || n > sizeof(fn_port)) {
				syslog(LOG_ERR, "output filename truncated, longer than %ld bytes", sizeof(fn_port));
				return 1;
			  }
			}; break;
			case 'v' : {
				verbose = atoi(optarg);
			}; break;
        }
    }
	return 0;
}