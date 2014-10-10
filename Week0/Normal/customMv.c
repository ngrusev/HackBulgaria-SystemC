#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/iohelper.h"
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 3)
		_exit(-1);

	char *source = argv[1];
	char *destination = argv[2];	

	if (link(source, destination)) {
		int fdin, fdout;
		if ((fdin = open(argv[1], O_RDONLY)) < 0 || (fdout = creat(argv[2], 0600)) < 0) {
			perror(NULL);
			_exit(-1);
		}
		copy_contents(fdin, fdout);
	}

	if (unlink(source)) {
		perror(NULL);
		_exit(-1);
	}
	return 0;
}
