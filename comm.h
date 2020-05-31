#ifndef COMM_H
#define COMM_H

	#define DEFAULT_SERIAL_PORT "/dev/ttyUSB0"
	#define DEFAULT_BAUD B9600
	#define DEFAULT_DELAY 1000000		// usec

	#define BUFF_LEN 2048

	extern unsigned char	buffer[BUFF_LEN];
	extern unsigned int		buff_cnt;

	extern int		baud;				// скорость на порту
	extern char		fn_port[128];		// имя порта
	extern int		fd_port;			// дескриптор порта

	int set_baud(char* baud_s);
	void init_port();

	void tohex(unsigned char * in, size_t insz, char * out, size_t outsz);

	void * writer(void *arg);
	void * reader(void *arg);

#endif