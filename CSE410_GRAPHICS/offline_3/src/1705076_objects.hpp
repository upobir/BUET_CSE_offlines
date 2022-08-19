#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

#include <GL/glut.h>

#include "1705076_vector3.hpp"
#include "1705076_lights.hpp"

#include <istream>
#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <limits>
#include <cassert>

const int SLICE_COUNT = 100;
const double epsilon = 0.001;

extern std::vector<PointLight> pointLights;
extern std::vector<SpotLight> spotLights;

extern int recursionLevelMax;
extern bool debug;

const int AMB = 0;
const int DIFF = 1;
const int SPEC = 2;
const int REFL = 3;


struct Ray {
    Vector3<double> start, dir;

    Ray(Vector3<double> _start, Vector3<double> _dir){
        start = _start;
        dir = _dir.getNormalized();
    }

    Vector3<double> at(double t){
        return start + dir * t;
    }
};


class Object {
public:
    Object() {}

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

    virtual double intersect(Ray& ray, double* color, int level){
        return -1.0;
    }

    static void raytrace(Ray& ray, double* color, int level);

protected:
    Vector3<double> ref_point;
    double height, width, length;
    std::array<double, 3> color;
    double coefs[4]; /// ambient, diffuse, specular, reflection
    int shine;

    void processColor(Ray& view, Vector3<double> point, double* seencolor);

    virtual Vector3<double> getNormal(Vector3<double> point) = 0;

    virtual std::array<double, 3> getColorAtPoint(Vector3<double> point) = 0;

    bool isInShadow(Ray ray, Vector3<double> point);

};

extern std::vector<std::shared_ptr<Object>> objects;

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

    double intersect(Ray& ray, double* seencolor, int level) override {
        double a = dot(ray.dir, ray.dir);
        double b = 2 * dot(ray.dir, ray.start - ref_point);
        double c = dot(ray.start - ref_point, ray.start - ref_point) - radius * radius;
        double descrim = b * b - 4 * a * c;
        if(descrim < 0) 
            return -1;

        double t = (-b - sqrt(descrim)) / (2 * a);
        if(t < 0)
            t = (-b + sqrt(descrim)) / (2 * a);

        if(t < 0)
            return -1;

        if(level == 0)
            return t;

        auto point = ray.at(t);

        processColor(ray, point, seencolor);

        if(level >= recursionLevelMax)
            return t;

        auto normal = getNormal(point);
        if(dot(normal, ray.dir) > 0)
            normal = -normal;

        auto reflected = reflect(-ray.dir, normal);

        double reflcolor[3];

        Ray newray (point + epsilon * reflected, reflected);

        Object::raytrace(newray, reflcolor, level+1);

        for(int c = 0; c < 3; c++){
            seencolor[c] += reflcolor[c] * coefs[REFL];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }

        return t;
    }

protected:
    double radius;

    Vector3<double> getNormal(Vector3<double> point) override {
        return (point-ref_point).getNormalized();
    }

    std::array<double, 3> getColorAtPoint(Vector3<double> point) override {
        return color;
    }

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

    virtual double intersect(Ray& ray, double* seencolor, int level){
        auto B_del = B - ref_point;
        auto C_del = C - ref_point;
        auto normal = cross(B_del, C_del);
        
        if(dot(ray.dir, normal) == 0)
            return -1;

        double t = dot(ref_point - ray.start, normal) / dot(ray.dir, normal);
        auto point = ray.at(t);

        auto c = dot(point - ref_point, cross(normal, B_del)) / dot(C_del , cross(normal, B_del));
        auto b = dot(point - ref_point, cross(normal, C_del)) / dot(B_del , cross(normal, C_del));

        if(b < 0 || c < 0 || b+c > 1) 
            return -1;

        if(level == 0)
            return t;

        processColor(ray, point, seencolor);

        if(level >= recursionLevelMax)
            return t;

        normal = getNormal(point);
        if(dot(normal, ray.dir) > 0)
            normal = -normal;

        auto reflected = reflect(-ray.dir, normal);

        double reflcolor[3];

        Ray newray (point + epsilon * reflected, reflected);

        Object::raytrace(newray, reflcolor, level+1);

        for(int c = 0; c < 3; c++){
            seencolor[c] += reflcolor[c] * coefs[REFL];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }

        return t;
    }

