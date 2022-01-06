#ifndef COST_HPP
#define COST_HPP

#include <string>
#include <vector>

#include "Creature.hpp"

class Cost{

    private:
        int m_any;
        int m_white;
        int m_black;
        int m_red;
        int m_green;
        int m_blue;

    public:
        Cost(int, int, int, int, int, int);
        ~Cost();

        void print() const;
};

#endif