#pragma once
#include <windows.h>
#include <glut.h>
#include "1705076_Vector3.hpp"

const double PI = acos(-1);

class Camera{
public:
    Camera(double _move_delta = 10.0, double _angle_delta_degrees = 5){
        position = {0, 0, 200};
        up = Vector3<double>::Y();
        look = -Vector3<double>::Z();
        right = Vector3<double>::X();
        move_delta = _move_delta;
        angle_delta = _angle_delta_degrees * PI/180.0;
    }

    void lookAt(){
        Vector3<double> look_point = position + look;
        gluLookAt(position.x, position.y, position.z
              , look_point.x, look_point.y, look_point.z
              , up.x, up.y, up.z);
    }

    void move_front(){
        position += move_delta * look;
    }

    void move_back(){
        position -= move_delta * look;
    }

    void move_right(){
        position += move_delta * right;
    }

    void move_left(){
        position -= move_delta * right;
    }

    void move_up(){
        position += move_delta * up;
    }

    void move_down(){
        position -= move_delta * up;
    }

    void look_left(){
        look = rotate(up, look, +angle_delta);
        right = rotate(up, right, +angle_delta);
    }

    void look_right(){
        look = rotate(up, look, -angle_delta);
        right = rotate(up, right, -angle_delta);
    }

    void look_up(){
        look = rotate(right, look, +angle_delta);
        up = rotate(right, up, +angle_delta);
    }

    void look_down(){
        look = rotate(right, look, -angle_delta);
        up = rotate(right, up, -angle_delta);
    }

    void tilt_clockwise(){
        up = rotate(look, up, +angle_delta);
        right = rotate(look, right, +angle_delta);
    }

    void tilt_counterclockwise(){
        up = rotate(look, up, -angle_delta);
        right = rotate(look, right, -angle_delta);
    }

private:
    Vector3<double> up, right, look, position;
    double move_delta , angle_delta;
};

class Object {
public:
    object() {}

    virtual void draw() {};

    void setColor(double r, double g, double b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    };

    void setShine(int _shine) {
        shine = _shine;
    };

    void setCoefficients(double ambient, double diffuse, double specular, double reflection) {
        coefs[0] = ambient;
        coefs[1] = diffuse;
        coefs[2] = specular;
        coefs[3] = reflection;
    };

protected:
    Vector3<double> ref_point;
    double height, width, length;
    double color[3];
    double coefs[4];
    int shine;
};

class Sphere : public Object {
public:
    Sphere(Vector3<double> center, double radius) {
        ref_point = center;
        length = radius;
    }

    void draw() override {
        /// TODO
    }

private:
};

class PointLight {
public:
    PointLight() {}

    void setLightPos(Vector3<double> pos){
        light_pos = pos;
    }

    void setColor(double r, double g, double b){
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

protected:
    Vector3<double> light_pos;
    double color[3];
};

class Spotlight : public PointLight {
public:
    Spotlight() {}

    void setLightDir(Vector3<double> dir){
        light_dir = dir;
    }

    void setCutoffAngle(double angle){
        cutoff_angle = angle;
    }

private:
    Vector3<double> light_dir;
    double cutoff_angle;
};