/*
 * fileio.cpp
 *
 * TODO This file contains...
 */

#include "fileio.h"
#include <cstdio>
#include <cstdlib>

char *read_file(const char *filename)
{
	FILE *input;

	do
	{
		input = fopen(filename, "rb");
		if (input == NULL) break;

		if (fseek(input, 0, SEEK_END) == -1) break;
		long size = ftell(input);
		if (size == -1) break;
		if (fseek(input, 0, SEEK_SET) == -1) break;

		char *content = (char *)malloc((size_t)size + 1);
		if (content == NULL) break;

		fread(content, 1, (size_t)size, input);
		if (ferror(input))
		{
			free(content);
			break;
		}

		fclose(input);
		return content;
	}
	while (0); // Abuse break

	if (input != NULL) fclose(input);
	fprintf(stderr, "Unable to open file \"%s\".", filename);
	return NULL;
}
