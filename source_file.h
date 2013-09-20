#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

void prepare_source_file();
void cleanup_source_file();
const char *get_source_line(zend_op_array *opa, int position);

#endif