protected:
    Vector3<double> B, C;

    Vector3<double> getNormal(Vector3<double> point) override {
        return cross(B-ref_point, C-ref_point).getNormalized();
    }

    std::array<double, 3> getColorAtPoint(Vector3<double> point) override {
        return color;
    }
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

    virtual double intersect(Ray& ray, double* seencolor, int level){
        double a = A*ray.dir.x*ray.dir.x + B*ray.dir.y*ray.dir.y + C*ray.dir.z*ray.dir.z
            + D*ray.dir.x*ray.dir.y + E*ray.dir.y*ray.dir.z + F*ray.dir.x*ray.dir.z;

        double b = A*2*ray.dir.x*ray.start.x + B*2*ray.dir.y*ray.start.y + C*2*ray.dir.z*ray.start.z
            + D*ray.dir.x*ray.start.y + D*ray.start.x*ray.dir.y
            + E*ray.dir.y*ray.start.z + E*ray.start.y*ray.dir.z
            + F*ray.dir.x*ray.start.z + F*ray.start.x*ray.dir.z
            + G*ray.dir.x + H*ray.dir.y + I*ray.dir.z;

        double c = A*ray.start.x*ray.start.x + B*ray.start.y*ray.start.y + C*ray.start.z*ray.start.z
            + D*ray.start.x*ray.start.y + E*ray.start.y*ray.start.z + F*ray.start.x*ray.start.z
            + G*ray.start.x + H*ray.start.y + I*ray.start.z + J;

        if(a == 0 && b == 0)
            return -1;

        double t;
        Vector3<double> point;

        if(a == 0){
            t = -c / b;
            point = ray.at(t);

            if(t < 0 || isClipped(point))
                return -1;
        }
        else{
            double descrim = b * b - 4 * a * c;
            if(descrim < 0) 
                return -1;

            t = (-b - sqrt(descrim)) / (2 * a);
            point = ray.at(t);

            if(t < 0 || isClipped(point)){
                t = (-b + sqrt(descrim)) / (2 * a);
                point = ray.at(t);

                if(t < 0 || isClipped(point))
                    return -1;
            }
        }

        if(level == 0)
            return t;

        processColor(ray, point, seencolor);

        if(level >= recursionLevelMax)
            return t;

        auto normal = getNormal(point);
        if(dot(normal, ray.dir) > 0)
            normal = -normal;

        auto reflected = reflect(-ray.dir, normal);

        double reflcolor[3];

        Ray newray (point + epsilon * reflected, reflected);

        Object::raytrace(newray, reflcolor, level+1);

        for(int c = 0; c < 3; c++){
            seencolor[c] += reflcolor[c] * coefs[REFL];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }

        return t;
    }

protected:
    double A, B, C, D, E, F, G, H, I, J;

    bool isClipped(Vector3<double> point){
        if (length != 0 && (point.x < ref_point.x || ref_point.x + length < point.x))
            return true;
        if (width != 0 && (point.y < ref_point.y || ref_point.y + width < point.y))
            return true;
        if (height != 0 && (point.z < ref_point.z || ref_point.z + height < point.z))
            return true;
        return false;
    }

    Vector3<double> getNormal(Vector3<double> point) override {
        return Vector3<double>(2*A*point.x + D*point.y + F*point.z + G,
                       2*B*point.y + D*point.x + E*point.z + H,
                       2*C*point.z + E*point.y + F*point.x + I).getNormalized();
    }

    std::array<double, 3> getColorAtPoint(Vector3<double> point) override {
        return color;
    }
};


