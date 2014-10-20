#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define EXIT_FAILURE -1

char *readline(int fd) {
	char *line = malloc(64);
	int size = 64;
	int rc = 0;
	int index = 0;
	while(read(fd, line + index, 1)) {
		if (line[index] == '\n') {
			line[index] = 0;
			return line;
		}		

		index++;
		if(index >= size) {
			size *= 2;
			line = realloc(line, size);
		}
	}
	free(line);
	return NULL;
}

int ishex(char ch) {
	return ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f';
}

int parsehex(char *str, int *index, char terminator, off_t *result) {
	*result = 0;
	while(str[*index] != 0 && str[*index] != terminator) {
		if (!ishex(str[*index])) {
			return 0;
		}
		*result *= 16;
		*result +=  (str[*index] < 'a') ? str[*index] - '0' : str[*index] - 'a' + 10;
		*index += 1;
	}
	*index += 1;
	return str[*index - 1] == terminator;
}

int getrange(int fd, off_t *startp, off_t *endp) {	
	char *line;
	while(line = readline(fd)) {
		int index = 0;
		
		if (parsehex(line, &index, '-', startp) && parsehex(line, &index, ' ', endp) && line[index] == 'r') {
			free(line);
			return 1;
		}
		
		free(line);
	}
	
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Invalid usage!\n");
		exit(EXIT_FAILURE);
	}
	
	int arglen = strlen(argv[1]);
	
	int piddirlen = 6 + arglen + 1;
	char *piddir = (char *)malloc(piddirlen + 1);
	strcpy(piddir, "/proc/");
	strcpy(piddir + 6, argv[1]);
	piddir[piddirlen - 1] = '/';
	piddir[piddirlen] = '\0';

	char *mapsloc = (char *)malloc(piddirlen + 5);
	strcpy(mapsloc, piddir);
	strcpy(mapsloc + piddirlen, "maps");

	char *memloc = (char *)malloc(piddirlen + 4);
	strcpy(memloc, piddir);
	strcpy(memloc + piddirlen, "mem");
	
	free(piddir);
	
	int fdmaps, fdmem;
	if ((fdmaps = open(mapsloc, O_RDONLY)) < 0 || (fdmem = open(memloc, O_RDONLY)) < 0) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	
	free(mapsloc); free(memloc);

	off_t start, end;
	while(getrange(fdmaps, &start, &end)) {
		lseek(fdmem, start, SEEK_SET);
		char buf[64];
		off_t current = start;
		while (current < end) {
			int size = 64;
			if (current + size > end)
				size = end - current;
			current += size;
			int rc;	
			if ((rc = read(fdmem, buf, size)) < size) {
				printf("Read problem! %llu %llu\n", start, end);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			write(1, buf, size);
		}
	}
	printf("\n");
	return 0;
}
