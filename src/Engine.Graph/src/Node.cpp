#include "Node.h"

#include "Connector.h"

Node::Node()
	: main(nullptr)
	, left(nullptr)
	, right(nullptr)
	, isSwitchedToLeft(true)
{
}

Node::~Node() = default;

auto Node::getMainConnector() const -> Connector*
{
	return this->main;
}

auto Node::setMainConnector(Connector* a_connector) -> void
{
	this->main = a_connector;
}

auto Node::getLeftConnector() const -> Connector*
{
	return this->left;
}

auto Node::setLeftConnector(Connector* a_connector) -> void
{
	this->left = a_connector;
}

auto Node::getRightConnector() const -> Connector*
{
	return this->right;
}

auto Node::setRightConnector(Connector* a_connector) -> void
{
	this->right = a_connector;
}

auto Node::getIsSwitchedToLeft() const -> bool
{
	return this->isSwitchedToLeft;
}

auto Node::setIsSwitchedToLeft(bool a_value) -> void
{
	this->isSwitchedToLeft = a_value;
}

auto Node::getLinkedConnector(Connector const* a_connector, bool /*a_shouldCut*/) const -> Connector const*
{
	if (a_connector == this->main)
	{
		return (isSwitchedToLeft) ? this->left : this->right;
	}

	if (a_connector == this->left)
	{
		return (isSwitchedToLeft) ? this->main : nullptr;
	}

	if (a_connector == this->right)
	{
		return (isSwitchedToLeft) ? nullptr : this->main;
	}

	return nullptr;
}

auto Node::switchToOther() -> void
{
	isSwitchedToLeft = !isSwitchedToLeft;
}