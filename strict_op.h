#ifndef STRICT_OP_H
#define STRICT_OP_H

void strict_scan_op_array(zend_op_array *op_array TSRMLS_DC);
void strict_op_dump(zend_op_array* opa TSRMLS_DC);

typedef struct _strict_op_info {
	zend_uint use_flag;
	const char *name;
} strict_op_info;

const char *get_source_line(zend_op_array *opa, int position);
const char *get_opcode_name(zend_uchar opcode);

#endif
