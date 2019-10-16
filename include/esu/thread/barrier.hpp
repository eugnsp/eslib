#pragma once
#include <esu/type_traits/functor.hpp>

#include <cassert>
#include <cstddef>
#include <condition_variable>
#include <mutex>
#include <type_traits>
#include <utility>

namespace esu
{
template<class Completion_fn = Nop_fn>
class Barrier
{
public:
	explicit Barrier(std::size_t n_threads, Completion_fn completion_fn = {}) :
		n_threads_(n_threads), completion_fn_(std::move(completion_fn))
	{
		assert(n_threads > 0);
	}

	void wait()
	{
		std::unique_lock lock(mutex_);
		const auto generation = generation_;
		if (++n_wait_ == n_threads_)
		{
			if constexpr (std::is_invocable_v<Completion_fn, std::size_t>)
				completion_fn_(generation_);
			else
				completion_fn_();

			++generation_;
			n_wait_ = 0;

			lock.unlock();
			cv_.notify_all();
		}
		else
			cv_.wait(lock, [this, generation] { return generation_ != generation; });
	}

private:
	const std::size_t n_threads_;
	std::size_t n_wait_ = 0;
	std::size_t generation_ = 0;

	std::mutex mutex_;
	std::condition_variable cv_;

	Completion_fn completion_fn_;
};
} // namespace esu
