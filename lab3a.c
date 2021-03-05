#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wiringPi.h"
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include "string.h"

int main(int argc, char **argv)
{
	unsigned int pin_mask, i;
	
	if (argc != 2) {
		printf("USAGE: ./lab2a (value)\n");
		exit(-1);
	}

	// Check if input pin mask is valid
	if (!(strncmp("0x",argv[1],2))) {
		if (strlen(argv[1]) < 3) {
			printf("ERROR: Invalid value (base 16)\n");
			exit(-2);
		} else {
			for (i = 2; i < strlen(argv[1]); i++) {
				// Check if each character is not valid hex value (between '0' & '7', 'a' & 'f', 'A' & 'F')
				if (!(((argv[1][i] <= '9') && (argv[1][i] >= '0'))
							|| ((argv[1][i] <= 'f') && (argv[1][i] >= 'a'))
							|| ((argv[1][i] <= 'F') && (argv[1][i] >= 'A')))) {
					printf("ERROR: Invalid value(base 16)\n");
					exit(-2);
				}
			}
		}
	} else if (argv[1][0] == '0') {
		// Check if each value is valid octal digit
		for (i = 0; i < strlen(argv[1]); i++) {
			if (!((argv[1][i] <= '7') && (argv[1][i] >= '0'))) {
				printf("ERROR: Invalid pin mask (base 8)\n");
				exit(-3);
			}
		}
	} else {
		// Check if each value is valid decimal digit
		for (i = 0; i < strlen(argv[1]); i++) {
			if (!((argv[1][i] <= '9') && (argv[1][i] >= '0'))) {
				printf("ERROR: Invalid pin mask (base 10)\n");
				exit(-3);
			}
		}
	}


	// Determine base of input pin mask
	if (argv[1][0] == '0') {
		if ((argv[1][1] == 'x') || (argv[1][1] == 'X'))
			pin_mask = strtol(argv[1], NULL, 16);
		else
			pin_mask = strtol(argv[1], NULL, 8);
	} else {
		pin_mask = atoi(argv[1]);
	}

	// Check if value is in valid range
	if (pin_mask > 0xFF) {
		printf("ERROR: value exceeds valid input\n");
		exit(-4);
	}

	// Set up GPIO
	wiringPiSetup();

	// Set each GPIO high or low depending on the logical result
	for (i = 0; i < 8; i++) {
		pinMode(i, OUTPUT);
		if (pin_mask & (1 << i))
			digitalWrite(i, 1);
		else
			digitalWrite(i, 0);
	}

	// USE FOR DEBUGGING
	//printf("pin_mask: %#4x\n",pin_mask);

	return 0;
}
