static const int block_size = 4096;

void copy_contents(int fdin, int fdout) {
	char buff[block_size];
	int rcount;
	while(rcount = read(fdin, buff, block_size)) {
		write(fdout, buff, rcount);
	}
}
