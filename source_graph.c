#include <stdlib.h>
#include "source_graph.h"

source_graph* sg_create(unsigned int size)
{
	source_graph *sg;

	sg = malloc(sizeof(source_graph));
	sg->node_num = size;
	sg->nodes = calloc(sizeof(source_graph_node*), size);

	return sg;
}

source_graph_node* sg_add_node(source_graph* sg, zend_uint position)
{
	source_graph_node* node;
	node = malloc(sizeof(source_graph_node));
	node->position = position;
	node->edge_num = 0;
	node->edges = NULL;

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
	source_graph_node *src = sg_get_node(sg, src_pos);
	source_graph_node *dest = sg_get_node(sg, dest_pos);

	new_edges = realloc(src->edges, sizeof(source_graph_node*) * src->edge_num + 1);
	if (NULL == new_edges) {
		abort();
	}

	new_edges[src->edge_num] = dest;
	src->edges = new_edges;
	++src->edge_num;
}

void sg_destroy(source_graph *sg)
{
	int i;

	for (i=0; i < sg->node_num; ++i) {
		if (sg->nodes[i]->edges) {
			free(sg->nodes[i]->edges);
		}

		free(sg->nodes[i]);
	}

	free(sg);
}

void sg_show(source_graph *sg) {
	int i,j;
	for (i = 0; i < sg->node_num; ++i) {
		source_graph_node* node = sg->nodes[i];
		php_printf("%d ", node->position);
		if (node->edge_num) {
			for (j = 0; j < node->edge_num; ++j) {
				php_printf("-> %d ", node->edges[j]->position);
			}
		}
		php_printf("\n");
	}

}


