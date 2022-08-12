#include <windows.h>
#include <glut.h>

#include "1705076_Vector3.hpp"
#include "1705076_objects.hpp"
#include <iostream>
#include <fstream>
#include <memory>

template <typename T>
using SP = std::shared_ptr<T>;

Camera camera;

void drawAxes()
{
    glBegin(GL_LINES);{
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f( 100,0,0);
        glVertex3f(-100,0,0);

        glVertex3f(0,-100,0);
        glVertex3f(0, 100,0);

        glVertex3f(0,0, 100);
        glVertex3f(0,0,-100);
    }glEnd();
}

void drawTestCube(){
    glBegin(GL_QUADS);{
        glColor3f(1.0, 0.0, 0.0);

        glVertex3f(50.0, 50.0, 50.0);
        glVertex3f(-50.0, 50.0, 50.0);
        glVertex3f(-50.0, -50.0, 50.0);
        glVertex3f(50.0, -50.0, 50.0);

        glVertex3f(50.0, 50.0, -50.0);
        glVertex3f(50.0, -50.0, -50.0);
        glVertex3f(-50.0, -50.0, -50.0);
        glVertex3f(-50.0, 50.0, -50.0);

        glColor3f(0.0, 1.0, 0.0);

        glVertex3f(-50.0, 50.0, 50.0);
        glVertex3f(-50.0, 50.0, -50.0);
        glVertex3f(-50.0, -50.0, -50.0);
        glVertex3f(-50.0, -50.0, 50.0);

        glVertex3f(50.0, 50.0, -50.0);
        glVertex3f(50.0, 50.0, 50.0);
        glVertex3f(50.0, -50.0, 50.0);
        glVertex3f(50.0, -50.0, -50.0);

        glColor3f(0.0, 0.0, 1.0);

        glVertex3f(50.0, 50.0, 50.0);
        glVertex3f(50.0, 50.0, -50.0);
        glVertex3f(-50.0, 50.0, -50.0);
        glVertex3f(-50.0, 50.0, 50.0);

        glVertex3f(50.0, -50.0, 50.0);
        glVertex3f(-50.0, -50.0, 50.0);
        glVertex3f(-50.0, -50.0, -50.0);
        glVertex3f(50.0, -50.0, -50.0);
        
    }glEnd();
}


void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case '1':
            camera.look_left();
            break;

        case '2':
            camera.look_right();
            break;

        case '3':
            camera.look_up();
            break;

        case '4':
            camera.look_down();
            break;

        case '5':
            camera.tilt_clockwise();
            break;

        case '6':
            camera.tilt_counterclockwise();
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            camera.move_back();
            break;

        case GLUT_KEY_UP:
            camera.move_front();
            break;

        case GLUT_KEY_RIGHT:
            camera.move_right();
            break;

        case GLUT_KEY_LEFT:
            camera.move_left();
            break;

        case GLUT_KEY_PAGE_UP:
            camera.move_up();
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera.move_down();
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
    drawTestCube();

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
    /// TODO
    sceneFile.close();
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