class Floor : public Object {
public:
    Floor(double floorWidth, double _tileWidth) {
        ref_point = Vector3<double>(-floorWidth/2, -floorWidth/2, 0);
        tileWidth = _tileWidth;
        length = width = floorWidth;
        coefs[AMB] = 0.2;
        coefs[DIFF] = 0.3;
        coefs[SPEC] = 0.3;
        coefs[REFL] = 0.6;
        shine = 5;
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

    double intersect(Ray& ray, double* seencolor, int level) override {
        if(ray.dir.z == 0)
            return -1.0;
        double t = -ray.start.z / ray.dir.z;
        auto point = ray.at(t);

        if (t <= 0 || point.x < ref_point.x || point.x > ref_point.x + length || point.y < ref_point.y || point.y > ref_point.y + width)
            return -1;

        if(level == 0)
            return t;
        
        processColor(ray, point, seencolor);

        if(level >= recursionLevelMax)
            return t;

        auto normal = getNormal(point);
        if(dot(normal, ray.dir) > 0)
            normal = -normal;

        auto reflected = reflect(-ray.dir, normal);

        double reflcolor[3];

        Ray newray (point + epsilon * reflected, reflected);

        Object::raytrace(newray, reflcolor, level+1);

        for(int c = 0; c < 3; c++){
            seencolor[c] += reflcolor[c] * coefs[REFL];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }

        return t;
    }

private:
    double tileWidth;

    Vector3<double> getNormal(Vector3<double> point) override {
        return Vector3<double>::Z();
    }

    std::array<double, 3> getColorAtPoint(Vector3<double> point) override {
        int i = (point.x - ref_point.x) / tileWidth;
        int j = (point.y - ref_point.y) / tileWidth;

        if((i+j)%2 == 0)
            return {0.0, 0.0, 0.0};
        else
            return {1.0, 1.0, 1.0};
    }
};


bool Object::isInShadow(Ray ray, Vector3<double> point){
    double t = (point - ray.start).length();

    for(auto object : objects){
        // if(object.get() == this)
        //     continue;
        double dummy[3];
        auto t_cand = object->intersect(ray, dummy, 0);
        if(0 < t_cand && t_cand < t-epsilon){
            return true;
        }
    }

    return false;
}


void Object::processColor(Ray& view, Vector3<double> point, double* seencolor) {

    auto original = getColorAtPoint(point);

    seencolor[0] = original[0];
    seencolor[1] = original[1];
    seencolor[2] = original[2];

    auto normal = getNormal(point);
    if(dot(normal, view.dir) > 0)
        normal = -normal;

    for(int c = 0; c<3; c++){
        seencolor[c] *= coefs[AMB];
    }

    for(auto& light : pointLights){
        Ray ray(light.getLightPos(), point - light.getLightPos());

        if(isInShadow(ray, point))
            continue;

        auto reflected = reflect(-ray.dir, normal);

        auto lambertValue = std::max(0.0, dot(normal, -ray.dir));
        auto phongValue = std::max(0.0, dot(reflected, -view.dir));

        for(int c = 0; c < 3; c++){
            seencolor[c] += light.getColor(c) * coefs[DIFF] * lambertValue * original[c];
            seencolor[c] += light.getColor(c) * coefs[SPEC] * pow(phongValue, shine) * original[c];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }
    }

    for(auto& light : spotLights){
        Ray ray(light.getLightPos(), point - light.getLightPos());

        if (acos(dot(ray.dir, light.getLightDir())/light.getLightDir().length()) > light.getCutoffRadians())
            continue;

        if(isInShadow(ray, point)) 
            continue;

        auto reflected = reflect(-ray.dir, normal);

        auto lambertValue = std::max(0.0, dot(normal, -ray.dir));
        auto phongValue = std::max(0.0, dot(reflected, -view.dir));

        for(int c = 0; c < 3; c++){
            seencolor[c] += light.getColor(c) * coefs[DIFF] * lambertValue * original[c];
            seencolor[c] += light.getColor(c) * coefs[SPEC] * pow(phongValue, shine) * original[c];
            if(seencolor[c] > 1) seencolor[c] = 1;
        }
    }
}


void Object::raytrace(Ray& ray, double* color, int level) {
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 0.0;

    std::shared_ptr<Object> nearest;
    double t, tMin = std::numeric_limits<double>::infinity();

    for(auto object : objects){
        double dummycolor[3];
        t = object->intersect(ray, dummycolor, 0);
        if(t > 0 && t <= tMin){
            tMin = t;
            nearest = object;
        }
    }

    if(nearest) 
        nearest->intersect(ray, color, level);

    if(debug){
        std::cout<<color[0]<< " "<<color[1]<<" "<<color[2]<<std::endl;
    }
}