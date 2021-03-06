#ifndef SOURCE_GRAPH_H
#define SOURCE_GRAPH_H

#include "zend_API.h"

typedef struct _source_graph_node {
	zend_uint position;
	unsigned int in;
	struct _source_graph_node **in_edges;
	unsigned int out;
	struct _source_graph_node **out_edges;
} source_graph_node;

typedef struct _source_graph {
	source_graph_node *entry_point;
	unsigned int node_num;
	source_graph_node **nodes;
} source_graph;

source_graph* sg_create(unsigned int size);
source_graph_node* sg_add_node(source_graph* sg, zend_uint position);
source_graph_node* sg_get_node(source_graph* sg, zend_uint position);
void sg_assosiate(source_graph* sg, zend_uint src_pos, zend_uint dest_pos);
void sg_destroy(source_graph *sg);
void sg_show(source_graph *sg, void print(const char* format, ...));
void sg_show_ex(source_graph *sg, zend_op_array *opa, void print(const char* format, ...));

#endif	/* SOURCE_GRAPH_H */

