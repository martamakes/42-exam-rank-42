#ifndef VECT2_HPP
# define VECT2_HPP

# include <iostream>

class vect2
{
private:
    int x;
    int y;

public:
    // Constructors / canonical form
    vect2();
    vect2(int x, int y);
    vect2(const vect2& other);
    vect2& operator=(const vect2& other);
    ~vect2();

    // Addition
    vect2 operator+(const vect2& other) const;
    vect2& operator+=(const vect2& other);

    // Subtraction
    vect2 operator-(const vect2& other) const;
    vect2& operator-=(const vect2& other);

    // Scalar multiplication
    vect2 operator*(int scalar) const;
    vect2& operator*=(int scalar);

    // Unary minus
    vect2 operator-() const;

    // Increment / decrement
    vect2& operator++();
    vect2 operator++(int);
    vect2& operator--();
    vect2 operator--(int);

    // Access
    int& operator[](int index);
    const int& operator[](int index) const;

    // Comparison
    bool operator==(const vect2& other) const;
    bool operator!=(const vect2& other) const;
};

vect2 operator*(int scalar, const vect2& vec);
std::ostream& operator<<(std::ostream& os, const vect2& vec);

#endif