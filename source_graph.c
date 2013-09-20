#include <stdlib.h>
#include "source_graph.h"
#include "strict_op.h"
#include "source_file.h"

source_graph* sg_create(unsigned int size)
{
	source_graph *sg;
	int i;

	sg = malloc(sizeof(source_graph));
	sg->node_num = size;
	sg->nodes = calloc(sizeof(source_graph_node*), size);
	for (i=0;i<size;++i) {
		sg_add_node(sg, i);
	}

	return sg;
}

source_graph_node* sg_add_node(source_graph* sg, zend_uint position)
{
	source_graph_node* node;
	node = malloc(sizeof(source_graph_node));
	node->position = position;
	node->in = 0;
	node->in_edges = NULL;
	node->out = 0;
	node->out_edges = NULL;

	sg->nodes[position] = node;
	return node;
}

source_graph_node* sg_get_node(source_graph* sg, zend_uint position)
{
	return sg->nodes[position];
}

void sg_assosiate(source_graph* sg, zend_uint src_pos, zend_uint dest_pos)
{
	source_graph_node **new_edges;
	source_graph_node *src;
	source_graph_node *dest;

	if (sg->node_num <= src_pos || sg->node_num <= dest_pos) {
		return;
	}

	src = sg_get_node(sg, src_pos);
	dest = sg_get_node(sg, dest_pos);

	new_edges = realloc(src->out_edges, sizeof(source_graph_node*) * (src->out + 1));
	if (NULL == new_edges) {
		abort();
	}

	new_edges[src->out++] = dest;
	src->out_edges = new_edges;


	new_edges = realloc(dest->in_edges, sizeof(source_graph_node*) * (dest->in + 1));
	if (NULL == new_edges) {
		abort();
	}

	new_edges[dest->in++] = src;
	dest->in_edges = new_edges;
}

void sg_destroy(source_graph *sg)
{
	int i;

	for (i=0; i < sg->node_num; ++i) {
		source_graph_node *node = sg->nodes[i];
		if (node->out_edges) {
			free(node->out_edges);
		}
		if (node->in_edges) {
			free(node->in_edges);
		}

		free(node);
	}

	free(sg);
}

void sg_show_ex(source_graph *sg, zend_op_array *opa, void print(const char* format, ...)) {
	int i,j, before = 0;
	char buf[100];

	for (i = 0; i < sg->node_num; ++i) {
		source_graph_node* node = sg->nodes[i];

		const zend_op op = opa->opcodes[node->position];
		print("%3d %-18s", node->position, get_opcode_name(op.opcode));
		if (before != op.lineno) {
			print("%3d %-30s",
				op.lineno,
				get_source_line(opa, node->position)
			);
			before = op.lineno;
		} else {
			print("%34s","");
		}

		buf[0] = 0;
		for (j = 0; j < node->in; ++j) {
			char str[16];
			snprintf(str, 16, "%s%d", (j==0?"":","), node->in_edges[j]->position);
			strcat(buf, str);
		}

		print("%10s -> %3d -> ", buf, node->position);

		buf[0] = 0;
		for (j = 0; j < node->out; ++j) {
			char str[16];
			snprintf(str, 16, "%s%d", (j==0?"":","), node->out_edges[j]->position);
			strcat(buf, str);
		}

		print("%s\n", buf);
	}
}


void sg_show(source_graph *sg, void print(const char* format, ...)) {

	int i,j;
	for (i = 0; i < sg->node_num; ++i) {
		source_graph_node* node = sg->nodes[i];

		print("%3d ", node->position);
		for (j = 0; j < node->out; ++j) {
			print("%s%d", (j==0?"-> ":","), node->out_edges[j]->position);
		}
		print("    ");
		for (j = 0; j < node->in; ++j) {
			print("%s%d", (j==0?"<- ":","), node->in_edges[j]->position);
		}
		print("\n");
	}
}
