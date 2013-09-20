#include "php.h"
#include "source_file.h"

#define MAX_COLUMN 30

typedef struct _source_file {
	const char *filename;
	int line_max;
	int line_alloc;
	char **line;
} source_file;

static source_file *sf;

static int is_opend_file(const char *filename) {
	return (sf->filename != NULL && strcmp(filename, sf->filename) == 0);
}

static void clear_source_file(const char *filename) {
	int i;

	for (i=0;i<sf->line_alloc;++i) {
		free(sf->line[i]);
	}

	free(sf->line);
	sf->line = NULL;

	sf->filename = filename;
	sf->line_alloc = 0;
	sf->line_max = 0;
}

void prepare_source_file() {
	sf = malloc(sizeof(source_file));
	sf->filename = NULL;
	sf->line_max = 0;
	sf->line_alloc = 16;

	sf->line = calloc(sizeof(char*), sf->line_alloc);
}

void cleanup_source_file() {
	clear_source_file(NULL);
	free(sf);
}

const char *get_source_line(zend_op_array *opa, int position) {
	FILE *fp;

	if (!is_opend_file(opa->filename)) {
		char buf[1025];
		int line_no = 0;

		clear_source_file(opa->filename);

		fp = fopen(sf->filename, "r");
		if (!fp) {
			return NULL;
		}

		while(fgets(buf, 1024, fp) != NULL) { // とりあえず、メンドイので 1行は1024byte 未満とする

			if (sf->line_alloc < line_no) {
				sf->line_alloc *= 2;
				sf->line = realloc(sizeof(char*), sf->line_alloc);
			}

			buf[1024] = 0; // safety
			buf[strlen(buf)-1] = 0; // remove \n
			sf->line[line_no] = calloc(sizeof(char), MAX_COLUMN+1);
			strncpy(sf->line[line_no], buf, MAX_COLUMN);
			++line_no;
		}

		fclose(fp);
	}

	if (opa->opcodes[position].lineno <= sf->line_max) {
		return sf->line[opa->opcodes[position].lineno-1];
	}
	return NULL;
}
