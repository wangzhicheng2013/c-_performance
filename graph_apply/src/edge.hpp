#ifndef SRC_EDGE_HPP_
#define SRC_EDGE_HPP_
#include "vertex.hpp"
struct edge {
	edge() {
		vertex_index0 = 0;
		vertex_index1 = 0;
		weight = 0;
	}
	unsigned vertex_index0;
	unsigned vertex_index1;
	double weight;
};

#endif /* SRC_EDGE_HPP_ */
