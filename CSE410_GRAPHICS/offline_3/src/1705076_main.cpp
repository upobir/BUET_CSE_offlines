#include <windows.h>
#include <glut.h>

#include "1705076_vector3.hpp"
#include "1705076_objects.hpp"
#include "1705076_camera.hpp"
#include "1705076_lights.hpp"

#include "bitmap_image.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <cassert>

template <typename T>
using SP = std::shared_ptr<T>;

Camera camera;
std::vector<SP<Object>> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

int recursionLevelMax;
int pixelDimension;

int captureCount = 0;

void capture(){
    bitmap_image image;

    image.setwidth_height(pixelDimension, pixelDimension, true);

    captureCount++;
    std::string filename = std::string("output_") + std::to_string(captureCount) + std::string(".bmp");

    image.save_image(filename);

    std::cout<<"Captured "<<filename<<std::endl;
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case '1':
            camera.lookLeft();
            break;

        case '2':
            camera.lookRight();
            break;

        case '3':
            camera.lookUp();
            break;

        case '4':
            camera.lookDown();
            break;

        case '5':
            camera.tiltClockwise();
            break;

        case '6':
            camera.tiltCounterclockwise();
            break;

        case ' ':
            capture();
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            camera.moveBack();
            break;

        case GLUT_KEY_UP:
            camera.moveFront();
            break;

        case GLUT_KEY_RIGHT:
            camera.moveRight();
            break;

        case GLUT_KEY_LEFT:
            camera.moveLeft();
            break;

        case GLUT_KEY_PAGE_UP:
            camera.moveUp();
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera.moveDown();
            break;

        default:
            break;
    }
}


void display(){

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);    //color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    camera.lookAt();

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    //drawAxes();
    //drawTestCube();

    for(auto object : objects){
        object->draw();
    }

    for(auto& light : pointLights){
        light.draw();
    }

    for(auto& light : spotLights){
        light.draw();
    }

    glutSwapBuffers();
}


void animate(){
    glutPostRedisplay();
}


void init(){
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80.0, 1.0, 1.0, 1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}


void loadData(){
    std::ifstream sceneFile("scene.txt");

    sceneFile >> recursionLevelMax;
    sceneFile >> pixelDimension;

    int objectCount;
    sceneFile >> objectCount;

    for(int i = 0; i<objectCount; i++){
        std::string type;
        sceneFile >> type;
        SP<Object> tmp;

        if(type == "sphere"){
            tmp = std::make_shared<Sphere>(Vector3<double>{}, 0);
            tmp->readFromStream(sceneFile);
        }
        else if(type == "triangle"){
            tmp = std::make_shared<Triangle>(Vector3<double>{}, Vector3<double>{}, Vector3<double>{});
            tmp->readFromStream(sceneFile);
        }
        else if(type == "general"){
            tmp = std::make_shared<QuadSurface>();
            tmp->readFromStream(sceneFile);
        }
        else{
            assert(!"invalid input");
        }

        objects.push_back(tmp);
    }

    SP<Object> floor = std::make_shared<Floor>(1000, 20);
    objects.push_back(floor);

    int pointLightCount, spotLightCount;

    sceneFile >> pointLightCount;
    for(int i = 0; i<pointLightCount; i++){
        PointLight light;
        light.readFromStream(sceneFile);
        pointLights.push_back(light);
    }

    sceneFile >> spotLightCount;
    for(int i = 0; i<spotLightCount; i++){
        SpotLight light;
        light.readFromStream(sceneFile);
        spotLights.push_back(light);
    }
    
    sceneFile.close();

    std::cout<<"Scene read finished "<<std::endl;
    std::cout<<"object count: "<<objects.size()<<std::endl;
    std::cout<<"pointlight count: "<<pointLights.size()<<std::endl;
    std::cout<<"spotlight count: "<<spotLights.size()<<std::endl;
    std::cout<<std::endl;
}


int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color
    glutCreateWindow("RayTracer");

    loadData();

    init();

    glEnable(GL_DEPTH_TEST);    //enable Depth Testing

    glutDisplayFunc(display);    //display callback function
    glutIdleFunc(animate);       //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();        //The main loop of OpenGL

    return 0;
}
