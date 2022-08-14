#pragma once
//#include <windows.h>
#include <GL/glut.h>

#include "1705076_vector3.hpp"

class Camera{
public:
    Camera(double _move_delta = 10.0, double _angle_delta_degrees = 5){
        position = {0, -200, 50};
        up = Vector3<double>::Z();
        look = Vector3<double>::Y();
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

    void moveFront(){
        position += move_delta * look;
    }

    void moveBack(){
        position -= move_delta * look;
    }

    void moveRight(){
        position += move_delta * right;
    }

    void moveLeft(){
        position -= move_delta * right;
    }

    void moveUp(){
        position += move_delta * up;
    }

    void moveDown(){
        position -= move_delta * up;
    }

    void lookLeft(){
        look = rotate(up, look, +angle_delta);
        right = rotate(up, right, +angle_delta);
    }

    void lookRight(){
        look = rotate(up, look, -angle_delta);
        right = rotate(up, right, -angle_delta);
    }

    void lookUp(){
        look = rotate(right, look, +angle_delta);
        up = rotate(right, up, +angle_delta);
    }

    void lookDown(){
        look = rotate(right, look, -angle_delta);
        up = rotate(right, up, -angle_delta);
    }

    void tiltClockwise(){
        up = rotate(look, up, +angle_delta);
        right = rotate(look, right, +angle_delta);
    }

    void tiltCounterclockwise(){
        up = rotate(look, up, -angle_delta);
        right = rotate(look, right, -angle_delta);
    }

    Vector3<double> getPosition(){return position;}

    Vector3<double> getUp(){return up;}

    Vector3<double> getRight(){return right;}

    Vector3<double> getLook(){return look;}

private:
    Vector3<double> up, right, look, position;
    double move_delta , angle_delta;
};
