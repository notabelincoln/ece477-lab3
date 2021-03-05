#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wiringPi.h"
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 32

int main(int argc, char **argv)
{
	double cpu_load;
	int proc_fd, proc_rd, proc_cl, i, debug_flag;
	char *proc_path = "/proc/loadavg";
	char load_str[BUFFER_SIZE];

	// Set up GPIOs as outputs
	wiringPiSetup();
	for (i = 0; i < 8; i++)
		pinMode(i, OUTPUT);

	if (argc >= 2)
		debug_flag = !(strcmp("debug",argv[1]));
	else
		debug_flag = 0;

	while(1) {
		// Reset buffer so useless/outdated info is purged
		memset(load_str, 0, BUFFER_SIZE);

		// Open file for reading CPU load
		proc_fd = open(proc_path, O_RDONLY);
		if (proc_fd < 0) {
			fprintf(stderr,"ERROR: cannot open file!  %s\n",strerror(errno));
			exit(errno);
		}

		// Read the contents of file into buffer
		proc_rd = read(proc_fd, load_str, BUFFER_SIZE-1);
		if (proc_rd < 0) {
			fprintf(stderr,"ERROR: cannot read file!  %s\n",strerror(errno));
			exit(errno);
		}

		// Read minute-load-average into variable
		sscanf(load_str,"%lf",&cpu_load);
		

		// Close file
		proc_cl = close(proc_fd);
		if (proc_cl < 0) {
			fprintf(stderr,"ERROR: cannot close file!  %s\n",strerror(errno));
			exit(errno);
		}

		// Write the appropriate output to the pins
		for (i = 0; i < 8; i++)
			digitalWrite(7 - i, (cpu_load >= (4.0 / (1 << i))) ? 1 : 0);

		// DEBUG: print cpu load and GPIO status
		if (debug_flag) {
			printf("1-minute load average is %.2lf\n",cpu_load);
			for (i = 0; i < 8; i++)
				printf("GPIO %u: %u\n",i,digitalRead(i));
		}

		// Update data about once a second
		sleep(1);
	}
	return 0;
}
