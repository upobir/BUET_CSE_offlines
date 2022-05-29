#pragma once

#include <ostream>
#include <math.h>

template<typename T>
struct Vector{
    T x, y, z;

    Vector(T x, T y, T z) : x(x), y(y), z(z) {}
    Vector() : x(0), y(0), z(0) {}

    static Vector<T> X() { return Vector<T>(1, 0, 0); }
    static Vector<T> Y() { return Vector<T>(0, 1, 0); }
    static Vector<T> Z() { return Vector<T>(0, 0, 1); }

    double length() {
        return sqrt(x * x + y * y + z * z);
    }

    Vector<T> normalize() {
        double len = length();
        return Vector<T>(x / len, y / len, z / len);
    }
};

template<typename T>
Vector<T> operator+(Vector<T> a, Vector<T> b) {
    return Vector<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Vector<T>& operator+=(Vector<T> &a, Vector<T> b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template<typename T>
Vector<T> operator-(Vector<T> a, Vector<T> b) {
    return Vector<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
Vector<T>& operator-=(Vector<T> &a, Vector<T> b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

template<typename T>
Vector<T> operator-(Vector<T> a) {
    return Vector<T>(-a.x, -a.y, -a.z);
}

template<typename T>
Vector<T> operator*(Vector<T> a, T b) {
    return Vector<T>(a.x * b, a.y * b, a.z * b);
}

template<typename T>
Vector<T> operator*(T a, Vector<T> b) {
    return Vector<T>(a * b.x, a * b.y, a * b.z);
}

template<typename T>
T dot(Vector<T> a, Vector<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
Vector<T> cross(Vector<T> a, Vector<T> b) {
    return Vector<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template<typename T>
Vector<T> rotate(Vector<T> r, Vector<T> x, double rad){
    T C = cos(rad);
    T S = sin(rad);

    return (1-C) * dot(r, x) * r + C * x + S * cross(r, x);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, Vector<T> v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
