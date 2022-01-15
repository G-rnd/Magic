#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <memory>

template <typename E>
class Vector {
    private:
        std::vector<std::unique_ptr<E> >    m_vector;

    public:
        Vector();
        ~Vector();

        template<typename T>
        void                                add(T e);
        template<typename T>
        void                                push_back(T e);
        void                                pop_back();
        void                                clear();
        E&                                  get(int index) const;
        void                                remove(int index);
        int                                 size();
        
        E&                                  operator[] (int index) const;
        /*
        const E*                            begin() const;
        const E*                            end() const;
        */
};

#endif
