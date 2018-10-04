#ifndef __COMPLEX_HPP__
#define __COMPLEX_HPP__
#include <iostream>

namespace lab34
{
  // examle user type
  class Complex
  {
  public:
    Complex(const double &real = 0, const double &imaginary = 0);
    ~Complex();
    Complex(const Complex &value);

    void operator () (const Complex &value);
    bool operator==(const Complex &value) const;
    bool operator!=(const Complex &value) const;
    bool operator>(const Complex &value) const;
    bool operator<(const Complex &value) const;
    bool operator>=(const Complex &value) const;
    bool operator<=(const Complex &value) const;

    const Complex &operator=(const Complex &value);
    const Complex &operator+=(const Complex &value);
    const Complex &operator-=(const Complex &value);
    const Complex &operator*=(const Complex &value);

    const Complex &operator+() const;
    Complex operator+(const Complex &value) const;

    Complex operator-() const;
    Complex operator-(const Complex &value) const;

    Complex operator*(const Complex &value) const;

    std::string toString() const;

    // real getter/setter
    const double &getReal() const;
    void setReal(const double &value);

    // imaginary getter/setter
    const double &getImaginary() const;
    void setImaginary(const double &value);

    // support functions
    // returns length of complex vector(real, imaginary)
    double length() const;

  private:
    double imaginary;
    double real;
  };
} // namespace lab34
#endif