#include <windows.h>
#include <glut.h>

#include "vector.hpp"
#include <iostream>

const double PI = acos(-1);


double cameraHeight;
double cameraAngle;

Vector<float> position;
double directionRotation;
double wheelRotation;

const double direction_delta = 3;
const double wheel_delta = 5;

const double B = 150;
const double R = 30; ///15.91
const double W = 6;
const int SLICE_COUNT = 100;


void drawGrid(){
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINES);{
        glVertex3f(0, 0, B);
        glVertex3f(0, 0, -B);
    }glEnd();

    for(int i = -B; i<=B; i+=10){
        glBegin(GL_LINES);{
            glVertex3f(i, B, 0);
            glVertex3f(i, -B, 0);

            glVertex3f(-B, i, 0);
            glVertex3f(B, i, 0);
        }glEnd();
    }
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

void moveWheel(int k){
    double rad = directionRotation * PI/180.0;

    Vector<float> delta(cos(rad), sin(rad), 0);
    double speed = 2*PI*R * wheel_delta / 360.0;
    delta *= (float)(speed * k);

    position += delta;
    wheelRotation += wheel_delta * k;

    if(wheelRotation < 0) wheelRotation += 360;
    if(wheelRotation >= 360) wheelRotation -= 360;

}

void keyboardListener(unsigned char key, int x,int y){
    

    switch(key){
        case 'w':
            moveWheel(1);
            break;

        case 's':
            moveWheel(-1);
            break;
        
        case 'a':
            directionRotation += direction_delta;
            if(directionRotation >= 360)
                directionRotation -= 360;
            break;
        
        case 'd':
            directionRotation -= direction_delta;
            if(directionRotation <= 0)
                directionRotation += 360;
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

        default:
            break;
    }
}

void drawWheel(){
    
    double x[SLICE_COUNT+1], y[SLICE_COUNT+1];

    for(int i = 0; i<=SLICE_COUNT; i++){
        double angle = 2*PI*i/SLICE_COUNT;
        x[i] = R*cos(angle);
        y[i] = R*sin(angle);
    }

    glColor3f(1.0, 1.0, 0.0);

    for(int i = 0; i<SLICE_COUNT; i++){
        glBegin(GL_QUADS);{
            glVertex3f(x[i], W, y[i]);
            glVertex3f(x[i+1], W, y[i+1]);
            glVertex3f(x[i+1], -W, y[i+1]);
            glVertex3f(x[i], -W, y[i]);
        }glEnd();
    }

    glBegin(GL_QUADS);{
        glVertex3f(R, W, 0);
        glVertex3f(R, -W, 0);
        glVertex3f(-R, -W, 0);
        glVertex3f(-R, W, 0);

        glVertex3f(0, W, R);
        glVertex3f(0, -W, R);
        glVertex3f(0, -W, -R);
        glVertex3f(0, W, -R);
    }glEnd();
}

void drawMovingWheel(){
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(directionRotation, 0, 0, 1);
    glRotatef(wheelRotation, 0, 1, 0);
    drawWheel();
    glPopMatrix();
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

    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,
              0,0,0,
              0,0,1);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawGrid();
    //drawTestCube();
    drawMovingWheel();

    glutSwapBuffers();
}


void animate(){
    glutPostRedisplay();
}

void init(){
    //codes for initialization
    cameraHeight=150.0;
	cameraAngle=1.0;

    position = Vector<float>(0, 0, R);
    directionRotation = 0;
    wheelRotation = 0;

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
