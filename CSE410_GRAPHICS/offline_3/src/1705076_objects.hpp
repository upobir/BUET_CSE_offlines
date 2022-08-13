#pragma once
#include <windows.h>
#include <glut.h>
#include "1705076_vector3.hpp"

#include <istream>
#include <iostream>

const int SLICE_COUNT = 100;

struct Ray {
    Vector3<double> start, dir;

    Ray(Vector3<double> _start, Vector3<double> _dir){
        start = _start;
        dir = _dir;
    }
};

class Object {
public:
    object() {}

    virtual void draw() {}

    void setColor(double r, double g, double b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShine(int _shine) {
        shine = _shine;
    }

    void setCoefficients(double ambient, double diffuse, double specular, double reflection) {
        coefs[0] = ambient;
        coefs[1] = diffuse;
        coefs[2] = specular;
        coefs[3] = reflection;
    }

    virtual void readFromStream(std::istream& is) {
        is >> color[0] >> color[1] >> color[2];
        is >> coefs[0] >> coefs[1] >> coefs[2] >> coefs[3];
        is >> shine;
    }

protected:
    Vector3<double> ref_point;
    double height, width, length;
    double color[3];
    double coefs[4]; /// ambient, diffuse, specular, reflection
    int shine;
};

class Sphere : public Object {
public:
    Sphere(Vector3<double> center, double _radius) {
        ref_point = center;
        radius = _radius;
    }

    void readFromStream(std::istream& is) override {
        is >> ref_point;
        is >> radius;
        Object::readFromStream(is);
    }

    void draw() override {
        Vector3<double> cur[SLICE_COUNT+1], old[SLICE_COUNT+1];

        glPushMatrix();
        glTranslatef(ref_point.x, ref_point.y, ref_point.z);

        for(int i = 0; i <= SLICE_COUNT; i++){
            old[i] = Vector3<double>(0, 0, -radius);
        }

        for(int j = 2; j<=SLICE_COUNT; j++){
            double z = -radius + 2*radius * j / (double)SLICE_COUNT;
            
            if(z > radius)
                z = radius;
            double r = sqrt(radius*radius-z*z);

            // std::cout<<z<<" "<<r<<std::endl;

            for(int i = 0; i <= SLICE_COUNT; i++){
                cur[i] = Vector3<double>(cos(2*PI*i/SLICE_COUNT)*r, sin(2*PI*i/SLICE_COUNT)*r, z);
            }
            
            for(int i = 0; i<SLICE_COUNT; i++){
                glBegin(GL_QUADS);{
                    glColor3f(color[0], color[1], color[2]);

                    glVertex3f(old[i].x, old[i].y, old[i].z);
                    glVertex3f(old[i+1].x, old[i+1].y, old[i+1].z);
                    glVertex3f(cur[i+1].x, cur[i+1].y, cur[i+1].z);
                    glVertex3f(cur[i].x, cur[i].y, cur[i].z);
                }glEnd();
            }

            std::swap(cur, old);
        }

        glPopMatrix();

        // std::cout<<color[0] << " "<<color[1]<<" "<<color[2]<<std::endl;
        // exit(0);
    }

private:
    double radius;
};


class Triangle : public Object {
public:
    Triangle(Vector3<double> _A, Vector3<double> _B, Vector3<double> _C) {
        ref_point = _A;
        B = _B;
        C = _C;
    }

    void readFromStream(std::istream& is) override {
        is >> ref_point >> B >> C;
        Object::readFromStream(is);
    }

    void draw() override {
        glBegin(GL_TRIANGLES);{
            glColor3f(color[0], color[1], color[2]);

            glVertex3f(ref_point.x, ref_point.y, ref_point.z);
            glVertex3f(B.x, B.y, B.z);
            glVertex3f(C.x, C.y, C.z);

        }glEnd();
    }

private:
    Vector3<double> B, C;
};


class QuadSurface : public Object {
public:
    QuadSurface() {}

    void readFromStream(std::istream& is) override {
        is >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
        is >> ref_point >> length >> width >> height;
        Object::readFromStream(is);
    }

    void draw() override {
    }

private:
    double A, B, C, D, E, F, G, H, I, J;
};


class Floor : public Object {
public:
    Floor(double floorWidth, double _tileWidth) {
        ref_point = Vector3<double>(-floorWidth/2, -floorWidth/2, 0);
        tileWidth = _tileWidth;
        length = width = floorWidth;
    }

    void draw() override {
        int tileCount = length/tileWidth;

        for(int i = 0; i<tileCount; i++){
            for(int j = 0; j<tileCount; j++){
                Vector3<double> base = ref_point + Vector3<double>(i, j, 0) * tileWidth;
                double c = ((i+j)%2 == 0 ? 0.0 : 1.0);

                glBegin(GL_QUADS);{
                    glColor3f(c, c, c);

                    glVertex3f(base.x, base.y, 0.0);
                    glVertex3f(base.x+tileWidth, base.y, 0.0);
                    glVertex3f(base.x+tileWidth, base.y+tileWidth, 0.0);
                    glVertex3f(base.x, base.y+tileWidth, 0.0);
        
                }glEnd();
            }
        }
    }

private:
    double tileWidth;

};



