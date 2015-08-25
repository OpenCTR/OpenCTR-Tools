/*
 * OpenCTR-Tools - Tools used in the OpenCTR SDK.
 * 
 * Copyright (C) 2015 The OpenCTR Project. 
 * 
 * This file is part of OpenCTR-Tools. 
 * 
 * OpenCTR-Tools is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License version 3 as 
 * published by the Free Software Foundation.
 * 
 * OpenCTR-Tools is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with OpenCTR-Tools. If not, see <http://www.gnu.org/licenses/>. 
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

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYSSTAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
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

/* 
 * Structure to hold several ELF strings.
 */
typedef struct {
	char* strings;
	uint32_t nstrings;
} StringTable;

/* 
 * Add a new string.
 */
static int make_string(StringTable* table, const char* str) {
	int ret;
	const int len = strlen(str);

	table->strings = realloc(table->strings, table->nstrings + len + 1);
	strncpy(&table->strings[table->nstrings], str, len);
	ret = table->nstrings;
	table->nstrings += (len + 1);

	return ret;
}

/**
 * Create a new ELF section.
 */
static Elf_Scn* make_section(Elf* elf, StringTable* names, const char* name, GElf_Word type, GElf_Word flags) {
	Elf_Scn* scn = NULL;
	GElf_Shdr* shdr = NULL;
	int ret;

	scn = elf_newscn(elf);
	if(scn == NULL) {
		return NULL;
	}

	shdr = gelf_getshdr(scn, NULL);
	if(shdr == NULL) {
		return NULL;
	}

	shdr->sh_name = make_string(names, name);
	shdr->sh_type = type;
	shdr->sh_flags = flags;
	shdr->sh_addr = 0;
	shdr->sh_link = SHN_UNDEF;
	shdr->sh_info = 0;
	shdr->sh_entsize = 0;

	ret = gelf_update_shdr(scn, shdr);
	if(ret == -1) {
		return NULL;
	}

	return scn;
}

/*
 * Free any allocated resources.
 */
static void cleanup(Elf* elf, int ifd, int ofd, char* name, char* buffer, char* symnames, char* secnames) {
	int ret;

	/* Finish the ELF context. */
	if(elf) {
		elf_end(elf);
	}

	/* Close the input file. */
	if(ifd) {
		ret = close(ifd);
		if(ret == -1) {
			fprintf(stderr, "Error closing input file: %s\n", elf_errmsg(-1));
		}
	}

	/* Close the output file. */
	if(ofd) {
		ret = close(ofd);
		if(ret == -1) {
			fprintf(stderr, "Error closing output file: %s\n", elf_errmsg(-1));
		}
	}

	/* Free the symbol names string. */
	if(symnames) {
		free(symnames);
	}

	/* Free the section names string. */
	if(secnames) {
		free(secnames);
	}

	/* Free the symbol name string. */
	if(name) {
		free(name);
	}

	/* Free the input file buffer. */
	if(buffer) {
		free(buffer);
	}
}

