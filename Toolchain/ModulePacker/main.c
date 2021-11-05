#include <argp.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "modulePacker.h"

// Parser elements
const char *argp_program_version = "x64BareBones ModulePacker (C) v0.2";
const char *argp_program_bug_address = "arq-catedra@googlegroups.com";

/* Program documentation. */
static char doc[] =
    "ModulePacker is an appender of binary files to be loaded all together";

/* A description of the arguments we accept. */
static char args_doc[] = "KernelFile Module1 Module2 ...";

/* The options we understand. */
static struct argp_option options[] = {
    {"output", 'o', "FILE", 0, "Output to FILE instead of standard output"},
    {0}};

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {

	struct arguments arguments;

	arguments.output_file = OUTPUT_FILE;
	arguments.count = 0;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	array_t fileArray = {arguments.args, arguments.count};

	if (!checkFiles(fileArray)) {
		return 1;
	}

	return !buildImage(fileArray, arguments.output_file);
}

int buildImage(array_t fileArray, char *output_file) {

	FILE *target;

	if ((target = fopen(output_file, "w")) == NULL) {
		printf("Can't create target file\n");
		return FALSE;
	}

	// First, write the kernel
	printf("Writing kernel. ");
	FILE *source = fopen(fileArray.array[0], "r");
	write_file(target, source);
	putchar('\n');

	// Write how many extra binaries we got.
	int extraBinaries = fileArray.length - 1;
	fwrite(&extraBinaries, sizeof(extraBinaries), 1, target);
	fclose(source);

	int i;
	for (i = 1; i < fileArray.length; i++) {
		FILE *source = fopen(fileArray.array[i], "r");

		// Write the file size;
		write_name(target, fileArray.array[i]);

		// Write the file size;
		write_size(target, fileArray.array[i]);

		printf("File: %s ", fileArray.array[i]);
		// Write the binary
		write_file(target, source);
		putchar('\n');

		fclose(source);
	}
	fclose(target);
	return TRUE;
}

int checkFiles(array_t fileArray) {

	int i = 0;
	for (; i < fileArray.length; i++) {
		if (access(fileArray.array[i], R_OK)) {
			printf("Can't open file: %s\n", fileArray.array[i]);
			return FALSE;
		}
	}
	return TRUE;
}

void write_name(FILE *target, char *filename) {
	char *base = malloc(32);
	strcpy(base, filename);
	base = basename(base);
	int len = strlen(base);
	base[len - 4] = '\0';
	printf("Name: %s ", base);
	fwrite(base, len - 3, 1, target);
}

int write_size(FILE *target, char *filename) {
	struct stat st;
	stat(filename, &st);
	uint32_t size = st.st_size;
	// printf("Size: %u ", size);
	fwrite(&size, sizeof(uint32_t), 1, target);
}

int write_file(FILE *target, FILE *source) {
	char buffer[BUFFER_SIZE];
	int read;
	uint64_t total = 0;

	while (!feof(source)) {
		read = fread(buffer, 1, BUFFER_SIZE, source);
		fwrite(buffer, 1, read, target);
		total += read;
	}
	printf("Copied: %u ", total);

	return TRUE;
}

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	/* Get the input argument from argp_parse, which we
	   know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
	case 'o':
		arguments->output_file = arg;
		break;

	case ARGP_KEY_ARG:
		arguments->args[state->arg_num] = arg;
		break;

	case ARGP_KEY_END:
		if (state->arg_num < 1)
			argp_usage(state);
		arguments->count = state->arg_num;
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
