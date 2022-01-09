#include <vector>
#include <memory>
#include <iostream>

#include "Vector.hpp"

template <typename E>
Vector<E>::Vector() : m_vector() {}

template <typename E>
Vector<E>::~Vector() {}

template <typename E>
template <typename T>
void Vector<E>::add(T e) {
    m_vector.push_back(std::make_unique<T> (e));
}

template <typename E>
template <typename T>
void Vector<E>::push_back(T e) {
    add(e);
}

template <typename E>
void Vector<E>::pop_back() {
    std::cout << size() ;
    if (size() > 0)
        remove(size()-1);
}

template <typename E>
void Vector<E>::clear() {
    m_vector.clear();
}

template <typename E>
E& Vector<E>::get(int index) const {
    return *(m_vector[index].get());
}
    
template <typename E>
void Vector<E>::remove(int index) {
    m_vector.erase(m_vector.begin() + index);
}

template <typename E>
int Vector<E>::size() {
    return m_vector.size();
}

template <typename E>
E& Vector<E>::operator[] (int index) const {
    return get(index);
}
/*
template <typename E>
const E* Vector<E>::begin() const {
    return m_vector.begin();
}

template <typename E>
const E* Vector<E>::end() const {
    return m_vector.end();
}
*/