int main(int argc, char *argv[]) {
	int i;
	int ifd = 0;
	int ofd = 0;
	Elf* elf = NULL;
	GElf_Ehdr* ehdr = NULL;
	Elf_Scn* data = NULL;
	Elf_Scn* symtab = NULL;
	Elf_Scn* strtab = NULL;
	Elf_Scn* shstrtab = NULL;
	Elf_Data* sd = NULL;
	StringTable symnames;
	uint32_t input_len = 0;
	struct stat st;
	int buffer_size;
	char* buffer = NULL;
	char* name = NULL;
	GElf_Shdr shdr;
	StringTable scnnames;
	Elf32_Sym symbols[3];
	int ret;
	char* symbol_name = NULL;
	char* symbol_size = NULL;
    const struct option options[] = {
        { "help", no_argument, NULL, 'h' },
        { "symbol-name",  required_argument, NULL, 's' },
        { NULL, 0, NULL, 0 }
    };
	uint8_t show_help = 0;
	int ch;

	/* TODO: Allow without getopt(). */

	/* Parse the command-line parameters. */
    while(1) {
        ch = getopt_long(argc, argv, "hs:", options, NULL);
        if(ch == -1) {
            break;
        }
        switch(ch) {
            case 'h':
                show_help = 1;
                break;
            case 's':
				name = strdup(optarg);
                break;
            default:
                break;
        }
    }

	/* Make sure enough parameters were given. */
    if((argc - optind) != 2) {
		fprintf(stderr, "Error: input and output files must be given\n");
        show_help = 1;
    }

	/* Print the help message.. */
	if(show_help == 1) {
		printf("%s %s - %s\n", PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_DESCRIPTION);
		printf("\n");
		printf("Usage:\n");
		printf("  %s [params] <input.ttf> <output.o>\n", argv[0]);
		printf("\n");
		printf("Parameters:\n");
		printf("  --help, -h                       Print this help information\n");
		printf("  --symbol-name=<NAME>, -s <NAME>  Use <NAME> and <NAME>_size as the symbol names\n");
		exit(EXIT_SUCCESS);
	}

    argc -= optind;
    argv += optind;

	/* Initialize symbols */
	memset(&symbols, 0x00, sizeof(symbols));

	/* Initialize section names. */
	scnnames.strings = malloc(sizeof(char));
	scnnames.strings[0] = '\0';
	scnnames.nstrings = 1;

	/* Initialize symbol names */
	symnames.strings = malloc(sizeof(char));
	symnames.strings[0] = '\0';
	symnames.nstrings = 1;

	/* If symbol name was not given by user, create it from filename. */
	if(name == NULL) {
		name = strdup(basename(argv[0]));
	}

	/* Replace any unusable characters in the symbol name. */
	for(i=0; i<strlen(name); i++) {
		if(name[i] == '.') {
			name[i] = '_';
		}
	}

	/* Initialize libelf */
	if(elf_version(EV_CURRENT) == EV_NONE) {
		fprintf(stderr, "Error initializing libelf: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Open the input file */
	ifd = open(argv[0], O_RDONLY);
	if(ifd == -1) {
		fprintf(stderr, "Error opening \"%s\" for reading: %s", argv[0], strerror(errno));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Find the input file size. */
	ret = fstat(ifd, &st);
	if(ret == -1) {
		fprintf(stderr, "Error using \"%s\" as input file: %s\n", argv[0], strerror(errno));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Allocate memory buffer for the input file. */
	buffer_size = st.st_size;
	buffer = (char*)malloc(sizeof(char*) * buffer_size);
	if(buffer == NULL) {
		fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Clear the memory buffer. */
	memset(buffer, 0x00, buffer_size);

	/* Read the input file into the memory buffer. */
	ret = read(ifd, buffer, buffer_size);
	if(ret != buffer_size) {
		fprintf(stderr, "Error reading \"%s\" as input file: %s\n", argv[0], strerror(errno));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Open the output file */
	ofd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(ofd == -1) {
		fprintf(stderr, "Error opening \"%s\" for writing: %s", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Create the output ELF file. */
	elf = elf_begin(ofd, ELF_C_WRITE, NULL);
	if(elf == NULL) {
		fprintf(stderr, "Error using \"%s\" as output ELF file: %s\n", argv[1], elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Create a new ELF header. */
	ehdr = (GElf_Ehdr*)gelf_newehdr(elf, ELFCLASS32);
	if(ehdr == NULL) {
		fprintf(stderr, "Error creating elf header: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* ELF header settings. */
	ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
	ehdr->e_type = ET_REL;
	ehdr->e_machine = EM_ARM;
	ehdr->e_version = EV_NONE;

	/* Create .rodata section. */
	data = make_section(elf, &scnnames, ".rodata", SHT_PROGBITS, SHF_ALLOC);
	if(data == NULL) {
		fprintf(stderr, "Error creating .rodata section: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Create .shstrtab section. */
	shstrtab = make_section(elf, &scnnames, ".shstrtab", SHT_STRTAB, 0);
	if(shstrtab == NULL) {
		fprintf(stderr, "Error creating .shstrtab section: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Create .symtab section. */
	symtab = make_section(elf, &scnnames, ".symtab", SHT_SYMTAB, 0);
	if(symtab == NULL) {
		fprintf(stderr, "Error creating .symtab section: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Create .strtab section. */
	strtab = make_section(elf, &scnnames, ".strtab", SHT_STRTAB, 0);
	if(strtab == NULL) {
		fprintf(stderr, "Error creating .strtab section: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Get .shstrtab section information. */
	ehdr->e_shstrndx = elf_ndxscn(shstrtab);
	if(gelf_getshdr(symtab, &shdr) == NULL) {
		fprintf(stderr, "Error finding .shstrtab section header: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Set .shstrtab section header contents. */
	shdr.sh_link = elf_ndxscn(strtab);
	shdr.sh_info = 1;

	/* Update the symbol table to include the .shstrtab header. */
	ret = gelf_update_shdr(symtab, &shdr);
	if(ret == -1) {
		fprintf(stderr, "Error updating symbol table: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	ret = gelf_update_ehdr(elf, ehdr);
	if(ret == -1) {
		fprintf(stderr, "Error updating the ELF header: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Create data segment for .shstrtab section. */
	sd = elf_newdata(shstrtab);
	if(sd == NULL) {
		fprintf(stderr, "Error creating data segment for .shstrtab: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}
	sd->d_buf = scnnames.strings;
	sd->d_type = ELF_T_BYTE;
	sd->d_size = scnnames.nstrings;
	sd->d_align = 1;

	/* Create data segment for .rodata section (symbol size). */
	sd = elf_newdata(data);
	if(sd == NULL) {
		fprintf(stderr, "Error creating data segment for .rodata: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}
	sd->d_buf = &input_len;
	sd->d_type = ELF_T_WORD;
	sd->d_size = sizeof(uint32_t);
	sd->d_align = 4;

	/* Create data segment for .rodata section (symbol location). */
	sd = elf_newdata(data);
	if(sd == NULL) {
		fprintf(stderr, "Error creating data segment 2 for .rodata: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}
	sd->d_buf = malloc(buffer_size);
	sd->d_size = buffer_size;
	sd->d_align = 1;
	memcpy(sd->d_buf, buffer, buffer_size);
	input_len += buffer_size;

	/* Create data segment for .symtab section. */
	sd = elf_newdata(symtab);
	if(sd == NULL) {
		fprintf(stderr, "Error creating data segment for .symtab: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}
	sd->d_buf = symbols;
	sd->d_type = ELF_T_SYM;
	sd->d_size = sizeof(symbols);
	sd->d_align = 4;

	/* Create symbol name buffers. */
	const int name_len = strlen(name) + 1;
	const int size_len = strlen(name) + strlen("_size") + 1;
	symbol_name = (char*)malloc(name_len);
	symbol_size = (char*)malloc(size_len);
	memset(symbol_name, 0x00, name_len);
	memset(symbol_size, 0x00, size_len);
	snprintf(symbol_name, name_len, "%s", name);
	snprintf(symbol_size, size_len, "%s_size", name);

	/* Create symbol for input file. */
	symbols[1].st_name = make_string(&symnames, symbol_name);
	symbols[1].st_size = input_len;
	symbols[1].st_info = ELF32_ST_INFO(STB_GLOBAL, STT_OBJECT);
	symbols[1].st_other = 0;
	symbols[1].st_shndx = elf_ndxscn(data);
	free(symbol_name);

	/* Create symbol for input file size. */
	symbols[2].st_name = make_string(&symnames, symbol_size);
	symbols[2].st_size = sizeof(uint32_t);
	symbols[2].st_info = ELF32_ST_INFO(STB_GLOBAL, STT_OBJECT);
	symbols[2].st_other = 0;
	symbols[2].st_shndx = elf_ndxscn(data);
	free(symbol_size);

	/* Update ELF file with new symbols. */
	ret = elf_update(elf, ELF_C_NULL);
	if(ret == -1) {
		fprintf(stderr, "Error updating ELF output file: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Get the first .rodata data segment. */
	sd = elf_getdata(data, NULL);
	if(sd == NULL) {
		fprintf(stderr, "Error finding .rodata data segment 1: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Point the filesize symbol to the location of the first .rodata segment. */
	symbols[2].st_value = sd->d_off;

	/* Get the second .rodata data segment. */
	sd = elf_getdata(data, sd);
	if(sd == NULL) {
		fprintf(stderr, "Error finding .rodata data segment 2: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Point the file symbol to the location of the second .rodata segment. */
	symbols[1].st_value = sd->d_off;

	/* Create data segment for .strtab section. */
	sd = elf_newdata(strtab);
	if(sd == NULL) {
		fprintf(stderr, "Error creating .strtab data segment: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* .strtab data segment values. */
	sd->d_buf = symnames.strings;
	sd->d_size = symnames.nstrings;
	sd->d_align = 1;

	/* Update ELF file with new symbol information. */
	ret = elf_update(elf, ELF_C_NULL);
	if(ret == -1) {
		fprintf(stderr, "Error updating ELF output file: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Write updated ELF file. */
	ret = elf_update(elf, ELF_C_WRITE);
	if(ret == -1) {
		fprintf(stderr, "Error updating ELF output file: %s\n", elf_errmsg(-1));
		cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);
		exit(EXIT_FAILURE);
	}

	/* Finished - cleanup any allocated resources. */
	cleanup(elf, ifd, ofd, name, buffer, symnames.strings, scnnames.strings);

	return 0;
}

