#pragma once

#include <ostream>
#include <iomanip>
#include <math.h>

const double PI = acos(-1);

template<typename T>
struct Vector3{
    T x, y, z;

    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vector3() : x(0), y(0), z(0) {}

    static Vector3<T> X() { return Vector3<T>(1, 0, 0); }
    static Vector3<T> Y() { return Vector3<T>(0, 1, 0); }
    static Vector3<T> Z() { return Vector3<T>(0, 0, 1); }

    double length() {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3<T> getNormalized() {
        double len = length();
        return Vector3<T>(x / len, y / len, z / len);
    }
};

template<typename T>
Vector3<T> operator+(Vector3<T> a, Vector3<T> b) {
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Vector3<T>& operator+=(Vector3<T> &a, Vector3<T> b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template<typename T>
Vector3<T> operator-(Vector3<T> a, Vector3<T> b) {
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
Vector3<T>& operator-=(Vector3<T> &a, Vector3<T> b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

template<typename T>
Vector3<T> operator-(Vector3<T> a) {
    return Vector3<T>(-a.x, -a.y, -a.z);
}

template<typename T>
Vector3<T> operator*(Vector3<T> a, T b) {
    return Vector3<T>(a.x * b, a.y * b, a.z * b);
}

template<typename T>
Vector3<T> operator*(T a, Vector3<T> b) {
    return Vector3<T>(a * b.x, a * b.y, a * b.z);
}

template<typename T>
Vector3<T>& operator*=(Vector3<T> &a, T b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}


template<typename T>
Vector3<T> operator/(Vector3<T> a, T b) {
    return Vector3<T>(a.x / b, a.y / b, a.z / b);
}


template<typename T>
Vector3<T>& operator/=(Vector3<T> &a, T b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}


template<typename T>
T dot(Vector3<T> a, Vector3<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
Vector3<T> cross(Vector3<T> a, Vector3<T> b) {
    return Vector3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template<typename T>
Vector3<T> rotate(Vector3<T> a, Vector3<T> x, double rad){
    T C = cos(rad);
    T S = sin(rad);

    return (1-C) * dot(a, x) * a + C * x + S * cross(a, x);
}

template<typename T>
Vector3<T> reflect(Vector3<T> a, Vector3<T> n){ //a, n should be normalized
    return 2 * dot(a, n) * n - a;
}

template<typename T>
std::istream& operator>>(std::istream& is, Vector3<T> &v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, Vector3<T> v) {
    os << std::fixed << std::setprecision(8) << v.x << " " << v.y << " " << v.z;
    return os;
}