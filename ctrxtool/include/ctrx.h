/*
 * ctrx.h - CTRX defines.
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

#ifndef CTRX_H
#define CTRX_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t magic; 				/**< Magic (3DSX). */
	uint16_t header_size; 			/**< Header size. */
	uint16_t reloc_header_size; 	/**< Relocation Header size. */
	uint32_t version; 				/**< Format version. */
	uint32_t flags; 				/**< Flags. */
	uint32_t code_size; 			/**< Code segment size. */
	uint32_t rodata_size; 			/**< Rodata segment size. */
	uint32_t data_size; 			/**< Data segment size (including BSS size). */
	uint32_t bss_size; 				/**< BSS segment size. */
} CTRX_Header;

typedef struct {
	uint32_t abs_count; 			/**< Number of absolute relocations. */
	uint32_t rel_count; 			/**< Number of relative relocations. */
} CTRX_Relocation_Header;

typedef struct {
	uint16_t skip_count; 			/**< Number of words to skip. */
	uint16_t patch_count; 			/**< Number of words to patch. */
} CTRX_Relocation;


/** Opaque data type for CTRX. */
typedef struct _CTRX CTRX;

/* Create a new CTRX object. */
int ctrx_new(CTRX* ctrx, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
