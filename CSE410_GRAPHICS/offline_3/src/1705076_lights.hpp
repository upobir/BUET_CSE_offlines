#pragma once
//#include <windows.h>
#include <GL/glut.h>

#include "1705076_vector3.hpp"

#include <istream>


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

    Vector3<double> getLightPos(){
        return light_pos;
    }

    double getColor(int c){
        return color[c];
    }

    virtual void readFromStream(std::istream& is) {
        is >> light_pos;
        is >> color[0] >> color[1] >> color[2];
    };

    virtual void draw() {
        glPushMatrix();
        glTranslatef(light_pos.x, light_pos.y, light_pos.z);

        glBegin(GL_QUADS);{

            glColor3f(color[0], color[1], color[2]);

            glVertex3f(1, 1, 1);
            glVertex3f(1, -1, 1);
            glVertex3f(-1, -1, 1);
            glVertex3f(-1, 1, 1);

            glVertex3f(1, 1, -1);
            glVertex3f(1, -1, -1);
            glVertex3f(-1, -1, -1);
            glVertex3f(-1, 1, -1);

            glVertex3f(1, 1, 1);
            glVertex3f(1, 1, -1);
            glVertex3f(-1, 1, -1);
            glVertex3f(-1, 1, 1);

            glVertex3f(1, -1, 1);
            glVertex3f(1, -1, -1);
            glVertex3f(-1, -1, -1);
            glVertex3f(-1, -1, 1);

            glVertex3f(1, 1, 1);
            glVertex3f(1, 1, -1);
            glVertex3f(1, -1, -1);
            glVertex3f(1, -1, 1);

            glVertex3f(-1, 1, 1);
            glVertex3f(-1, 1, -1);
            glVertex3f(-1, -1, -1);
            glVertex3f(-1, -1, 1);

        }glEnd();

        glPopMatrix();
    }

protected:
    Vector3<double> light_pos;
    double color[3];
};


class SpotLight : public PointLight {
public:
    SpotLight() {}

    void setLightDir(Vector3<double> dir){
        light_dir = dir;
    }

    void setCutoffAngle(double angle){
        cutoff_angle = angle;
    }

    void readFromStream(std::istream& is) override {
        PointLight::readFromStream(is);
        is >> light_dir;
        is >> cutoff_angle;
    };

    Vector3<double> getLightDir(){
        return light_dir;
    }

    double getCutoffRadians(){
        return cutoff_angle * PI / 180.0;
    }

private:
    Vector3<double> light_dir;
    double cutoff_angle; // degrees
};