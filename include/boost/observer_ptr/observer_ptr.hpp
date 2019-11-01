#ifndef BOOST_OBSERVER_PTR_HPP_INCLUDED
#define BOOST_OBSERVER_PTR_HPP_INCLUDED

//          Copyright Bernhard Manfred Gruber & Michael Florian Hava.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/observer_ptr/ for documentation.

#pragma once

#include <boost/assert.hpp>
#include <functional>
#include <type_traits>

namespace boost {
	template <typename T>
	class observer_ptr {
		using pointer = typename std::add_pointer<T>::type;
		using reference = typename std::add_lvalue_reference<T>::type;
	public:
		using element_type = T;

		constexpr observer_ptr() noexcept =default;

		constexpr observer_ptr(std::nullptr_t) noexcept {}

		constexpr explicit observer_ptr(pointer other) noexcept : ptr(other) {}

		template <typename T2, typename = typename std::enable_if<std::is_convertible<T2, T>::value>::type>
		constexpr observer_ptr(observer_ptr<T2> other) noexcept : ptr(other.get()) {}

		constexpr pointer get() const noexcept {
			return ptr;
		}

		BOOST_CXX14_CONSTEXPR reference operator*() const noexcept {
			BOOST_ASSERT(ptr != nullptr);
			return *get();
		}

		constexpr pointer operator->() const noexcept {
			return get();
		}

		constexpr explicit operator bool() const noexcept {
			return get() != nullptr;
		}

		constexpr explicit operator pointer() const noexcept {
			return get();
		}

		BOOST_CXX14_CONSTEXPR pointer release() noexcept {
			auto p = ptr;
			ptr = nullptr;
			return p;
		}

		BOOST_CXX14_CONSTEXPR void reset(pointer p = nullptr) noexcept {
			ptr = p;
		}

		BOOST_CXX14_CONSTEXPR void swap(observer_ptr& other) noexcept {
			using std::swap;
			swap(ptr, other.ptr);
		}

	private:
		T* ptr{nullptr};
	};

	template <typename T>
	BOOST_CXX14_CONSTEXPR void swap(observer_ptr<T>& p1, observer_ptr<T>& p2) noexcept {
		p1.swap(p2);
	}

	template <typename T>
	constexpr observer_ptr<T> make_observer(T* p) noexcept {
		return observer_ptr<T>{p};
	}

	template <typename T1, typename T2>
	constexpr bool operator==(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		return p1.get() == p2.get();
	}

	template <typename T1, typename T2>
	constexpr bool operator!=(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		return !(p1 == p2);
	}

	template <typename T>
	constexpr bool operator==(observer_ptr<T> p, std::nullptr_t) noexcept {
		return !p;
	}

	template <typename T>
	constexpr bool operator==(std::nullptr_t, observer_ptr<T> p) noexcept {
		return !p;
	}

	template <typename T>
	constexpr bool operator!=(observer_ptr<T> p, std::nullptr_t) noexcept {
		return static_cast<bool>(p);
	}

	template <typename T>
	constexpr bool operator!=(std::nullptr_t, observer_ptr<T> p) noexcept {
		return static_cast<bool>(p);
	}

	template <typename T1, typename T2>
	constexpr bool operator<(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		using T3 = typename std::common_type<
			typename std::add_pointer<T1>::type,
			typename std::add_pointer<T2>::type
		>::type;
		return std::less<T3>{}(p1.get(), p2.get());
	}

	template <typename T1, typename T2>
	constexpr bool operator>(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		return p2 < p1;
	}

	template <typename T1, typename T2>
	constexpr bool operator<=(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		return !(p2 < p1);
	}

	template <typename T1, typename T2>
	constexpr bool operator>=(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
		return !(p1 < p2);
	}

	template <typename T>
	struct hash<boost::observer_ptr<T>> {
		BOOST_CXX14_CONSTEXPR auto operator()(const boost::observer_ptr<T>& p) const noexcept -> std::size_t {
			return std::hash<T*>{}(p.get());
		}
	};
}

namespace std {
	template <typename T>
	struct hash<boost::observer_ptr<T>> {
		BOOST_CXX14_CONSTEXPR auto operator()(const boost::observer_ptr<T>& p) const noexcept -> std::size_t {
			return std::hash<T*>{}(p.get());
		}
	};
}

#endif // #ifndef BOOST_OBSERVER_PTR_HPP_INCLUDED
