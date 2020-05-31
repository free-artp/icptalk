#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <getopt.h>


#include "comm.h"

// return	1 - fail
//			0 - success
int config(int argc, char* argv[]) {
	int opt;
	int n;
	char buf[8];
	for(opt=getopt(argc, argv, "p:b:"); opt != -1; opt=getopt(argc, argv, "c:p:b:m:dh")) {
		switch (opt) {
            case 'b' : { /* set baud rate */
			  n = snprintf(buf, sizeof(buf), "%s", optarg);
			  if (n < 0 || n > sizeof(buf)) {
				syslog(LOG_ERR, "couldn't convert -b option into a baud rate");
				return 1;
			  }
			  baud = set_baud(buf);
			  if (baud == 0) {
				syslog(LOG_ERR, "baud rate [%s] not supported", buf);
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

        }
    }
	return 0;
}