#include <windows.h>
#include <glut.h>

#include "vector.hpp"
#include <iostream>

const double PI = acos(-1);

Vector<double> up, right, look, camera;
const double move_delta = 10.0;
const double angle_delta = 5 * PI/180.0;

const double B = 50, X_delta = 2;
double X = 25;

const int SLICE_COUNT = 100;

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

void drawUnitSquare(){
    glBegin(GL_QUADS);{
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
    }glEnd();
}

void drawUnitSphereOneEighth(){
    Vector<float> cur[SLICE_COUNT+1], old[SLICE_COUNT+1];

    for(int i = 0; i <= SLICE_COUNT; i++){
        old[i] = Vector<float>((float)cos(PI/2.0*i/SLICE_COUNT), (float)sin(PI/2.0*i/SLICE_COUNT), 0);
    }

    for(int j = 1; j<=SLICE_COUNT; j++){
        double z = j/(double)SLICE_COUNT;
        double r = sqrt(1-z*z);
        for(int i = 0; i <= SLICE_COUNT; i++){
            cur[i] = Vector<float>((float)cos(PI/2.0*i/SLICE_COUNT)*r, (float)sin(PI/2.0*i/SLICE_COUNT)*r, z);
        }
        
        for(int i = 0; i<SLICE_COUNT; i++){
            glBegin(GL_QUADS);{
                glVertex3f(old[i].x, old[i].y, old[i].z);
                glVertex3f(old[i+1].x, old[i+1].y, old[i+1].z);
                glVertex3f(cur[i+1].x, cur[i+1].y, cur[i+1].z);
                glVertex3f(cur[i].x, cur[i].y, cur[i].z);
            }glEnd();
        }

        std::swap(cur, old);
    }
}

void drawUnitCylinderOneFourth(){
    double x[SLICE_COUNT+1], y[SLICE_COUNT+1];

    for(int i = 0; i<=SLICE_COUNT; i++){
        x[i] = cos(PI/2.0 * i/SLICE_COUNT);
        y[i] = sin(PI/2.0 * i/SLICE_COUNT);
    }

    for(int i = 0; i<SLICE_COUNT; i++){
        glBegin(GL_QUADS);{
            glVertex3f(x[i], y[i], 1);
            glVertex3f(x[i], y[i], -1);
            glVertex3f(x[i+1], y[i+1], -1);
            glVertex3f(x[i+1], y[i+1], 1);
        }glEnd();
    }
}

