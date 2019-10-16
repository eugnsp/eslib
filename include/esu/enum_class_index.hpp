#pragma once

#define ES_UTIL_ENUM_CLASS_INDEX_TYPE(Index_type, Index_name, ...)                                                     \
	struct Index_name##_wrapper                                                                                        \
	{                                                                                                                  \
		enum class Index_name : Index_type                                                                             \
		{                                                                                                              \
			__VA_ARGS__                                                                                                \
		};                                                                                                             \
                                                                                                                       \
		friend constexpr Index_type operator*(Index_name index)                                                        \
		{                                                                                                              \
			return static_cast<Index_type>(index);                                                                     \
		}                                                                                                              \
                                                                                                                       \
		template<typename N>                                                                                           \
		friend constexpr Index_name& operator+=(Index_name& index, N n)                                                \
		{                                                                                                              \
			index = static_cast<Index_name>(*index + n);                                                               \
			return index;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		template<typename N>                                                                                           \
		friend constexpr Index_name& operator-=(Index_name& index, N n)                                                \
		{                                                                                                              \
			index = static_cast<Index_name>(*index - n);                                                               \
			return index;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		template<typename N>                                                                                           \
		friend constexpr Index_name operator+(Index_name index, N n)                                                   \
		{                                                                                                              \
			return index += n;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		template<typename N>                                                                                           \
		friend constexpr Index_name operator-(Index_name index, N n)                                                   \
		{                                                                                                              \
			return index -= n;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		friend constexpr Index_name& operator++(Index_name& index)                                                     \
		{                                                                                                              \
			return index += 1;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		friend constexpr Index_name operator++(Index_name& index, int)                                                 \
		{                                                                                                              \
			const auto old = index;                                                                                    \
			++index;                                                                                                   \
			return old;                                                                                                \
		}                                                                                                              \
                                                                                                                       \
		friend constexpr Index_name& operator--(Index_name& index)                                                     \
		{                                                                                                              \
			return index -= 1;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		friend constexpr Index_name operator--(Index_name& index, int)                                                 \
		{                                                                                                              \
			const auto old = index;                                                                                    \
			++index;                                                                                                   \
			return old;                                                                                                \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	using Index_name = Index_name##_wrapper::Index_name;
