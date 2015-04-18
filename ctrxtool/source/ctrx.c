/*
 * ctrx.c - CTRX functions
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ctrx.h"

#ifdef HAVE_LIBELF
#include "libelf.h"
#endif

#ifdef HAVE_GELF
#include "gelf.h"
#endif

struct _CTRX {
	CTRX_Header header;
	CTRX_Relocation_Header* rel_headers;
	uint32_t rel_headers_size;
	CTRX_Relocation* relocs;
	uint32_t relocs_size;
};

int ctrx_new(CTRX* ctrx, const char* filename) {
	if(filename == NULL) {
		return -1;
	}

	return 0;
}

