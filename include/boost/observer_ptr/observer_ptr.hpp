#ifndef BOOST_OBSERVER_PTR_HPP_INCLUDED
#define BOOST_OBSERVER_PTR_HPP_INCLUDED

//          Copyright Bernhard Manfred Gruber & Michael Florian Hava.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/observer_ptr/ for documentation. TODO

#pragma once

#include <boost/assert.hpp>
#include <functional>
#include <type_traits>

namespace boost {
	//  TODO: documentation
	template <typename W>
	class observer_ptr {
		using pointer = typename std::add_pointer<W>::type;
		using reference = typename std::add_lvalue_reference<W>::type;
	public:
		using element_type = W;

		constexpr observer_ptr() noexcept =default;

		constexpr observer_ptr(std::nullptr_t) noexcept {}

		constexpr explicit observer_ptr(pointer other) noexcept : ptr(other) {}

		template <typename W2, typename = typename std::enable_if<std::is_convertible<W2, W>::value>::type>
		constexpr observer_ptr(observer_ptr<W2> other) noexcept : ptr(other.get()) {}

		constexpr pointer get() const noexcept {
			return ptr;
		}

		BOOST_CXX14_CONSTEXPR reference operator*() const {//TODO noexcept here? AFAIK there is an exception to the Lakos Rule in the standard libraries smart pointers... (at least shared_ptr does)
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
		W* ptr{nullptr};
	};

	template <typename W>
	BOOST_CXX14_CONSTEXPR void swap(observer_ptr<W>& p1, observer_ptr<W>& p2) noexcept {
		p1.swap(p2);
	}

	template <typename W>
	constexpr observer_ptr<W> make_observer(W* p) noexcept {
		return observer_ptr<W>{p};
	}

	template <typename W1, typename W2>
	constexpr bool operator==(observer_ptr<W1> p1, observer_ptr<W2> p2) {
		return p1.get() == p2.get();
	}

	template <typename W1, typename W2>
	constexpr bool operator!=(observer_ptr<W1> p1, observer_ptr<W2> p2) {
		return !(p1 == p2);
	}

	template <typename W>
	constexpr bool operator==(observer_ptr<W> p, std::nullptr_t) noexcept {
		return !p;
	}

	template <typename W>
	constexpr bool operator==(std::nullptr_t, observer_ptr<W> p) noexcept {
		return !p;
	}

	template <typename W>
	constexpr bool operator!=(observer_ptr<W> p, std::nullptr_t) noexcept {
		return static_cast<bool>(p);
	}

	template <typename W>
	constexpr bool operator!=(std::nullptr_t, observer_ptr<W> p) noexcept {
		return static_cast<bool>(p);
	}

	template <typename W1, typename W2>
	constexpr bool operator<(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept {
		// TODO: where W3 is the composite pointer type (C++17 §8) of W1* and W2*.
		// from N4820:
		//4 The composite pointer type of two operands p1 and p2 having types T1 and T2, respectively, where at least one is a pointer or pointer-to-member type or std::std::nullptr_t, is:
		//(4.1) - if both p1 and p2 are null pointer constants, std::std::nullptr_t;
		//(4.2) - if either p1 or p2 is a null pointer constant, T2 or T1, respectively;
		//(4.3) - if T1 or T2 is "pointer to cv1 void" and the other type is "pointer to cv2 T", where T is an object type or void, "pointer to cv12 void", where cv12 is the union of cv1 and cv2;
		//(4.4) - if T1 or T2 is "pointer to noexcept function" and the other type is "pointer to function", where the function types are otherwise the same, "pointer to function";
		//(4.5) - if T1 is "pointer to cv1 C1" and T2 is "pointer to cv2 C2", where C1 is reference-related to C2 or C2 is reference-related to C1(9.3.3), the cv-combined type of T1 and T2 or the cv-combined type of T2 and T1, respectively;
		//(4.6) - if T1 or T2 is "pointer to member of C1 of type function", the other type is "pointer to member of C2 of type noexcept function", and C1 is reference-related to C1 or C2 is reference-related to C1(9.3.3), where the function types are otherwise the same, "pointer to member of C2 of type function" or "pointer to member of C1 of type function", respectively;
		//(4.7) - if T1 is "pointer to member of C1 of type cv1 U" and T2 is "pointer to member of C2 of type cv2 U", for some non-function type U, where C1 is reference-related to C2 or C2 is reference-related to C1(9.3.3), the cv-combined type of T2 and T1 or the cv-combined type of T1 and T2, respectively;
		//(4.8) - if T1 and T2 are similar types(7.3.5), the cv-combined type of T1 and T2;
		//(4.9) - otherwise, a program that necessitates the determination of a composite pointer type is ill-formed.

		using W3 = typename std::common_type<
			typename std::add_pointer<W1>::type,
			typename std::add_pointer<W2>::type
		>::type;
		return std::less<W3>{}(p1.get(), p2.get());
	}

	template <typename W1, typename W2>
	constexpr bool operator>(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept {
		return p2 < p1;
	}

	template <typename W1, typename W2>
	constexpr bool operator<=(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept {
		return !(p2 < p1);
	}

	template <typename W1, typename W2>
	constexpr bool operator>=(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept {
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
