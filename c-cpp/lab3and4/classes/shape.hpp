#ifndef __SHAPE__HPP__
#define __SHAPE__HPP__
#include <vector>
#include <iostream>
#include "exceptions/toosmallvalueexception.hpp"
#include "exceptions/toolargevalueexception.hpp"


namespace lab34
{
    class Shape
    {
    public:
        Shape(const std::vector<std::size_t> &coefs);
        Shape(const Shape &value);
        ~Shape();

        bool operator == (const Shape &value) const;
        bool operator != (const Shape &value) const;
        const Shape &operator=(const Shape &value);
        // Shape[i] == shape coef on index i,
        // if i < 0 -> -1 - end, -2 - pre-end (like in Python)
        const std::size_t &operator[](int index) const;

        std::string toString() const;

        // count of parameters define a shape
        const std::size_t &getCoefsCount() const;

    private:
        std::vector<std::size_t> content;
        std::size_t coefsCount;
    };
} // namespace lab34

#endif