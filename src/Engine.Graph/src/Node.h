#pragma once

class Connector;

class Node final
{
public:
	Node(Node const& a_node) = delete;
	auto operator= (Node const& a_node) -> Node& = delete;

	Node(Node&& a_node) = delete;
	auto operator= (Node&& a_node)->Node & = delete;

	Node();

	~Node();

	auto getMainConnector() const -> Connector*;
	auto setMainConnector(Connector* a_connector) -> void;

	auto getLeftConnector() const->Connector*;
	auto setLeftConnector(Connector* a_connector) -> void;

	auto getRightConnector() const->Connector*;
	auto setRightConnector(Connector* a_connector) -> void;

	auto getIsSwitchedToLeft() const -> bool;
	auto setIsSwitchedToLeft(bool a_value) -> void;

	auto getLinkedConnector(Connector const* a_connector, bool a_shouldCut = false) const -> Connector const*;

	auto switchToOther() -> void;

private:
	Connector* main;

	Connector* left;

	Connector* right;

	bool isSwitchedToLeft;
};