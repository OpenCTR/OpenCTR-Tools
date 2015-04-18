/*
 * utils.c - utility functions.
 * 
 * Copyright 2015, Alex Shaw <alex.shaw.as@gmail.com>
 * 
 * This file is part of OpenCTR.
 * 
 * OpenCTR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenCTR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenCTR.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#ifdef DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif

#include "utils.h"

int readfile(unsigned char* buffer[], unsigned int* size, const char* filename) {
	struct stat st;
	int ret;
	unsigned int filesize;
	unsigned char* filebuffer;
	int fd;

	if(filename == NULL) {
		return -1;
	}

	fd = open(filename, O_RDONLY);
	if(fd == -1) {
		return -1;
	}

	ret = fstat(fd, &st);
	if(ret == -1) {
		return -1;
	}

	filesize = st.st_size;

	filebuffer = (unsigned char*)malloc(sizeof(unsigned char) * filesize);
	ASSERT(filebuffer != NULL);

	memset(filebuffer, 0x00, sizeof(unsigned char) * filesize);

	ret = read(fd, filebuffer, sizeof(unsigned char) * filesize);
	if(ret == -1) {
		return -1;
	}

	ret = close(fd);
	if(ret == -1) {
		return -1;
	}

	if(buffer) {
		*buffer = filebuffer;
	}

	if(size) {
		*size = filesize;
	}

	return 0;
}

