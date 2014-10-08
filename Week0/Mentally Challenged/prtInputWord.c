#include <stdio.h>
#include <errno.h>

int is_whitespace(char ch) {
	return ch == ' ' || ch == '\f' || ch == '\n' ||
		ch == '\r' || ch == '\t' || ch == '\v';
}

void write_input_word_to_output() {
	char buff[32];
	int rcount;
	while ((rcount = read(0, buff, 32))) {
		int i;
		for (i = 0; i < rcount; i++)
			if (is_whitespace(buff[i]))
				break;
		
		write(1, buff, i);

		if (i < rcount)
			break;
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		int fd = creat(argv[1], 0600);
		if (fd >= 0)
			dup2(fd, 1);
		else
			perror(NULL);
	}
	
	write_input_word_to_output();
	return 0;
}
