/*
 * Iterator Design Pattern
 *
 * Intent: Lets you traverse elements of a collection without exposing its
 * underlying representation (list, stack, tree, etc.).
 */

#include <iostream>
#include <utility>
#include <vector>
#include "../SmartPointer.h"

/*
 * The Iterator pattern is a common design pattern in many programming languages.
 * It is used to traverse a container and access the container's elements. The
 * iterator pattern decouples algorithms from containers; in some cases, algorithms
 * are necessarily container-specific and thus cannot be decoupled.
 */

/*
 * C++ already provides a few iterators for the standard library containers.
 * For example, std::vector has the std::vector::iterator type. However,
 * you can also create your own iterators.
 */

template <typename T, typename U>
class Iterator {
public:
    typedef typename std::vector<T>::iterator iterator_type;
    explicit Iterator(SmartPtr<U> p_data) : m_p_data_(std::move(p_data)) {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void First() {
        m_it_ = m_p_data_->m_data_.begin();
        if (m_it_ != m_p_data_->m_data_.end()) {
            m_it_--;
        }
    }


    void Next() {
        m_it_++;
    }

    bool IsDone() {
        return (m_it_ == m_p_data_->m_data_.end());
    }

    T CurrentItem() {
        return *m_it_;
    }

private:
    SmartPtr<U> m_p_data_;
    iterator_type m_it_;
};

/*
 * The Container owns some important data and also defines a method for
 * retrieving fresh iterator instances, compatible with the container class.
 */

template <class T>
class Container {
    friend class Iterator<T, Container>;
public:
    void Add(T a) {
        m_data_.push_back(a);
    }

    SmartPtr<Iterator<T, Container>> CreateIterator() {
        return SmartPtr<Iterator<T, Container>>(new Iterator<T, Container>(SmartPtr<Container>(this)));
    }

private:
    std::vector<T> m_data_;
};

class Data {
public:
    explicit Data(int a = 0) : m_data_(a) {}

    void set_data(int a) {
        m_data_ = a;
    }

    [[nodiscard]] int data() const {
        return m_data_;
    }

private:
    int m_data_;
};

/*
 * The client code may or may not know about the Concrete Iterator or Collection
 * classes, depending on the level of indirection you want to keep in your program.
 */

void ClientCode() {
    std::cout << " ------------------ Iterator with int------------------";
    Container<int> cont;
    for (int i = 0; i < 10; i++) {
        cont.Add(i);
    }

    SmartPtr<Iterator<int, Container<int>>> it = cont.CreateIterator();
    for (it->First(); !it->IsDone(); it->Next()) {
        std::cout << it->CurrentItem() << std::endl;
    }

    Container<Data> cont2;
    Data a(1), b(10), c(100);
    cont2.Add(a);
    cont2.Add(b);
    cont2.Add(c);

    std::cout << " ------------------ Iterator with Custom Class------------------";
    SmartPtr<Iterator<Data, Container<Data>>> it2 = cont2.CreateIterator();
    for (it2->First(); !it2->IsDone(); it2->Next()) {
        std::cout << it2->CurrentItem().data() << std::endl;
    }
}

int main() {
    ClientCode();
}