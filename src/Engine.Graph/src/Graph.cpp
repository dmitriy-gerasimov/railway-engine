#include "Graph.h"

#include "Arc.h"
#include "Node.h"

Graph::Graph() = default;

Graph::~Graph() = default;

auto Graph::createArc(std::vector<Vector3D>&& a_vertices) -> Arc*
{
	this->arcs.push_back(std::make_unique<Arc>(std::move(a_vertices)));

	return this->arcs.back().get();
}

auto Graph::createNode() -> Node*
{
	this->nodes.push_back(std::make_unique<Node>());

	return this->nodes.back().get();
}