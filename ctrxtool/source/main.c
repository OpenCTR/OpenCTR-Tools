/*
 * main.c - Program entry point.
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

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef HAVE_LIBELF_H
#include "libelf.h"
#endif

#ifdef HAVE_GELF_H
#include "gelf.h"
#endif

#include "endian.h"
#include "utils.h"
#include "ctrx.h"

#ifndef PATH_MAX
#define PATH_MAX (255)
#endif

static void write32(int fd, uint32_t val) {
	int ret;

	ret = write(fd, &val, sizeof(uint32_t));
}

static void write16(int fd, uint16_t val) {
	int ret;

	ret = write(fd, &val, sizeof(uint16_t));
}


int main(int argc, char *argv[]) {
	int ret;
	int bflag = 0;
	int ch;
	int show_usage = 0;
	int show_version = 0;
	const struct option options[] = {
		{ "version", 0, NULL, 'v' }, 
		{ "help", 0, NULL, 'h' }, 
		{ "output", 1, NULL, 'o' }, 
		{ NULL, 0, NULL, 0 } 
	};
	const char* program_name = basename(argv[0]);
	const char* output_filename = NULL;
	const char* input_filename = NULL;
	unsigned char* data = NULL;
	unsigned int size = 0;
	Elf32_Ehdr* ehdr = NULL;
	unsigned int i;
	unsigned int j;
	Elf* e = NULL;
	size_t phdr_size;
	GElf_Phdr phdr[3];
	int fd;
	size_t shdr_size;

	while(1) {
		if(bflag) {
			break;
		}
		ch = getopt_long(argc, argv, "vho:", options, NULL);
		switch(ch) {
			case 'v':
				show_version = 1;
				bflag = 1;
				break;
			case 'h':
				show_usage = 1;
				bflag = 1;
				break;
			case 'o':
				output_filename = optarg;
				break;
			default:
				bflag = 1;
				break;
		}
	}

	if((show_usage == 0) && (show_version == 0)) {
		if(argc != optind + 1) {
			fprintf(stderr, "%s: Must specify an input file\n\n", program_name);
			show_usage = 1;
		}
	}

	if(show_usage) {
		printf("%s %s - %s\n", PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_DESCRIPTION);
		printf("\n");
		printf("Usage:\n");
		printf("  %s [options] input.elf\n", program_name);
		printf("Options:\n");
		printf("  -h, --help          Print this help message\n");
		printf("  -v, --version       Print the version number\n");
		printf("  -o, --output=OUTPUT Use OUTPUT as the output file\n");
		exit(EXIT_SUCCESS);
	} else if(show_version) {
		printf("%s\n", PACKAGE_VERSION);
		exit(EXIT_SUCCESS);
	}

	input_filename = argv[optind];
	if(output_filename == NULL) {
		char cwd[PATH_MAX];
		getcwd(cwd, PATH_MAX);
		asprintf((char**)&output_filename, "%s/output.ctrx", cwd);
	}

	/* Read input file into memory. */
	ret = readfile(&data, &size, input_filename);
	if(ret != 0) {
		fprintf(stderr, "%s: Error using \'%s\' as input file\n", program_name, input_filename);
		exit(EXIT_FAILURE);
	}

	/* Initialize libelf */
	ret = elf_version(EV_CURRENT);
	if(ret == EV_NONE) {
		fprintf(stderr, "%s: Error initializing libelf - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Create a libelf context from the ELF file data. */
	e = elf_memory((char*)data, (size_t)size);
	if(e == NULL) {
		fprintf(stderr, "%s: Error using input file \'%s\' as ELF file - %s\n", program_name, input_filename, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Make sure the user has given a valid ELF file. */
	if(elf_kind(e) != ELF_K_ELF) {
		fprintf(stderr, "%s: Input file \'%s\' is not a valid ELF file\n", program_name, input_filename);
		exit(EXIT_FAILURE);
	}

	ehdr = elf32_getehdr(e);

	/* Check that input is 32-bit ELF */
	if(ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
		fprintf(stderr, "%s: Input file is not 32-bit\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Check that input is ARM binary */
	if(ehdr->e_machine != EM_ARM) {
		fprintf(stderr, "%s: Input file is not ARM-format\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Check that input is Executable ELF */
	if(ehdr->e_type != ET_EXEC) {
		fprintf(stderr, "%s: Input file is not executable\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Get the number of program headers. */
	ret = elf_getphdrnum(e, &phdr_size);
	if(ret != 0) {
		fprintf(stderr, "%s: elf_getphdrnum() failed - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Make sure the ELF file has 3 program headers. */
	if(phdr_size != 3) {
		fprintf(stderr, "%s: Incorrect number of segments\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Program Header 1 */
	if(gelf_getphdr(e, 0, &phdr[0]) == NULL) {
		fprintf(stderr, "%s: gelf_getphdr() failed - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Program Header 2 */
	if(gelf_getphdr(e, 1, &phdr[1]) == NULL) {
		fprintf(stderr, "%s: gelf_getphdr() failed - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Program Header 3 */
	if(gelf_getphdr(e, 2, &phdr[2]) == NULL) {
		fprintf(stderr, "%s: gelf_getphdr() failed - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	/* Check if segments are word-aligned. */
	for(i=0; i<3; i++) {
		// TODO: Should p_align be used for checking alignment?

		if((phdr[i].p_memsz & 3) || (phdr[i].p_filesz & 3)) {
			fprintf(stderr, "%s: Segment %d is not word-aligned\n", program_name, i+1);
			exit(EXIT_FAILURE);
		}
	}

	/* Make sure section 1 is code segment. */
	if(phdr[0].p_flags != (PF_R + PF_X)) {
		fprintf(stderr, "%s: Segment 1 is not the code segment\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Make sure section 2 is rodata segment. */
	if(phdr[1].p_flags != PF_R) {
		fprintf(stderr, "%s: Segment 2 is not the rodata segment\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Make sure section 3 is BSS segment. */
	if(phdr[2].p_flags != (PF_R + PF_W)) {
		fprintf(stderr, "%s: Segment 3 is not the BSS segment\n", program_name);
		exit(EXIT_FAILURE);
	}

	/* Check entry point. */
	if(ehdr->e_entry != phdr[0].p_vaddr) {
		fprintf(stderr, "%s: Incorrect entry point\n", program_name);
		exit(EXIT_FAILURE);
	}

	const uint32_t bss_size = phdr[2].p_memsz - phdr[2].p_filesz;

	ret = elf_getshdrnum(e, &shdr_size);
	if(ret != 0) {
		fprintf(stderr, "%s: elf_getshdrnum() failed - %s\n", program_name, elf_errmsg(-1));
		exit(EXIT_FAILURE);
	}

	//printf("shdr_size=%zu\n", shdr_size);


	Elf32_Shdr* elfSects = (Elf32_Shdr*)(data + ehdr->e_shoff);

	for(i=0; i<shdr_size; i++) {
		Elf_Scn* scn = elf_getscn(e, i);
		if(scn == NULL) {
			fprintf(stderr, "%s: elf_getscn() failed - %s\n", program_name, elf_errmsg(-1));
			exit(EXIT_FAILURE);
		}

		Elf32_Shdr* shdr = elf32_getshdr(scn);
		if(shdr == NULL) {
			fprintf(stderr, "%s: elf32_getshdr() failed - %s\n", program_name, elf_errmsg(-1));
			exit(EXIT_FAILURE);
		}


		if(shdr->sh_type == SHT_RELA) {
			fprintf(stderr, "%s: Relocations with addends are not supported\n", program_name);
			exit(EXIT_FAILURE);
		} else if(shdr->sh_type == SHT_SYMTAB) {
			Elf_Data* data = elf_getdata(scn, NULL);
			if(data == NULL) {
				fprintf(stderr, "%s: elf_getdata() failed - %s\n", program_name, elf_errmsg(-1));
				exit(EXIT_FAILURE);
			}

			//Elf32_Sym* manual_way = (Elf32_Sym*)(data + shdr->sh_offset);
			//Elf32_Sym* new_way = (Elf32_Sym*)gelf_getsym(data, shdr->sh_offset, NULL);

			

			/*
			extern Elf_Data *elf_getdata __P((Elf_Scn *__scn, Elf_Data *__data));
			extern GElf_Sym         *gelf_getsym __P((Elf_Data *__src, int __ndx, GElf_Sym *__dst));
			elfSyms = (Elf32_Sym*) (img + le_word(sect->sh_offset));
			elfSymCount = le_word(sect->sh_size) / sizeof(Elf32_Sym);
			elfSymNames = (const char*)(img + le_word(elfSects[le_word(sect->sh_link)].sh_offset));
			*/
		} else if(shdr->sh_type == SHT_REL) {
			Elf_Scn* target_scn = elf_getscn(e, shdr->sh_info);
			if(target_scn == NULL) {
				fprintf(stderr, "%s: elf_getscn() failed - %s\n", program_name, elf_errmsg(-1));
				exit(EXIT_FAILURE);
			}

			Elf32_Shdr* target_shdr = elf32_getshdr(target_scn);
			if(target_shdr == NULL) {
				fprintf(stderr, "%s: elf32_getshdr() failed - %s\n", program_name, elf_errmsg(-1));
				exit(EXIT_FAILURE);
			}

			/* Only process loadable sections */
			if(target_shdr->sh_flags & SHF_ALLOC) {
				//uint8_t* sectData = data + (target_shdr->sh_offset);

				Elf32_Rel* relTab = (Elf32_Rel*)(data + shdr->sh_offset);
				Elf32_Sym* symTab = (Elf32_Sym*)(data + (elfSects[shdr->sh_link].sh_offset));

				const int nentries = shdr->sh_size / shdr->sh_entsize;
				for(j=0; j<nentries; j++) {
					Elf32_Rel* reloc = relTab + j;
				}

//extern int elf_getshdrnum __P((Elf *__elf, size_t *__resultp));

/*
				u32 vsect = le_word(targetSect->sh_addr);
				byte_t* sectData = img + le_word(targetSect->sh_offset);
				Elf32_Sym* symTab = (Elf32_Sym*)(img + le_word(elfSects[le_word(sect->sh_link)].sh_offset));
				Elf32_Rel* relTab = (Elf32_Rel*)(img + le_word(sect->sh_offset));
				int relCount = (int)(le_word(sect->sh_size) / le_word(sect->sh_entsize));
				safe_call(ScanRelocSection(vsect, sectData, symTab, relTab, relCount));
*/
			}

		}

	}

/*
int ElfConvert::ScanRelocSection(u32 vsect, byte_t* sectData, Elf32_Sym* symTab, Elf32_Rel* relTab, int relCount)
{
	for (int i = 0; i < relCount; i ++)
	{
		Elf32_Rel* rel = relTab + i;
		u32 relInfo = le_word(rel->r_info);
		int relType = ELF32_R_TYPE(relInfo);
		Elf32_Sym* relSym = symTab + ELF32_R_SYM(relInfo);

		u32 relSymAddr = le_word(relSym->st_value);
		u32 relSrcAddr = le_word(rel->r_offset);
		u32& relSrc = *(u32*)(sectData + relSrcAddr - vsect);

		switch (relType)
		{
			// Notes:
			// R_ARM_TARGET2 is equivalent to R_ARM_REL32
			// R_ARM_PREL31 is an address-relative signed 31-bit offset

			case R_ARM_ABS32:
			case R_ARM_TARGET1:
			{
				if(relSrcAddr & 3)
					die("Unaligned relocation!");

				// Ignore unbound weak symbols (keep them 0)
				if (ELF32_ST_BIND(le_word(relSym->st_info)) == STB_WEAK && relSymAddr == 0) break;

				// Add relocation
				relSrc -= baseAddr;
				SetReloc(relSrcAddr, absRelocMap);
				break;
			}

			case R_ARM_REL32:
			case R_ARM_TARGET2:
			case R_ARM_PREL31:
			{
				if (relSrcAddr & 3)
					die("Unaligned relocation!");

				// For some reason this can happen, and we definitely don't
				// want relative relocations to be processed more than once
				// since we convert them to absolute addresses.
				if (HasReloc(relSrcAddr, relRelocMap))
					break;

				// PREL31 relocations sign-extend to 32-bit offsets
				if (relType == R_ARM_PREL31)
				{
					// "If bit 31 is one: this is a table entry itself (ARM_EXIDX_COMPACT)"
					if (relSrc & BIT(31))
						break;

					relSrc &= ~BIT(31);
					if (relSrc & BIT(30))
						relSrc |= BIT(31);
				}

				int relocOff = (int)relSrc - ((int)relSymAddr - (int)relSrcAddr);

				relSymAddr += relocOff;
				if (relSymAddr >= topAddr)
				{
					printf("%d relSymAddr=%08X relSrcAddr=%08X topAddr=%08X\n", relocOff, relSymAddr, relSrcAddr, topAddr);
					die("Relocation to invalid address!");
				}

				if (
					((relSymAddr < rodataStart) && !(relSrcAddr < rodataStart)) ||
					((relSymAddr >= rodataStart && relSymAddr < dataStart) && !(relSrcAddr >= rodataStart && relSrcAddr < dataStart)) ||
					((relSymAddr >= dataStart   && relSymAddr < topAddr)   && !(relSrcAddr >= dataStart   && relSrcAddr < topAddr))
					)
				{
					relSrc = relSymAddr - baseAddr; // Convert to absolute address
					SetReloc(relSrcAddr, relRelocMap); // Add relocation
				}

				break;
			}
		}
	}
	return 0;
}
*/

// extern GElf_Sym         *gelf_getsym __P((Elf_Data *__src, int __ndx, GElf_Sym *__dst));
//extern Elf32_Shdr *elf32_getshdr __P((Elf_Scn *__scn));
//extern int elf_getshdrnum __P((Elf *__elf, size_t *__resultp));
//extern Elf_Scn *elf_getscn __P((Elf *__elf, size_t __index));

	return 0;




	fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC);
	if(fd == -1) {
		fprintf(stderr, "%s: Error opening output file \'%s\' - %s\n", program_name, output_filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Write magic number. */
	write32(fd, 0x58534433);
	/* Write header size. */
	write16(fd, sizeof(CTRX_Header));
	/* Write size of relocation. */
	write16(fd, sizeof(CTRX_Relocation));
	/* Write version. */
	write32(fd, 0x00);
	/* Write flags. */
	write32(fd, 0x00);
	/* Write size of code segment. */
	write32(fd, phdr[0].p_memsz);
	/* Write size of rodata segment. */
	write32(fd, phdr[1].p_memsz);
	/* Write size of data segment. */
	write32(fd, phdr[2].p_memsz);
	/* Write size of BSS segment */
	write32(fd, bss_size);

	return 0;
}

