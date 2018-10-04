#include "complex.hpp"
#include <cmath>
#include <string>

namespace lab34
{
  Complex::Complex(const double &real, const double &imaginary)
    : real(real), imaginary(imaginary)
  {
  }

  Complex::Complex(const Complex &value)
    : real(value.getReal()), imaginary(value.getImaginary())
  {
  }

  Complex::~Complex()
  {
  }

  //------------------------------------------------------------ operators
  void Complex::operator()(const Complex & value)
  {
    real = value.getReal();
    imaginary = value.getImaginary();
  }

  bool Complex::operator==(const Complex &value) const
  {
    return (this->real == value.getReal()) &&
      (this->imaginary == value.getImaginary());
  }

  bool Complex::operator!=(const Complex &value) const
  {
    return !((*this) == value);
  }

  bool Complex::operator>(const Complex &value) const
  {
    return this->length() > value.length();
  }

  bool Complex::operator<(const Complex &value) const
  {
    return this->length() < value.length();
  }

  bool Complex::operator>=(const Complex &value) const
  {
    return !((*this) < value);
  }

  bool Complex::operator<=(const Complex &value) const
  {
    return !((*this) > value);
  }

  const Complex &Complex::operator=(const Complex &value)
  {
    // check for self-asignment
    if (this == &value)
      return *this;

    this->setImaginary(value.getImaginary());
    this->setReal(value.getReal());

    return *this;
  }

  const Complex &Complex::operator+=(const Complex &value)
  {
    return *this = *this + value;
  }

  const Complex &Complex::operator-=(const Complex &value)
  {
    return *this = *this - value;
  }

  const Complex &Complex::operator*=(const Complex &value)
  {
    return *this = *this * value;
  }

  const Complex &Complex::operator+() const
  {
    return *this;
  }

  Complex Complex::operator+(const Complex &value) const
  {
    double real = this->getReal() + value.getReal();
    double imaginary = this->getImaginary() + value.getImaginary();

    return Complex(real, imaginary);
  }

  Complex Complex::operator-() const
  {
    return Complex(-real, -imaginary);
  }

  Complex Complex::operator-(const Complex &value) const
  {
    double real = this->getReal() - value.getReal();
    double imaginary = this->getImaginary() - value.getImaginary();

    return Complex(real, imaginary);
  }

  Complex Complex::operator*(const Complex &value) const
  {
    double real = this->real * value.getReal() -
      this->imaginary * value.getImaginary();

    double imaginary = this->real * value.getImaginary() +
      value.getReal() * this->imaginary;

    return Complex(real, imaginary);
  }
  //------------------------------------------------------------ support
  std::string Complex::toString() const
  {
    return std::string(
      "(" + std::to_string(real) +
      ", " + std::to_string(imaginary) + "i)"
    );
  }
  //------------------------------------------------------------ support
  //------------------------------------------------------------ operators
  //------------------------------------------------------------ real
  const double &Complex::getReal() const
  {
    return real;
  }

  void Complex::setReal(const double &value)
  {
    real = value;
  }
  //------------------------------------------------------------ real
  //------------------------------------------------------------ imaginary
  const double &Complex::getImaginary() const
  {
    return imaginary;
  }

  void Complex::setImaginary(const double &value)
  {
    imaginary = value;
  }
  //------------------------------------------------------------ imaginary
  //------------------------------------------------------------ support
  double Complex::length() const
  {
    return sqrt(this->imaginary * this->imaginary +
      this->real * this->real);
  }
  //------------------------------------------------------------ support
} // namespace lab34