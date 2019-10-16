#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <memory>
#include <queue>
#include <tuple>
#include <vector>

namespace esf::internal
{
template<class T>
class Quad_tree
{
public:
	enum class Neighbour_type
	{
		NORTH,
		SOUTH,
		WEST,
		EAST
	};

	class Cell
	{
	public:
		using Children = std::array<Cell, 4>;

	public:
		explicit Cell(const Cell* parent = nullptr) : parent_(parent)
		{}

		bool is_in_root() const
		{
			return !parent_;
		}

		bool is_leaf() const
		{
			return !children_;
		}

		const Cell& parent() const
		{
			assert(parent_);
			return *parent_;
		}

		std::size_t depth() const
		{
			return is_in_root() ? 0U : 1U + parent_->depth();
		}

		/** Children */
		const Children& children() const
		{
			assert(!is_leaf());
			return *children_;
		}

		Children& children()
		{
			assert(!is_leaf());
			return *children_;
		}

		Cell& child(std::size_t index) const
		{
			// Parent is not responsible for its children, so return a non-const reference
			assert(index < 4 && !is_leaf());
			return (*children_)[index];
		}

		Cell& ne_child() const
		{
			assert(!is_leaf());
			return (*children_)[0];
		}

		Cell& nw_child() const
		{
			assert(!is_leaf());
			return (*children_)[1];
		}

		Cell& sw_child() const
		{
			assert(!is_leaf());
			return (*children_)[2];
		}

		Cell& se_child() const
		{
			assert(!is_leaf());
			return (*children_)[3];
		}

		bool is_ne() const
		{
			assert(!is_in_root());
			return this == &parent_->ne_child();
		}
		bool is_nw() const
		{
			assert(!is_in_root());
			return this == &parent_->nw_child();
		}
		bool is_sw() const
		{
			assert(!is_in_root());
			return this == &parent_->sw_child();
		}
		bool is_se() const
		{
			assert(!is_in_root());
			return this == &parent_->se_child();
		}

		void create_children()
		{
			assert(is_leaf());
			children_ = std::unique_ptr<Children>(
				new Children{Cell(this), Cell(this), Cell(this), Cell(this)});
		}

		/** Data */
		const T& data() const
		{
			return data_;
		}

		T& data()
		{
			return data_;
		}

	private:
		const Cell* const parent_;
		std::unique_ptr<Children> children_;
		T data_;
	};

	struct Neighbouring_cell
	{
		Cell* cell;
		bool is_larger;

		explicit operator bool() const
		{
			return cell != nullptr;
		}

		const Cell* operator->() const
		{
			return cell;
		}

		Cell* operator->()
		{
			return cell;
		}
	};

public:
	Quad_tree(std::size_t n_cells_x, std::size_t n_cells_y) :
		n_cells_x_(n_cells_x), n_cells_y_(n_cells_y)
	{
		assert(n_cells_x > 0 && n_cells_y > 0);
		forest_.resize(n_cells_x * n_cells_y);
	}

	std::size_t n_root_cells() const
	{
		return n_cells_x_ * n_cells_y_;
	}

	std::size_t n_root_cells_x() const
	{
		return n_cells_x_;
	}

	std::size_t n_root_cells_y() const
	{
		return n_cells_y_;
	}

	Cell& root_cell(std::size_t nx, std::size_t ny)
	{
		return forest_[nx + ny * n_cells_x_];
	}

	Cell& root_cell(std::size_t index)
	{
		return forest_[index];
	}

	Neighbouring_cell neighbour_cell(const Cell& cell, Neighbour_type type)
	{
		return cell.is_in_root() ? neighbour_cell_in_root(cell, type)
								 : neighbour_cell_in_forest(cell, type);
	}