void drawSquares(){
    
    glColor3f(1.0, 0.0, 0.0);

    // back face
    glPushMatrix();
    glTranslatef(0.0, 0.0, -B);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();

    // front face
    glPushMatrix();
    glTranslatef(0.0, 0.0, B);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();

    // right face
    glPushMatrix();
    glTranslatef(B, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();

    // left face
    glPushMatrix();
    glTranslatef(-B, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();

    // up face
    glPushMatrix();
    glTranslatef(0.0, B, 0.0);
    glRotatef(90, 1, 0, 0);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();

    // down face
    glPushMatrix();
    glTranslatef(0.0, -B, 0.0);
    glRotatef(90, 1, 0, 0);
    glScalef(X, X, 0);
    drawUnitSquare();
    glPopMatrix();
}


void drawSpheres(){
    glColor3f(0.0, 1.0, 0.0);
    
    /// top right front
    glPushMatrix();
    glTranslatef(X, X, X);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// top right back
    glPushMatrix();
    glTranslatef(X, X, -X);
    glRotatef(90, 0, 1, 0);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// top left front
    glPushMatrix();
    glTranslatef(-X, X, X);
    glRotatef(270, 0, 1, 0);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// top left back
    glPushMatrix();
    glTranslatef(-X, X, -X);
    glRotatef(180, 0, 1, 0);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// bottom right front
    glPushMatrix();
    glTranslatef(X, -X, X);
    glRotatef(270, 0, 0, 1);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// bottom right back
    glPushMatrix();
    glTranslatef(X, -X, -X);
    glRotatef(180, 1, 0, 0);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// bottom left back
    glPushMatrix();
    glTranslatef(-X, -X, -X);
    glScalef(-(B-X), -(B-X), -(B-X));
    drawUnitSphereOneEighth();
    glPopMatrix();

    /// bottom left front
    glPushMatrix();
    glTranslatef(-X, -X, X);
    glRotatef(180, 0, 0, 1);
    glScalef(B-X, B-X, B-X);
    drawUnitSphereOneEighth();
    glPopMatrix();
}

void drawCylinders(){
    glColor3f(0.0, 0.0, 1.0);
    
    // top right
    glPushMatrix();
    glTranslatef(X, X, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // top left
    glPushMatrix();
    glTranslatef(-X, X, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // bottom left
    glPushMatrix();
    glTranslatef(-X, -X, 0);
    glRotatef(180, 0, 0, 1);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // bottom right
    glPushMatrix();
    glTranslatef(X, -X, 0);
    glRotatef(270, 0, 0, 1);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // top back
    glPushMatrix();
    glTranslatef(0, X, -X);
    glRotatef(90, 0, 1, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // top front
    glPushMatrix();
    glTranslatef(0, X, X);
    glRotatef(270, 0, 1, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // bottom back
    glPushMatrix();
    glTranslatef(0, -X, -X);
    glRotatef(270, 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // bottom front
    glPushMatrix();
    glTranslatef(0, -X, X);
    glRotatef(180, 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // front right
    glPushMatrix();
    glTranslatef(X, 0, X);
    glRotatef(90, 1, 0, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // front left
    glPushMatrix();
    glTranslatef(-X, 0, X);
    glRotatef(-90, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // back right
    glPushMatrix();
    glTranslatef(X, 0, -X);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();

    // back left
    glPushMatrix();
    glTranslatef(-X, 0, -X);
    glRotatef(180, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(B-X, B-X, X);
    drawUnitCylinderOneFourth();
    glPopMatrix();
}

void drawObject(){
    drawSquares();
    drawSpheres();
    drawCylinders();
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case '1':
            look = rotate(up, look, +angle_delta);
            right = rotate(up, right, +angle_delta);
            break;

        case '2':
            look = rotate(up, look, -angle_delta);
            right = rotate(up, right, -angle_delta);
            break;

        case '3':
            look = rotate(right, look, +angle_delta);
            up = rotate(right, up, +angle_delta);
            break;

        case '4':
            look = rotate(right, look, -angle_delta);
            up = rotate(right, up, -angle_delta);
            break;

        case '5':
            up = rotate(look, up, +angle_delta);
            right = rotate(look, right, +angle_delta);
            break;

        case '6':
            up = rotate(look, up, -angle_delta);
            right = rotate(look, right, -angle_delta);
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            camera -= move_delta * look;
            break;

        case GLUT_KEY_UP:
            camera += move_delta * look;
            break;

        case GLUT_KEY_RIGHT:
            camera += move_delta * right;
            break;

        case GLUT_KEY_LEFT:
            camera -= move_delta * right;
            break;

        case GLUT_KEY_PAGE_UP:
            camera += move_delta * up;
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera -= move_delta * up;
            break;

        case GLUT_KEY_HOME:
            X -= X_delta;
            if(X < 0) X = 0;
            break;

        case GLUT_KEY_END:
            X += X_delta;
            if(X > B) X = B;
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

    Vector<double> look_point = camera + look;

    gluLookAt(camera.x, camera.y, camera.z
              , look_point.x, look_point.y, look_point.z
              , up.x, up.y, up.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    //drawTestCube();

    drawObject();

    glutSwapBuffers();
}


void animate(){
    glutPostRedisplay();
}

void init(){
    //codes for initialization

    camera = {0,0,200};
    up = Vector<double>::Y();
    look = -Vector<double>::Z();
    right = Vector<double>::X();

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

int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color
    glutCreateWindow("Task 1");

    init();

    glEnable(GL_DEPTH_TEST);    //enable Depth Testing

    glutDisplayFunc(display);    //display callback function
    glutIdleFunc(animate);       //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();        //The main loop of OpenGL

    return 0;
}
