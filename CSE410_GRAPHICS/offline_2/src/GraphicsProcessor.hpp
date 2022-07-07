#pragma once

#include <sstream>
#include <cassert>
#include <stack>
#include <iostream>
#include <vector>
#include <random>

#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "bitmap_image.hpp"

std::mt19937 rng(time(0));

struct Triangle {
    Vector3<float> points[3];
    int rgb[3];

    Triangle(){
        std::uniform_int_distribution<int> dist(0, 255);
        rgb[0] = dist(rng);
        rgb[1] = dist(rng);
        rgb[2] = dist(rng);
    }
};

class GraphicsProcessor{
public:
    GraphicsProcessor(std::istream& _scene, std::istream& _config, std::ostream& _out1, std::ostream& _out2, std::ostream& _out3, std::ostream& _zbuffer_file, bitmap_image& _bmp)
        : scene(_scene), 
        config(_config),
        out1(_out1),
        out2(_out2),
        out3(_out3),
        zbuffer_file(_zbuffer_file),
        bmp(_bmp) {}

    void processTransformations(){
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

    void processDrawing(){
        int width, height;
        float left_lim, right_lim, bottom_lim, top_lim, front_Z, rear_Z;

        config >> width >> height;

        config >> left_lim;
        right_lim = -left_lim;

        config >> bottom_lim;
        top_lim = -bottom_lim;

        config >> front_Z >> rear_Z;

        std::vector<std::vector<float>> zbuffer(width, std::vector<float>(height, rear_Z));

        bmp.setwidth_height(width, height, true);

        float dx = (right_lim - left_lim) / width;
        float dy = (top_lim - bottom_lim) / height;

        float Left_X = left_lim + dx/2;
        float Right_X = right_lim - dx/2;
        float Top_Y = top_lim - dy/2;
        float Bottom_Y = bottom_lim + dy/2;

        auto pixel_to_point_x = [&](int px){
            return Left_X + px * dx;
        };

        auto pixel_to_point_y = [&](int py){
            return Top_Y - py * dy;
        };

        auto point_to_pixel_x = [&](float x){
            return (int) roundf((x - Left_X) / dx);
        };

        auto point_to_pixel_y = [&](float y){
            return (int) roundf((Top_Y - y) / dy);
        };

        for(auto& tri : triangles){
            int min_py = height-1;
            int max_py = 0;

            Vector3<float> normal = cross(tri.points[1] - tri.points[0], tri.points[2] - tri.points[0]);
            float plane_D = dot(normal, tri.points[0]);


            for(int i = 0; i<3; i++){
                int py = point_to_pixel_y(tri.points[i].y);
                min_py = std::min(min_py, py);
                max_py = std::max(max_py, py);
            }

            min_py = std::max(0, min_py);
            max_py = std::min(height-1, max_py);

            for(int py = min_py; py <= max_py; py++){
                float y = pixel_to_point_y(py);
                int min_px = width-1;
                int max_px = 0;

                for(int i = 0; i<3; i++){
                    Vector3<float> a = tri.points[i];
                    Vector3<float> b = tri.points[(i+1) % 3];

                    if((a.y > y && b.y > y) || (a.y < y && b.y < y))
                        continue;

                    if(a.y == b.y){
                        min_px = std::min(min_px, point_to_pixel_x(std::min(a.x, b.x)));
                        max_px = std::max(max_px, point_to_pixel_x(std::max(a.x, b.x)));
                    }
                    else{
                        float x = (a.x - b.x) / (a.y - b.y) * (y - a.y) + a.x;
                        int px = point_to_pixel_x(x);
                        min_px = std::min(min_px, px);
                        max_px = std::max(max_px, px);
                    }
                }

                min_px = std::max(0, min_px);
                max_px = std::min(width-1, max_px);

                float x = pixel_to_point_x(min_px);
                float z = (plane_D - x * normal.x - y * normal.y) / normal.z; // TODO normal.z = 0

                for(int px = min_px; px <= max_px; px++){
                    if(front_Z <= z && z <= rear_Z && z < zbuffer[px][py]){
                        zbuffer[px][py] = z;
                        bmp.set_pixel(px, py, tri.rgb[0], tri.rgb[1], tri.rgb[2]);
                    }
                    z -= dx * normal.x / normal.z;
                }
            }
        }

        for(int py = 0; py < height; py++){
            for(int px = 0; px < width; px++){
                if(zbuffer[px][py] < rear_Z)
                    zbuffer_file << zbuffer[px][py] << " ";
            }
            zbuffer_file << std::endl;
        }

    }

private:
    std::istream& scene;
    std::istream& config;
    std::ostream& out1; 
    std::ostream& out2; 
    std::ostream& out3; 
    std::ostream& zbuffer_file; 
    bitmap_image& bmp;

    std::stack<Matrix4<float>> transformations;
    Matrix4<float> view;
    Matrix4<float> projection;
    std::vector<Triangle> triangles;

    void processTriangle() {
        Triangle tri;

        for(int i = 0; i<3; i++){
            scene >> tri.points[i];
        }

        for(int i = 0; i<3; i++){
            auto modeled = transformations.top() * tri.points[i];
            auto viewed = view * modeled;
            auto projected = projection * viewed;

            out1 << modeled << std::endl;
            out2 << viewed << std::endl;
            out3 << projected << std::endl;

            tri.points[i] = projected;
        }

        out1 << std::endl;
        out2 << std::endl;
        out3 << std::endl;

        triangles.push_back(tri);
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
    }
};
