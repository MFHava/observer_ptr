# `boost::observer_ptr<T>`

## Introduction

TODO

This library and documentation is largly taken from the Working Draft, C++ Extensions for Library Fundamentals, Version 3 (N4806](http://wg21.link/N4806).

A non-owning pointer, known as an observer, is an object o that stores a pointer to a second object, w.
In this context, w is known as a watched object.
[ Note: There is no watched object when the stored pointer is `nullptr`. — end note ]
An observer takes no responsibility or ownership of any kind for its watched object, if any;
in particular, there is no inherent relationship between the lifetimes of o and w.

Specializations of `observer_ptr` shall meet the requirements of a `CopyConstructible` and `CopyAssignable` type.
The template parameter W of an `observer_ptr` shall not be a reference type, but may be an incomplete type.

[ Note: The uses of `observer_ptr` include clarity of interface specification in new code, and interoperability with pointer-based legacy code. — end note ]


## Synopsis

```C++
namespace boost {
	template <typename W> class observer_ptr {
		using pointer = std::add_pointer_t<W>;            // exposition-only
		using reference = std::add_lvalue_reference_t<W>; // exposition-only
	public:
		// publish our template parameter and variations thereof
		using element_type = W;

		// default constructor
		constexpr observer_ptr() noexcept;

		// pointer-accepting constructors
		constexpr observer_ptr(nullptr_t) noexcept;
		constexpr explicit observer_ptr(pointer) noexcept;

		// copying constructors (in addition to the implicit copy constructor)
		template <typename W2>
		constexpr observer_ptr(observer_ptr<W2>) noexcept;

		// observers
		constexpr pointer get() const noexcept;
		constexpr reference operator*() const;
		constexpr pointer operator->() const noexcept;
		constexpr explicit operator bool() const noexcept;

		// conversions
		constexpr explicit operator pointer() const noexcept;

		// modifiers
		constexpr pointer release() noexcept;
		constexpr void reset(pointer = nullptr) noexcept;
		constexpr void swap(observer_ptr&) noexcept;
	}; // observer_ptr<>

	// observer_ptr specialized algorithms
	template <typename W>
	void swap(observer_ptr<W>&, observer_ptr<W>&) noexcept;

	template <typename W>
	observer_ptr<W> make_observer(W*) noexcept;

	// (in)equality operators
	template <typename W1, typename W2>
	bool operator==(observer_ptr<W1>, observer_ptr<W2>);
	template <typename W1, typename W2>
	bool operator!=(observer_ptr<W1>, observer_ptr<W2>);

	template <typename W>
	bool operator==(observer_ptr<W>, nullptr_t) noexcept;
	template <typename W>
	bool operator!=(observer_ptr<W>, nullptr_t) noexcept;
	template <typename W>
	bool operator==(nullptr_t, observer_ptr<W>) noexcept;
	template <typename W>
	bool operator!=(nullptr_t, observer_ptr<W>) noexcept;

	// ordering operators
	template <typename W1, typename W2>
	bool operator<(observer_ptr<W1>, observer_ptr<W2>);
	template <typename W1, typename W2>
	bool operator>(observer_ptr<W1>, observer_ptr<W2>);
	template <typename W1, typename W2>
	bool operator<=(observer_ptr<W1>, observer_ptr<W2>);
	template <typename W1, typename W2>
	bool operator>=(observer_ptr<W1>, observer_ptr<W2>);

	// observer_ptr boost::hash support
	template <typename T> struct hash<observer_ptr<T>>;
} // namespace boost

namespace std {
	// observer_ptr std::hash support
	template <typename T> struct hash<boost::observer_ptr<T>>;
} // namespace std
```

## constructors

```C++
constexpr observer_ptr() noexcept;
constexpr observer_ptr(nullptr_t) noexcept;
```

Effects: Constructs an observer_ptr object that has no corresponding watched object.

Postconditions: `get() == nullptr`.

```C++
constexpr explicit observer_ptr(pointer other) noexcept;
```

Postconditions: `get() == other`.

```C++
template <class W2> constexpr observer_ptr(observer_ptr<W2> other) noexcept;
```

Postconditions: `get() == other.get()`.

Remarks: This constructor shall not participate in overload resolution unless `W2*` is convertible to `W*`.

## observers

```C++
constexpr pointer get() const noexcept;
```

Returns: The stored pointer.

```C++
constexpr reference operator*() const;
```

Requires: `get() != nullptr`.

Returns: `*get()`.

Throws: Nothing.

```C++
constexpr pointer operator->() const noexcept;
```

Returns: `get()`.

```C++
constexpr explicit operator bool() const noexcept;
```

Returns: `get() != nullptr`.

## conversions

```C++
constexpr explicit operator pointer() const noexcept;
```

Returns: `get()`.


## modifiers

```C++
constexpr pointer release() noexcept;
```

Postconditions: `get() == nullptr`.

Returns: The value `get()` had at the start of the call to release.

```C++
constexpr void reset(pointer p = nullptr) noexcept;
```

Postconditions: `get() == p`.

```C++
constexpr void swap(observer_ptr& other) noexcept;
```

Effects: Invokes swap on the stored pointers of `*this` and other.


## specialized algorithms

```C++
template <class W>
void swap(observer_ptr<W>& p1, observer_ptr<W>& p2) noexcept;
```

Effects: `p1.swap(p2)`.

```C++
template <class W> observer_ptr<W> make_observer(W* p) noexcept;
```

Returns: `observer_ptr<W>{p}`.

```C++
template <class W1, class W2>
bool operator==(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `p1.get() == p2.get()`.

```C++
template <class W1, class W2>
bool operator!=(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `not (p1 == p2)`.

```C++
template <class W>
bool operator==(observer_ptr<W> p, nullptr_t) noexcept;
template <class W>
bool operator==(nullptr_t, observer_ptr<W> p) noexcept;
```

Returns: `not p`.

```C++
template <class W>
bool operator!=(observer_ptr<W> p, nullptr_t) noexcept;
template <class W>
bool operator!=(nullptr_t, observer_ptr<W> p) noexcept;
```

Returns: `(bool)p`.

```C++
template <class W1, class W2>
bool operator<(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `std::less<W3>()(p1.get(), p2.get())`, where W3 is the composite pointer type (C++17 §8) of `W1*` and `W2*`. FIXME ( we do not target C++17)

```C++
template <class W1, class W2>
bool operator>(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `p2 < p1`.

```C++
template <class W1, class W2>
bool operator<=(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `not (p2 < p1)`.

```C++
template <class W1, class W2>
bool operator>=(observer_ptr<W1> p1, observer_ptr<W2> p2);
```

Returns: `not (p1 < p2)`.


## boost::hash support
```C++
template <class T> struct hash<observer_ptr<T>>;
```

## std::hash support
```C++
namespace std {
	template <class T> struct hash<boost::observer_ptr<T>>;
}
```

The template specialization shall meet the requirements of class template `std::hash` (C++17 §23.14.15). For an object `p` of type `boost::observer_ptr<T>`, `std::hash<boost::observer_ptr<T>>()(p)` shall evaluate to the same value as `std::hash<T*>()(p.get())`.
