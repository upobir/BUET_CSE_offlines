#pragma once

#include <sstream>
#include <cassert>
#include <stack>
#include <iostream>

#include "Vector3.hpp"
#include "Matrix4.hpp"

class GraphicsProcessor{
public:
    GraphicsProcessor(std::istream& _scene, std::istream& _config, std::ostream& _out1, std::ostream& _out2, std::ostream& _out3, std::ostream& _zbuffer, std::ostream& _bmp)
        : scene(_scene), 
        config(_config),
        out1(_out1),
        out2(_out2),
        out3(_out3),
        zbuffer(_zbuffer),
        bmp(_bmp) {}

    void process(){
        transformations.push(Matrix4<float>::I());

        auto m = transformations.top();

        Vector3<float> eye, look, up;
        float fovY, aspectRatio, near, far;

        scene >> eye;
        scene >> look;
        scene >> up;
        scene >> fovY >> aspectRatio >> near >> far;

        buildViewMatrix(eye, look, up);
        buildProjection(fovY, aspectRatio, near, far);

        while(true){
            std::string command;

            scene >> command;

            if(command == "triangle"){
                processTriangle();
            }
            else if(command == "translate")
                processTranslate();
            else if(command == "rotate")
                processRotate();
            else if(command == "scale")
                processScale();
            else if(command == "push")
                processPush();
            else if(command == "pop")
                processPop();
            else if(command == "end")
                break;
            else
                assert(!"invalid input");

        }
    }
private:
    std::istream& scene;
    std::istream& config;
    std::ostream& out1; 
    std::ostream& out2; 
    std::ostream& out3; 
    std::ostream& zbuffer; 
    std::ostream& bmp;

    std::stack<Matrix4<float>> transformations;
    Matrix4<float> view;
    Matrix4<float> projection;

    void processTriangle() {
        Vector3<float> p[3];
        for(int i = 0; i<3; i++){
            scene >> p[i];
        }

        for(int i = 0; i<3; i++){
            auto modeled = transformations.top() * p[i];
            auto viewed = view * modeled;
            auto projected = projection * viewed;

            out1 << modeled << std::endl;
            out2 << viewed << std::endl;
            out3 << projected << std::endl;
        }

        out1 << std::endl;
        out2 << std::endl;
        out3 << std::endl;
    }

    void processTranslate() {
        Vector3<float> t;
        scene >> t;

        transformations.top() = transformations.top() * Matrix4<float>::translation(t);
    }

    void processRotate(){
        float angle;
        Vector3<float> a;
        
        scene >> angle;
        scene >> a;

        transformations.top() = transformations.top() * Matrix4<float>::rotation(angle, a);
    }

    void processScale(){
        Vector3<float> s;
        scene >> s;

        transformations.top() = transformations.top() * Matrix4<float>::scale(s);
    }

    void processPush(){
        transformations.push(transformations.top());
    }

    void processPop(){
        transformations.pop();
    }

    void buildViewMatrix(Vector3<float> eye, Vector3<float> look, Vector3<float> up){

        auto l = look - eye;
        l = l.getNormalized();
        auto r = cross(l, up);
        r = r.getNormalized();
        auto u = cross(r, l); 

        auto T = Matrix4<float>::translation(-eye);

        Matrix4<float> R({
            { r.x,  r.y,  r.z, 0.0},
            { u.x,  u.y,  u.z, 0.0},
            {-l.x, -l.y, -l.z, 0.0},
            { 0.0,  0.0,  0.0, 1.0}
        });

        view = R * T;
    }

    void buildProjection(float fovY, float aspectRatio, float near, float far){
        float fovX = fovY * aspectRatio;
        float t = near * tan(fovY / 2 * PI / 180.0);
        float r = near * tan(fovX / 2 * PI / 180.0);

        projection = Matrix4<float>({
            {near/r, 0.0, 0.0, 0.0},
            {0.0, near/t, 0.0, 0.0},
            {0.0, 0.0, -(far + near)/(far - near), -(2*far*near)/(far - near)},
            {0.0, 0.0, -1.0, 0.0},
        });

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                std::cout<<projection.at(i, j)<<" ";
            }
            std::cout<<std::endl;
        }
    }
};
