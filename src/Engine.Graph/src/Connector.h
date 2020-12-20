#pragma once

class Arc;
class Node;

class Connector final
{
public:
	Connector(Connector const& a_connector) = delete;
	auto operator= (Connector const& a_connector) -> Connector& = delete;

	Connector(Connector&& a_connector) = delete;
	auto operator= (Connector&& a_connector) -> Connector& = delete;

	Connector(Arc* a_arc);

	~Connector();

	auto getArc() const -> Arc const*;

	auto getNode() const -> Node*;

	auto getAnother() const -> Connector const*;

	auto isBegin() const -> bool;

	auto isEnd() const -> bool;

	auto getLinkedConnector() const -> Connector const*;

private:
	Arc* arc;

	Node* node;
};