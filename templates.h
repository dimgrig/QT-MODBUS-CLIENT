//
// Created by Dima on 06.03.2021.
//

#ifndef ASTRA_TEMPLATES_H
#define ASTRA_TEMPLATES_H

#include <iterator>
#include <vector>

#include "templates_cout.h"
#include "TestRunner.h"

void TemplatesTest();
void IteratorTest();
void SliceTest();
void ZipTest();


class TestValue {
public:
    TestValue() : value_(0) {};
    TestValue(int value) : value_(value) {};
    int get() const {return value_;};
private:
    int value_;
};

template <class T>
class Iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T*;
    using pointer           = T**;
    using reference         = T*&;

    explicit Iterator<T>(pointer ptr) : p_(ptr) {};
    ~Iterator<T>() = default;

    reference operator*() const { return *p_; }
    pointer operator->() { return p_; }
    Iterator& operator++() { p_++; return *this; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const Iterator& a, const Iterator& b) { return a.p_ == b.p_; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.p_ != b.p_; };
private:
    pointer p_;
};

//template <class T>
//class Iterator_nonpointer
//{
//public:
//    using iterator_category = std::forward_iterator_tag;
//    using difference_type   = std::ptrdiff_t;
//    using value_type        = T;
//    using pointer           = T*;
//    using reference         = T&;

//    explicit Iterator_nonpointer<T>(pointer ptr) : p_(ptr) {};
//    ~Iterator_nonpointer<T>() = default;

//    reference operator*() const { return *p_; }
//    pointer operator->() { return p_; }
//    Iterator_nonpointer& operator++() { p_++; return *this; }
//    Iterator_nonpointer operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
//    friend bool operator== (const Iterator_nonpointer& a, const Iterator_nonpointer& b) { return a.p_ == b.p_; };
//    friend bool operator!= (const Iterator_nonpointer& a, const Iterator_nonpointer& b) { return a.p_ != b.p_; };
//private:
//    pointer p_;
//};

template <class C>
class IteratorImpl
{
public:
    IteratorImpl() {};
    ~IteratorImpl() {};
    //IteratorImpl(C & c) : c_(c) {}
    Iterator<C> begin() { return Iterator<C>(get_it_begin()); }
    Iterator<C> end()   { return Iterator<C>(get_it_end()); }

    virtual C** get_it_begin() = 0;
    virtual C** get_it_end() = 0;
//private:
    //C & c_;
};

template <class C>
class IntValueIterator : public IteratorImpl<C> {
public:
    IntValueIterator() {};
    explicit IntValueIterator(std::vector<C*> &v) : v_(v) {};
    ~IntValueIterator() {};
private:

    virtual C** get_it_begin() override {
        return &v_[0];
    };
    virtual C** get_it_end() override {
        return &v_[v_.size()];
    };

    std::vector<C*> v_;
};

template<typename T>
void slice(std::vector<T> const &v, std::pair<size_t, size_t> p, std::vector<T> &res) {
    res.clear();
    //if (p.first < 0) p.first = 0;
    if (p.first > v.size()) p.first = v.size();
    if (p.second > v.size()) p.second = v.size();

    if (p.first >= v.size()) {
        return;
    }
    if (p.first >= p.second) {
        return;
    }

    auto first = v.begin() + p.first;
    auto last = v.begin() + p.second;

    //res.insert(res.end(), first, last);
    res.assign(first, last);
}

//template <class C>
//class DebugDecorator {
//public:
//    DebugDecorator(const C &c, const std::string &s) : c_(c), s_(s) {};

//    template <typename  ... Args>
//    auto operator()(Args&& ... args) const {
//        std::cout << "Call " << s_ << std::endl;
//        auto res = c_(std::forward<Args>(args) ...);
//        std::cout << "Result: " << res << std::endl;
//        return res;
//    }

//private:
//    C &c_;
//    std::string s_;

//};

//#define DEBUG_CALL(x) decorated_debug(x, #x);

//template <class C>
//auto decorated_debug(const C &c, const std::string &s) {
//    return DebugDecorator(c, s);
//}


template <typename ...T>
std::vector<std::tuple<T...>> zip(std::vector<T> ... v)
{
    std::vector<std::tuple<T...>>  res;
    size_t size = std::min({v.size() ... });
    if (size != 0) {
        for (size_t i = 0; i < size; i++) {
            res.emplace_back(std::make_tuple(*(v.begin()+i) ... ));
        }
    }
    return res;
}

template <typename ...T>
QVector<std::tuple<T...>> qzip(QVector<T> ... v)
{
    QVector<std::tuple<T...>>  res;
    size_t size = std::min({v.size() ... });
    if (size != 0) {
        for (size_t i = 0; i < size; i++) {
            res.push_back(std::make_tuple(*(v.begin()+i) ... ));
        }
    }
    return res;
}

template <typename K, typename V>
static std::map<V, K> reverse_map(const std::map<K, V> &m) {
    std::map<V, K> r;
    for (const auto& kv : m) {
        r[kv.second] = kv.first;
    }
    return r;
}

#endif //ASTRA_TEMPLATES_H
