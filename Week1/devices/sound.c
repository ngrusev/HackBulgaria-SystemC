#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#include <linux/input.h>

#include <string.h>

int main(int argc, char *argv[]) {
	int fdSound, fdMouse;
	struct input_event ie;
	
	if((fdSound = open("/dev/dsp", O_WRONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}

	if((fdMouse = open("/dev/input/event4", O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}

	while(read(fdMouse, &ie, sizeof(struct input_event))) {
		if (!ie.code && !ie.value)
			continue;
		int value = 20000 + ie.code * 10000 + ie.value * 5000;
		int times = 3;
		while(times--)
			write(fdSound, &value, sizeof(int));
	}

	return 0;
}
