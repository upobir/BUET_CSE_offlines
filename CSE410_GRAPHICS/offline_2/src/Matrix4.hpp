#pragma once

#include <ostream>
#include <math.h>
#include <cstring>

#include "Vector3.hpp"

const float PI = acos(-1);

template<typename T>
struct Matrix4{
    T vals[4][4];

    Matrix4() {
        memset(vals, 0, sizeof(vals));
    }

    Matrix4(std::initializer_list<std::initializer_list<T>> l) {
        int i = 0;
        for(auto& row : l){
            std::copy(std::begin(row), std::end(row), vals[i]);
            i++;
        }
    }

    Matrix4(Matrix4 const& m){
        memcpy(vals, m.vals, 16 * sizeof(T));
    }

    Matrix4& operator=(Matrix4 const& m){
        memcpy(vals, m.vals, 16 * sizeof(T));
        return *this;
    }

    T& at(int row, int col) {
        return vals[row][col];
    }

    const T& at(int row, int col) const {
        return vals[row][col];
    }

    static Matrix4 I(){
        return Matrix4({
            {1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0},
        });
    }

    static Matrix4 translation(Vector3<T> t){
        return Matrix4({
            {1.0, 0.0, 0.0, t.x},
            {0.0, 1.0, 0.0, t.y},
            {0.0, 0.0, 1.0, t.z},
            {0.0, 0.0, 0.0, 1.0},
        });
    }

    static Matrix4 scale(Vector3<T> s){
        return Matrix4({
            {s.x, 0.0, 0.0, 0.0},
            {0.0, s.y, 0.0, 0.0},
            {0.0, 0.0, s.z, 0.0},
            {0.0, 0.0, 0.0, 1.0},
        });
    }

    static Matrix4 rotation(T angle, Vector3<T> a){

        T rad = angle * PI / 180;
        auto rx = rotate(a, Vector3<T>::X(), rad);
        auto ry = rotate(a, Vector3<T>::Y(), rad);
        auto rz = rotate(a, Vector3<T>::Z(), rad);

        return Matrix4({
            {rx.x, ry.x, rz.x, 0.0},
            {rx.y, ry.y, rz.y, 0.0},
            {rx.z, ry.z, rz.z, 0.0},
            { 0.0,  0.0,  0.0, 1.0},
        });
    }
};


template<typename T>
Matrix4<T> operator*(Matrix4<T> const& m1, Matrix4<T> const& m2){
    Matrix4<T> res;
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            for(int k = 0; k<4; k++){
                res.at(i, j) += m1.at(i, k) * m2.at(k, j);
            }
        }
    }

    return res;
}


template<typename T>
Vector3<T> operator*(Matrix4<T> const& m, Vector3<T> const& v){

    Vector3<T> res;

    res.x = m.at(0, 0) * v.x + m.at(0, 1) * v.y + m.at(0, 2) * v.z + m.at(0, 3);
    res.y = m.at(1, 0) * v.x + m.at(1, 1) * v.y + m.at(1, 2) * v.z + m.at(1, 3);
    res.z = m.at(2, 0) * v.x + m.at(2, 1) * v.y + m.at(2, 2) * v.z + m.at(2, 3);

    T w = m.at(3, 0) * v.x + m.at(3, 1) * v.y + m.at(3, 2) * v.z + m.at(3, 3);

    res /= w;

    return res;
}