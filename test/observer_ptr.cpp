
//          Copyright Bernhard Manfred Gruber & Michael Florian Hava.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/core/demangle.hpp>
#include "observer_ptr/observer_ptr.hpp"

#include <string>
#include <iomanip>

namespace boost {
	template <typename T>
	auto operator<<(std::ostream& os, const boost::observer_ptr<T>& op) -> std::ostream& {
		return os << "observer_ptr<" << boost::core::demangle(typeid(T).name()) << ">{0x" << std::hex << op.get() << "}";
	}
}

BOOST_AUTO_TEST_SUITE(observer_ptr)

BOOST_AUTO_TEST_CASE(ctor_default) {
	boost::observer_ptr<int> op;
	BOOST_TEST(op.get() == nullptr);
}

BOOST_AUTO_TEST_CASE(ctor_nullptr) {
	boost::observer_ptr<int> op{nullptr};
	BOOST_TEST(op.get() == nullptr);
}

BOOST_AUTO_TEST_CASE(ctor_from_raw_pointer) {
	int i = 42;
	int* p = &i;
	boost::observer_ptr<int> op{p};
	BOOST_TEST(op.get() == p);
}

BOOST_AUTO_TEST_CASE(ctor_from_observer_different_type) {
	int i = 42;
	boost::observer_ptr<int> op1{&i};
	boost::observer_ptr<const int> op2{op1};
	BOOST_TEST(op1.get() == op2.get());
}

BOOST_AUTO_TEST_CASE(copy_ctor) {
	int i = 42;
	boost::observer_ptr<int> op1{&i};
	boost::observer_ptr<int> op2{op1};
	BOOST_TEST(op1.get() == op2.get());
}

BOOST_AUTO_TEST_CASE(dereference) {
	int i = 42;
	boost::observer_ptr<int> op{&i};
	BOOST_TEST(*op == 42);
}

BOOST_AUTO_TEST_CASE(arrow_operator) {
	std::string s = "Hello";
	boost::observer_ptr<std::string> op{&s};
	BOOST_TEST(op->size() == 5);
}

BOOST_AUTO_TEST_CASE(operator_bool) {
	boost::observer_ptr<int> op1;
	BOOST_TEST((bool)op1 == false);

	int i = 42;
	boost::observer_ptr<int> op2{&i};
	BOOST_TEST((bool)op2);
}

BOOST_AUTO_TEST_CASE(pointer_conversion) {
	int i = 42;
	boost::observer_ptr<int> op{&i};
	int* p = static_cast<int*>(op);
	BOOST_TEST(p == &i);
}

BOOST_AUTO_TEST_CASE(release) {
	int i = 42;
	boost::observer_ptr<int> op{&i};
	BOOST_TEST(op.get() == &i);
	auto p = op.release();
	BOOST_TEST(p == &i);
	BOOST_TEST(op.get() == nullptr);
}

BOOST_AUTO_TEST_CASE(reset) {
	int i = 42;
	boost::observer_ptr<int> op{&i};
	op.reset();
	BOOST_TEST(op.get() == nullptr);
	op.reset(&i);
	BOOST_TEST(op.get() == &i);
}

BOOST_AUTO_TEST_CASE(swap) {
	int a = 42, b = 43;
	boost::observer_ptr<int> opA{&a};
	boost::observer_ptr<int> opB{&b};
	std::swap(opA, opB);
	BOOST_TEST(opA.get() == &b);
	BOOST_TEST(opB.get() == &a);
}

BOOST_AUTO_TEST_CASE(make_observer) {
	int i = 42;
	auto op = boost::make_observer(&i);
	BOOST_TEST(op.get() == &i);
}

BOOST_AUTO_TEST_CASE(equality_comparison) {
	int a = 42, b = 42;
	boost::observer_ptr<int> opA{&a};
	BOOST_TEST(opA == opA);
	BOOST_TEST(!(opA != opA));
	boost::observer_ptr<int> opB{&b};
	BOOST_TEST(!(opA == opB));
	BOOST_TEST(opA != opB);
}

BOOST_AUTO_TEST_CASE(less_than_comparison) {
	int* a = (int*)42, *b = (int*)43;
	boost::observer_ptr<int> opA{a};
	boost::observer_ptr<int> opB{b};
	BOOST_TEST(opA < opB);
	BOOST_TEST(opA <= opB);
	BOOST_TEST(opA <= opA);
	BOOST_TEST(opB > opA);
	BOOST_TEST(opB >= opA);
	BOOST_TEST(opB >= opB);
}

BOOST_AUTO_TEST_CASE(hash) {
	int i = 42;
	auto op = boost::make_observer(&i);
	BOOST_TEST(std::hash<boost::observer_ptr<int>>{}(op) == std::hash<int*>{}(op.get()));
}

BOOST_AUTO_TEST_SUITE_END()
