#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/iohelper.h"

int main(int argc, char *argv[]) {
	int fd = 0;
	if (argc > 1 && (fd = open(argv[1], O_RDONLY)) < 0) {
		perror(NULL);
		_exit(-1);
	}
	copy_contents(fd, 1);
	return 0;
}