	// Returns the neighbouring cell if it has the same depth
	// or nullptr otherwise
	Cell* neighbour_cell_same_depth(const Cell& cell, Neighbour_type type)
	{
		const auto neighbour = neighbour_cell(cell, type);
		return neighbour.is_larger ? nullptr : neighbour.cell;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Iterators */

	// Traverses the tree starting from the cell (parent)
	// and calls (Func) for each leaf
	template<class Func>
	void for_each_leaf(Cell& parent, Func func)
	{
		if (parent.is_leaf())
			func(parent);
		else
			for (auto& child : parent.children())
				for_each_leaf(child, func);
	}

	// Traverses the tree and calls (Func) for each leaf
	template<class Func>
	void for_each_leaf(Func func)
	{
		for (auto& child : forest_)
			for_each_leaf(child, func);
	}

	// Traverses the tree in the preorder staring from the cell (parent)
	// and calls (Func) for each cell
	template<class Func>
	void traverse_preorder(Cell& parent, Func func)
	{
		if (parent.is_leaf())
			func(parent);
		else
		{
			func(parent);
			for (auto& child : parent.children())
				traverse_preorder(child, func);
		}
	}

	// Traverses the tree in the preorder and calls (Func) for each cell
	template<class Func>
	void traverse_preorder(Func func)
	{
		for (auto& child : forest_)
			traverse_preorder(func, child);
	}

	// Rebalances the tree so that no two adjacent cells have depth difference
	// larger than one;/ for each splitted cell a function func(const Cell&) is called
	template<class Func>
	void rebalance(Func func)
	{
		std::queue<Cell*> splitting_candidates;
		for_each_leaf([&splitting_candidates](auto& cell) { splitting_candidates.push(&cell); });

		while (!splitting_candidates.empty())
		{
			auto& cell = *splitting_candidates.front();
			splitting_candidates.pop();

			// Note: the cell can cease to be a leaf because it
			// was in the stack and has already been split,
			// (is_to_be_split) will check whether the cell is a leaf

			if (!is_to_be_split(cell))
				continue;

			cell.create_children();
			for (std::size_t i = 0; i < 4; ++i)
				splitting_candidates.push_front(&cell.child(i));

			for (auto n : neighbours)
			{
				auto neighbour = neighbour_cell(cell, n);
				if (neighbour.is_larger)
					splitting_candidates.push_front(neighbour.cell);
			}

			func(cell);
		}
	}

	static constexpr std::array<Neighbour_type, 4> neighbours = {
		Neighbour_type::NORTH, Neighbour_type::SOUTH, Neighbour_type::WEST, Neighbour_type::EAST};

private:
	Neighbouring_cell neighbour_cell_in_root(const Cell& cell, Neighbour_type type)
	{
		assert(cell.is_in_root());

		const auto it = std::find_if(
			forest_.begin(), forest_.end(), [&cell](const Cell& c) { return &c == &cell; });

		assert(it != forest_.end());
		const auto pos = static_cast<std::size_t>(it - forest_.begin());

		// TODO : single function call
		auto j = pos / n_cells_x_;
		auto i = pos % n_cells_x_;

		assert(i < n_cells_x_ && j < n_cells_y_);

		switch (type)
		{
		case Neighbour_type::NORTH:
			if (++j == n_cells_y_)
				return {nullptr, false};
			break;

		case Neighbour_type::SOUTH:
			if (j-- == 0)
				return {nullptr, false};
			break;

		case Neighbour_type::WEST:
			if (i-- == 0)
				return {nullptr, false};
			break;

		case Neighbour_type::EAST:
			if (++i == n_cells_x_)
				return {nullptr, false};
		}

		return {&forest_[i + j * n_cells_x_], false};
	}

	Neighbouring_cell neighbour_cell_in_forest(const Cell& cell, Neighbour_type type)
	{
		assert(!cell.is_in_root());

		// When the neighbour is our sibling
		switch (type)
		{
		case Neighbour_type::NORTH:
			if (cell.is_sw())
				return {&cell.parent().nw_child(), false};
			if (cell.is_se())
				return {&cell.parent().ne_child(), false};
			break;

		case Neighbour_type::SOUTH:
			if (cell.is_nw())
				return {&cell.parent().sw_child(), false};
			if (cell.is_ne())
				return {&cell.parent().se_child(), false};
			break;

		case Neighbour_type::WEST:
			if (cell.is_ne())
				return {&cell.parent().nw_child(), false};
			if (cell.is_se())
				return {&cell.parent().sw_child(), false};
			break;

		case Neighbour_type::EAST:
			if (cell.is_nw())
				return {&cell.parent().ne_child(), false};
			if (cell.is_sw())
				return {&cell.parent().se_child(), false};
			break;
		}

		// Otherwise query the parent
		const auto neighbour = neighbour_cell(cell.parent(), type);
		if (!neighbour || neighbour->is_leaf())
			return {neighbour.cell, !!neighbour};

		switch (type)
		{
		case Neighbour_type::NORTH:
			return {cell.is_nw() ? &neighbour->sw_child() : &neighbour->se_child(), false};

		case Neighbour_type::SOUTH:
			return {cell.is_sw() ? &neighbour->nw_child() : &neighbour->ne_child(), false};

		case Neighbour_type::WEST:
			return {cell.is_nw() ? &neighbour->ne_child() : &neighbour->se_child(), false};

		default: // case NeighbourType::EAST:
			return {cell.is_ne() ? &neighbour->nw_child() : &neighbour->sw_child(), false};
		}
	}

	bool is_to_be_split(const Cell& cell, Neighbour_type type) const
	{
		const auto neighbour = const_cast<Quad_tree*>(this)->neighbour_cell(cell, type); // Safe
		if (!neighbour || neighbour->is_leaf())
			return false;

		switch (type)
		{
		case Neighbour_type::NORTH:
			return !neighbour->sw_child().is_leaf() || !neighbour->se_child().is_leaf();

		case Neighbour_type::SOUTH:
			return !neighbour->nw_child().is_leaf() || !neighbour->ne_child().is_leaf();

		case Neighbour_type::WEST:
			return !neighbour->ne_child().is_leaf() || !neighbour->se_child().is_leaf();

		default: // case NeighbourType::EAST:
			return !neighbour->nw_child().is_leaf() || !neighbour->sw_child().is_leaf();
		}
	}

	bool is_to_be_split(const Cell& cell) const
	{
		return !cell.is_leaf() && (is_to_be_split(cell, Neighbour_type::NORTH) ||
									  is_to_be_split(cell, Neighbour_type::SOUTH) ||
									  is_to_be_split(cell, Neighbour_type::WEST) ||
									  is_to_be_split(cell, Neighbour_type::EAST));
	}

private:
	std::vector<Cell> forest_;
	// esl::Matrix<Cell, la::dynamic, la::dynamic> forest_;

	const std::size_t n_cells_x_;
	const std::size_t n_cells_y_;
};
} // namespace esf::internal
