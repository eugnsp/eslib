#pragma once
#include "iterator.hpp"

namespace esf
{

// 		class EdgeChainVertexIterator : public Internal::IteratorBase<2, Vertex_tag,
// std::forward_iterator_tag>
// 		{
// 		public:
// 			const ViewType& operator*() const
// 			{
// 				return view_;
// 			}
//
// 			const ViewType* operator->() const
// 			{
// 				return &view_;
// 			}
//
// 			void operator++()
// 			{
//
// 			}
//
// 			bool operator!=(const EdgeChainVertexIterator&)
// 			{
// 				return false;
// 			}
//
// 		private:
// 			ViewType view_;
// 		};

class Edge_chain_edge_iterator :
	public internal::Iterator_base<Edge_tag, Mesh2, std::forward_iterator_tag>
{
private:
	using Edge_list = std::vector<Index>;

public:
	Edge_chain_edge_iterator(
		const Mesh2& mesh, Edge_list::const_iterator it, Edge_list::const_iterator itEnd) :
		Iterator_base(mesh),
		it_(it), itEnd_(itEnd)
	{
		if (it != itEnd)
			set_view_index(*it);
	}

	Edge_chain_edge_iterator& operator++()
	{
		++it_;
		if (it_ != itEnd_)
			set_view_index(*it_);

		return *this;
	}

	bool operator==(const Edge_chain_edge_iterator& other) const
	{
		return it_ == other.it_;
	}

	bool operator!=(const Edge_chain_edge_iterator& other) const
	{
		return it_ != other.it_;
	}

private:
	Edge_list::const_iterator it_;
	const Edge_list::const_iterator itEnd_;
};

} // namespace esf
