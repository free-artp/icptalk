#ifndef __SERIAL_H_
#define __SERIAL_H_

extern int		baud;       		// скорость порта
extern char	    fn_port[128];		// имя порта
extern int		fd_port;			// дескриптор порта


//int set_baud(char* baud_s);
int get_baud(int baud);
int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

void tohex(unsigned char * in, size_t insz, char * out, size_t outsz);

void init_port();

#endif
