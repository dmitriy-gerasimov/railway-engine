#include "Connector.h"

#include "Arc.h"
#include "Node.h"

Connector::Connector(Arc* a_arc)
	: arc(a_arc)
	, node(nullptr)
{
}

Connector::~Connector() = default;

auto Connector::getArc() const -> Arc const*
{
	return this->arc;
}

auto Connector::getNode() const -> Node*
{
	return this->node;
}

auto Connector::getAnother() const -> Connector const*
{
	return (isBegin() ? this->getArc()->getEnd() : this->getArc()->getBegin());
}

auto Connector::isBegin() const -> bool
{
	return (this == this->getArc()->getBegin());
}

auto Connector::isEnd() const -> bool
{
	return (this == this->getArc()->getEnd());
}

auto Connector::getLinkedConnector() const -> Connector const*
{
	if (this->node == nullptr)
	{
		return nullptr;
	}
	
	return this->node->getLinkedConnector(this);
}