#pragma once

#include "Vector3D.h"

#include <vector>
#include <memory>

class Arc;
class Node;
class RailedVehicle;

class Graph final
{
public:

	Graph(Graph const& a_graph) = delete;
	auto operator= (Graph const& a_graph) -> Graph& = delete;

	Graph(Graph&& a_graph) = delete;
	auto operator= (Graph&& a_graph) -> Graph & = delete;

	Graph();

	~Graph();

	auto createArc(std::vector<Vector3D>&& a_vertices) -> Arc*;

	auto createNode() -> Node*;

private:
	std::vector<std::unique_ptr<Arc>> arcs;

	std::vector<std::unique_ptr<Node>> nodes;
};