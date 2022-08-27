#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>



int main( int argc, char ** argv) {

	int serial_port = open("/dev/ttyUSB0", O_RDWR);
	if (serial_port < 0) {
		printf("Error %i from open: %s\n", errno, strerror(errno));
	}
	struct termios tty;
	if(tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= CREAD | CLOCAL;

	tty.c_lflag &= ~ICANON; //don't line buffer 
	tty.c_lflag &= ~ISIG;

	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
	tty.c_oflag &= ~OPOST;

	cfsetispeed(&tty, B460800);
	cfsetospeed(&tty, B460800);
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}

}
