#ifndef SRC_GRAPH_HPP_
#define SRC_GRAPH_HPP_
#include <vector>
#include <list>
#include "vertex.hpp"
#include "edge.hpp"
using namespace std;
class graph {
using node_type = pair<vertex, list<edge>>;
public:
	graph() {
		directed = false;
		vertex_num_ = 0;
		edge_num_ = 0;
	}
	~graph() = default;
public:
	inline void set_directed() {
		directed = true;
	}
	inline void add_vertex(vertex &v) {
		linked_list_.emplace_back(make_pair(v, list<edge>()));

	}
	inline void add_edge(const edge &e) {
		for (auto &node : linked_list_) {
			if (node.first.vertex_index == e.vertex_index0) {
				node.second.push_back(e);
				if (false == directed) {

				}
			}
		}
	}
private:
	bool directed;
	int vertex_num_;
	int edge_num_;
	vector<node_type>linked_list_;
};



#endif /* SRC_GRAPH_HPP_ */
