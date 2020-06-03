#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>


int		baud = 9600;		// скорость порта
char	fn_port[128];		// имя порта
int		fd_port;			// дескриптор порта

//--------------------------------------------------

// int set_baud(char* baud_s) {
//     int b;
//     int baud;

//     b = atoi(baud_s);
//     switch (b) {
//         case 300 : baud = B300; break;
//         case 1200 : baud = B1200; break;
//         case 2400 : baud = B2400; break;
//         case 9600 : baud = B9600; break;
//         case 19200 : baud = B19200; break;
//         case 38400 : baud = B38400; break;
//         case 57600 : baud = B57600; break;
//         case 115200 : baud = B115200; break;
//         default : baud = 0;
//     };
//     return baud;
// }

// converts integer baud to Linux define
int get_baud(int baud)
{
	switch (baud) {
    case 300:
        return B300;
    case 1200:
        return B1200;
    case 2400:
        return B2400;
    case 4800:
        return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default: 
		return -1;
	}
}


// int set_interface_attribs (int fd, int speed, int parity)
// {
//         struct termios tty;
//         memset (&tty, 0, sizeof tty);
//         if (tcgetattr (fd, &tty) != 0)
//         {
//                 syslog(LOG_ERR,"error %d from tcgetattr", errno);
//                 return -1;
//         }
// //
// //   tty.c_cflag = (CS8 | CREAD) & ~PARENB; /* 8N1 */
// // //  tty.c_iflag |= IXON | IXOFF;
// //   tty.c_lflag &= ~ICANON & ~ISIG;
// //
//         cfsetospeed (&tty, speed);
//         cfsetispeed (&tty, speed);
// 		cfmakeraw(&tty);
//         tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
//         tty.c_cflag &= ~CSTOPB;
// //        tty.c_cflag &= ~CRTSCTS;
//         tty.c_cc[VMIN]  = 1;            // read doesn't block - 0
//         tty.c_cc[VTIME] = 2;            // 0.5 seconds read timeout - 5
// /*
//         tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
//         // disable IGNBRK for mismatched speed tests; otherwise receive break
//         // as \000 chars
//         tty.c_iflag &= ~IGNBRK;         // disable break processing
//         tty.c_lflag = 0;                // no signaling chars, no echo,
//                                         // no canonical processing
//         tty.c_oflag = 0;                // no remapping, no delays
//         tty.c_cc[VMIN]  = 0;            // read doesn't block
//         tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
//         tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
//         tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
//                                         // enable reading
//         tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
//         tty.c_cflag |= parity;
//         tty.c_cflag &= ~CSTOPB;
//         tty.c_cflag &= ~CRTSCTS;
// */
//         if (tcsetattr (fd, TCSANOW, &tty) != 0)
//         {
//                 syslog(LOG_ERR,"error %d from tcsetattr", errno);
//                 return -1;
//         }
//         return 0;
// }


//=================================================

int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios options;


	/* Configure port reading */
	//fcntl(fd, F_SETFL, 0); 	//read com-port is the bloking
	fcntl(fd, F_SETFL, FNDELAY);  //read com-port not bloking
	//fcntl(fd, F_SETFL, O_NDELAY);  //read com-port not bloking
	
	//ioctl(fd, FIOASYNC, 1);
	
/* Get the current options for the port */
	tcgetattr(fd, &options);
	if(0 != baud) {
		cfsetispeed(&options,get_baud(baud));
	} else {
		cfsetispeed(&options, B9600);	
	}
//	cfsetispeed(&options, B9600);	
	 
/* Enable the receiver and set local mode */
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB; 						/* Mask the character size to 8 bits, no parity */
	options.c_cflag &= ~CSTOPB; 						//one stop bit
//	options.c_cflag |= CSTOPB;							//two stop bit
	options.c_cflag &= ~CSIZE;
	options.c_cflag |=  CS8;							/* Select 8 data bits */
	options.c_cflag &= ~CRTSCTS;						/* Disable hardware flow control */  
	options.c_oflag &= ~OPOST;							/* Disable postprocessing */  
	

	/* Software flow control is disabled */
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
 
/* Enable data to be processed as raw input */
//	options.c_lflag &= ~(ICANON | ECHO | ISIG);

	options.c_lflag &= ~(ECHO | ISIG);
	options.c_lflag |= ICANON;
/* Set the new options for the port */


// Добавлено для проверки
	options.c_cc[VMIN]  = 1; //50;
	options.c_cc[VTIME] = 5; //10; 
	
	options.c_iflag |= IGNBRK;
//До этого момента всё завелось

//	options.c_iflag &= ~ICRNL;
	options.c_iflag |= ICRNL;

	options.c_oflag &= ~ONLCR;
	options.c_lflag &= ~IEXTEN;
	options.c_lflag &= ~ECHOE;
	options.c_lflag &= ~ECHOK;
#ifndef OPENWRT_ARM
	options.c_lflag &= ~ECHOCTL;
	options.c_lflag &= ~ECHOKE;
#endif
	tcsetattr(fd, TCSANOW, &options);
	return fd;
}


void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                syslog(LOG_ERR,"error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                syslog(LOG_ERR,"error %d setting term attributes", errno);
}

// 9600 8N1

void init_port() {

	if (fd_port) close(fd_port);
	
	fd_port = open (fn_port, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (fd_port < 0)
	{
			syslog(LOG_ERR, "error %d opening %s: %s", errno, fn_port, strerror (errno) );
			closelog();
			exit(1);
	}

	set_interface_attribs (fd_port, baud, 0);  // set speed to 115,200 bps, 8n1 (no parity)
//	set_blocking (fd_port, 0);                // set no blocking

	syslog(LOG_INFO, "port %s configured", fn_port);

	tcflush(fd_port,TCIOFLUSH);
}


void tohex(unsigned char * in, size_t insz, char * out, size_t outsz)
{
    unsigned char * pin = in;
    const char * hex = "0123456789ABCDEF";
    char * pout = out;
	memset(out, 0, outsz);
    for(; pin < in+insz; pout +=2, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        if (pout + 2 - out > outsz){
            /* Better to truncate output string than overflow buffer */
            /* it would be still better to either return a status */
            /* or ensure the target buffer is large enough and it never happen */
            break;
        }
    }
    *pout = 0;
}

//================================================= crc calculation

unsigned short crc16(const unsigned char* data_p, unsigned char length){
    unsigned char x,i;
    unsigned short crc = 0;		// CRC-16 XMODEM. Otherwise 0xFFFF

	i = length;
    while (i--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

