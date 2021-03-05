#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wiringPi.h"

#define BUFFER_SIZE 32

int main(int argc, char **argv)
{
	double min1_load, min5_load, min15_load;
	int proc_fd, proc_rd, proc_cl, i, debug_flag;
	char *proc_path = "/proc/loadavg";
	char load_str[BUFFER_SIZE];
	int min1_vals[8], min5_vals[8];

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
		sscanf(load_str,"%lf %lf %lf",&min1_load,&min5_load,&min15_load);
		

		// Close file
		proc_cl = close(proc_fd);
		if (proc_cl < 0) {
			fprintf(stderr,"ERROR: cannot close file!  %s\n",strerror(errno));
			exit(errno);
		}

		// Write the appropriate output to the pins
		for (i = 0; i < 8; i++)
			digitalWrite(7 - i, (min1_load >= (4.0 / (1 << i))) ? 1 : 0);
		// DEBUG: print 1-minute cpu load average and GPIO status
		if (debug_flag) {
			for (i = 0; i < 8; i++)
				min1_vals[i] = digitalRead(i);
		}
		
		sleep(2);
		for (i = 0; i < 8; i++)
			digitalWrite(7 - i, (min5_load >= (4.0 / (1 << i))) ? 1 : 0);
		// DEBUG: print 5-minute cpu load average and GPIO status
		if (debug_flag) {
			for (i = 0; i < 8; i++)
				min5_vals[i] = digitalRead(i);
		}

		sleep(2);
		for (i = 0; i < 8; i++)
			digitalWrite(7 - i, (min15_load >= (4.0 / (1 << i))) ? 1 : 0);
		// DEBUG: print cpu load and GPIO status
		if (debug_flag) {
			printf("%s %.2lf | %.2lf | %.2lf\n",
					"CPU Load [1m | 5m | 15m]:",
					min1_load, min5_load, min15_load);
			for (i = 0; i < 8; i++)
				printf("GPIO %u: %u | %u | %u\n",
						i,min1_vals[i],min5_vals[i],digitalRead(i));
		}

		sleep(2);
		for (i = 0; i < 8; i++)
			digitalWrite(7 - i, 0);

		// Update data about once every 7 seconds
		sleep(1);
	}
	return 0;
}
