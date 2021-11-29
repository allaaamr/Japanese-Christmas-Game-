#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
using namespace std;
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

double x_position=0;
double z_position =0;
double angle=0;
double gift1_y = 0;
double gift2_y = 0;
double gift3_y = 0;
int remaining_gifts = 3;
int c;

class Vector3f {
public:
    float x, y, z;

    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator+(Vector3f v) {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f operator-(Vector3f v) {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f operator*(float n) {
        return Vector3f(x * n, y * n, z * n);
    }

    Vector3f operator/(float n) {
        return Vector3f(x / n, y / n, z / n);
    }

    Vector3f unit() {
        return *this / sqrt(x * x + y * y + z * z);
    }

    Vector3f cross(Vector3f v) {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

class Camera {
public:
    Vector3f eye, center, up;

    Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
        eye = Vector3f(eyeX, eyeY, eyeZ);
        center = Vector3f(centerX, centerY, centerZ);
        up = Vector3f(upX, upY, upZ);
    }

    void moveX(float d) {
        Vector3f right = up.cross(center - eye).unit();
        eye = eye + right * d;
        center = center + right * d;
    }

    void moveY(float d) {
        eye = eye + up.unit() * d;
        center = center + up.unit() * d;
    }

    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        eye = eye + view * d;
        center = center + view * d;
    }

    void rotateX(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
        up = view.cross(right);
        center = eye + view;
    }

    void rotateY(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
        right = view.cross(up);
        center = eye + view;
    }

    void look() {
        gluLookAt(
            eye.x, eye.y, eye.z,
            center.x, center.y, center.z,
            up.x, up.y, up.z
        );
    }
   
    
    
    
};

Camera camera;

void drawWall(double thickness) {
    glPushMatrix();
    glTranslated(0.5, 0.5 * thickness, 0.5);
    glScaled(1.0, thickness, 1.0);
    glutSolidCube(1);
    glPopMatrix();
}

void setupLights() {
    GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
    GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat shininess[] = { 50 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
    GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 640 / 480, 0.0001, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
}

void Floor(){
    glColor3f(0.3f, 0.0f, 0.0f);
    glPushMatrix();
    glScaled(10, 0.3, 10);
    glRotated(45, 0, 1, 0);
    glutSolidCube(0.2);
    glPopMatrix();
}

void Gift(float red, float green, float blue){
glPushMatrix();
    
glPushMatrix();
glColor3f(red, 0.3, 0.2);
glTranslatef(0, 0.1, 0);
glRotated(45,0, 1, 0);
glutSolidCube(0.2);
glPopMatrix();

glPushMatrix();
glColor3f(red, green, 0.0f);
glTranslatef(0, 0.14f, 0);
glRotated(45,0, 1, 0);
glScalef(1.08, 1.05, 0.08);
glutSolidCube(0.2);
glPopMatrix();
    
glPushMatrix();
glColor3f(red, green, 0.0f);
glTranslatef(0, 0.14f, 0);
glRotated(-45,0, 1, 0);
glScalef(1.08, 1.05, 0.08);
glutSolidCube(0.2);
glPopMatrix();
    
glPushMatrix();
glColor3f(red, 0.0f, blue);
glTranslatef(0, 0.26f, 0);
glutSolidSphere(0.02, 5, 5);
glPopMatrix();
    
glPopMatrix();

}

void Presents(){
glPushMatrix();
glTranslatef(0.7, gift1_y, 0);
Gift(1,1,1);
glPopMatrix();
    
glPushMatrix();
glTranslatef(-0.3, gift2_y, 0.4);
Gift(0,1.3,1.5);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.2, gift3_y, 0.7);
Gift(0.8,1.3,0);
glPopMatrix();
}

void DetectCollision(){
    
    //Check Collision with 1st gift
    if (0.6<=x_position &&  x_position<=0.8 &&  -0.1<=z_position && z_position<=0.1){
        cout << x_position ;
       gift1_y+=10;
        remaining_gifts--;
    }
    //Check Collision with 2nd gift
    if (-0.4<=x_position && x_position<=-0.2 &&  0.3<=z_position && z_position<=0.5){
       gift2_y+=10;
        remaining_gifts--;
    }
    //Check Collision with 3rd gift
    if (-0.3<=x_position && x_position<=-0.1 &&  0.6<=z_position && z_position<=0.8){
       gift3_y+=10;
        remaining_gifts--;
    }
}

void Character(){
    glPushMatrix();
    glColor3f(1, 0, 0);
    
    //Legs
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslated(0.05, 0.1, 0.05);
    glScalef(1, 10, 1);
    glRotatef(45,0,1,0);
    glutSolidCube(0.05);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslated(-0.05, 0.1, 0.05);
    glScalef(1, 10, 1);
    glRotatef(45,0,1,0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    //Body
    glPushMatrix();
    GLUquadricObj* qobj;
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glTranslated(0, 0.8, 0);
    glRotated(90, 1, 0, 0);
    gluCylinder(qobj, 0.07, 0.2, 0.55, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(0, 0.25, 0);
    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.03, 0.19, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.7, 0, 0);
    glTranslated(0, 0.5, 0);
    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.028, 0.13, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(0, 0.82, 0);
    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.03, 0.05, 10, 10);
    glPopMatrix();
    
    
    //Face
    glPushMatrix();
    glColor3f(1, 1, 0.7);
    glTranslated(0, 0.9, 0);
    glScaled(1, 1.3, 1);
    glutSolidSphere(0.07, 10, 10);
    glPopMatrix();
    
    
    //Hat
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(0, 0.97, 0);
    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.02, 0.04, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.97, 0);
    glColor3f(1.0f, 0.0f , 0.0f);
    glRotated(-90,1, 0, 0);
    glutSolidCone(0.055f,0.15f,10,2);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);
    glTranslatef(0, 1.12, 0);
    glutSolidSphere(0.02f,10,10);
    glPopMatrix();
    
    // Eyes
    glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(0.052, 0.9, 0.022);
    glutSolidSphere(0.017f,10,10);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(0.022, 0.9, 0.052);
    glutSolidSphere(0.017f,10,10);
    glPopMatrix();
    
    //Arms
    glPushMatrix();
    glColor3f(1.0f,0,0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glTranslated(-0.08, 0.7, 0);
    glRotated(45, 1, 0, 1);
    gluCylinder(qobj, 0.025, 0.025, 0.25, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f,0,0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glTranslated(0.04, 0.72, 0.003);
    glRotated(45, 1, 0, 1);
    glRotated(90, 1, 0, 0);
    gluCylinder(qobj, 0.025, 0.025, 0.25, 20, 20);
    glPopMatrix();
    
    glPopMatrix();
    
}

void Lantern(){
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslated(0, 0.3, 0);
    glScaled(1, 0.9, 1);
    glutSolidSphere(0.17, 10, 10);
    glColor3f(0.0f,0.0f, 0.0f);
    GLUquadricObj* qobj;
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glTranslated(0, 0.3, 0);
    glRotated(90, 1, 0, 0);
    gluCylinder(qobj, 0.05, 0.05, 0.55, 20, 20);
    glPopMatrix();

}

void Lanterns(){
    glPushMatrix();
    glTranslated(1.1, 0, 0.2);
    Lantern();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.2, 0, 1.1);
    Lantern();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.3, 0, -0.3);
    Lantern();
    glPopMatrix();
}

void Tree(){
    
// Tree 1
glPushMatrix();
glColor3f(0.0f, 0.6f, 0.0f);

glPushMatrix();
glTranslatef(0.15f, 0.13f, -0.5);
glRotated(-90,1, 0, 0);
glutSolidCone(0.3f,0.7f,10,2);
glPopMatrix();

glPushMatrix();
glTranslatef(0.15f, 0.55f, -0.5);
glRotated(-90,1, 0, 0);
glutSolidCone(0.22f,0.5f,10,2);
glPopMatrix();
    
glPushMatrix();
glTranslatef(0.15f, 0.85f, -0.5);
glRotated(-90,1, 0, 0);
glutSolidCone(0.14f,0.5f,10,2);
glPopMatrix();
    
 // Tree 1 Christmas Decoration
glPushMatrix();
glColor3f(1.0f, 0.0f, 1.0f);
glTranslatef(-0.2f, 0.185f, -0.3);
glTranslatef(0.56, 0.61f, 0.195);
glutSolidSphere(0.01f,10,10);
glPopMatrix();
        
    
glPushMatrix();
glColor3f(0.0f, 0.6f, 0.0f);

glPushMatrix();
glTranslatef(-0.5f, 0.13f, 0.15);
glRotated(-90,1, 0, 0);
glutSolidCone(0.3f,0.7f,10,2);
glPopMatrix();

glPushMatrix();
    glTranslatef(-0.5f, 0.55f, 0.15);
glRotated(-90,1, 0, 0);
glutSolidCone(0.22f,0.5f,10,2);
glPopMatrix();
    
glPushMatrix();
glTranslatef(-0.5f, 0.86f, 0.15);
glRotated(-90,1, 0, 0);
glutSolidCone(0.14f,0.5f,10,2);
glPopMatrix();
    
}

void drawSnowMan() {

//glPushMatrix();

 glColor3f(1.0f, 1.0f, 1.0f);


// Draw Body
glPushMatrix();
glTranslatef(0.5, 0.4f, 0.1);
glScaled(1, 1.3, 1);
glutSolidSphere(0.1f,20,20);
glPopMatrix();

glPushMatrix();
glColor3f(1.0f, 1.0f , 1.0f);
glTranslatef(0.5 ,0.58, 0.13);
glutSolidSphere(0.065f,30,20);
glPopMatrix();
    
glPushMatrix();
glColor3f(0.0f,0.0f,0.0f);
glTranslatef(0.56, 0.58f, 0.195);
glutSolidSphere(0.01f,10,10);
glPopMatrix();
    
glPushMatrix();
glColor3f(0.0f,0.0f,0.0f);
glTranslatef(0.56, 0.61f, 0.195);
glutSolidSphere(0.01f,10,10);
glPopMatrix();
    
   
// Draw Scarf
glPushMatrix();
glTranslatef(0.5, 0.65f, 0.14);
glColor3f(1.0f, 0.0f , 0.0f);
glRotatef(90, 1, 0, 0);
glutSolidTorus(0.02f, 0.03f, 20, 20);
glPopMatrix();

// Draw Head
 glColor3f(1.0f, 1.0f, 1.0f);
 glPushMatrix();
 glTranslatef(0.5, 0.7f, 0.14);
 glutSolidSphere(0.04f,20,20);
 glPopMatrix();

//Draw Hat
glPushMatrix();
glTranslatef(0.5f, 0.73f, 0.14);
glColor3f(1.0f, 0.0f , 0.0f);
glRotated(-90,1, 0, 0);
glutSolidCone(0.03f,0.06f,10,2);
glPopMatrix();
    
glPushMatrix();
glColor3f(1.0f,1.0f,1.0f);
glTranslatef(0.5, 0.8f, 0.14);
glutSolidSphere(0.01f,10,10);
glPopMatrix();


//Draw Nose
glPushMatrix();
glColor3f(1.0f, 0.0f , 0.0f);
glTranslatef(0.5, 0.7f, 0.14);
glRotatef(45.0f,0.0f, 1.0f, 0.0f);
glutSolidCone(0.01f,0.1f,10,2);
glPopMatrix();

// Draw Eyes
//glPushMatrix();
//glColor3f(0.0f,0.0f,0.0f);
//glTranslatef(0.3, 0.71f, 0.14);
//glutSolidSphere(0.01f,10,10);
//glPopMatrix();

//glPushMatrix();
//glTranslatef(0.5, 0.8f, 0.14);
//glutSolidSphere(0.01f,10,10);
//glPopMatrix();

glPopMatrix();
}

void LeftFence(){
    glColor3f(0.6f, 0.0f, 0.0f);
    double z= 0.4;
    double x=-0.85;
    for( int i=0; i<7; i++ ,z+=0.15, x+=0.15 ){
    glPushMatrix();
    glTranslated(x, 0.3, z);
    glScaled(0.1, 1, 0.1);
    glRotated(45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
}
    
    glPushMatrix();
    glTranslated(-0.66, 0.4, 0.55);
    glRotated(45, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    glScaled(0.1, 4, 0.1);
    glutSolidCube(0.5);
    glPopMatrix();
}

void RightFence(){
    glColor3f(0.6f, 0.0f, 0.0f);
    double z= -0.85;
    double x=0.35;
    for( int i=0; i<7; i++ ,z+=0.15, x+=0.15 ){
    glPushMatrix();
    glTranslated(x, 0.3, z);
    glScaled(0.1, 1, 0.1);
//    glRotated(90, 1, 0, 1);
    glRotated(45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
}
    glPushMatrix();
    glTranslated(0.6, 0.4, -0.55);
    glRotated(45, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    glScaled(0.1, 3.7, 0.1);
    glutSolidCube(0.5);
    glPopMatrix();
}

void BackFence(){
    glColor3f(0.6f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslated(-1.15, 0.3, 0.1);
    glScaled(0.18, 1, 0.18);
//    glRotated(90, 1, 0, 1);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.95, 0.3, -0.1);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.75, 0.3, -0.3);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.55, 0.3, -0.5);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.35, 0.3, -0.7);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.15, 0.3, -0.9);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.05, 0.3, -1.1);
    glScaled(0.18, 1, 0.18);
    glRotated(-45, 0, 1, 0);
    glutSolidCube(0.6);
    glPopMatrix();
    
    
    glTranslated(-0.5, 0.3, -0.45);
    glRotated(-45, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    glScaled(0.1, 4, 0.1);
    glutSolidCube(0.5);
    glPopMatrix();
    
 
}

void Display() {
    setupCamera();
    setupLights();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(c>=2 && remaining_gifts>0){
        gift1_y +=0.1;
        gift2_y +=0.1;
        gift3_y +=0.1;
    }
    DetectCollision();
    Floor();
    
    glPushMatrix();
    glTranslated(x_position, 0, z_position);
    glRotatef(angle,0,1,0);
    Character();
    glPopMatrix();
    
    Tree();
    
    glPushMatrix();
    glTranslated(0, -0.22, 0);
    drawSnowMan();
    glPopMatrix();
    
    
    Presents();
    Lanterns();
    LeftFence();
    RightFence();
    BackFence();
   


    glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
    float d = 0.01;

    switch (key) {
    case 'w':
        camera.moveY(d);
        break;
    case 's':
        camera.moveY(-d);
        break;
    case 'a':
        camera.moveX(d);
        break;
    case 'd':
        camera.moveX(-d);
        break;
    case 'q':
        camera.moveZ(d);
        break;
    case 'e':
        camera.moveZ(-d);
        break;
    case 'i':
            x_position+=0.01;
            z_position+=0.01;
            angle=0;
        break;
    case 'k':
            
            x_position-=0.01;
            z_position-=0.01;
            angle=180;
        break;
    case 'l':
            x_position+=0.01;
            z_position-=0.01;
            angle=90;
        break;
    case 'j':
            x_position-=0.01;
            z_position+=0.01;
            angle=-90;
        break;
    case '1':
            camera.eye = Vector3f(1.3,0,-1.2);
//            camera.moveY(1);
        break;
    case '2':
            camera.eye = Vector3f(-1.2,0,1.3);
//            camera.moveY(1);
        break;
    case '3':
        camera.moveX(d);
            camera.eye = Vector3f(0,1,0);
            camera.moveY(0.8);
        break;
    case '4':
        camera.moveX(-d);
        break;

    case GLUT_KEY_ESCAPE:
        exit(EXIT_SUCCESS);
    }

    glutPostRedisplay();
}

void Special(int key, int x, int y) {
    float a = 1.0;

    switch (key) {
    case GLUT_KEY_UP:
        camera.rotateX(a);
        break;
    case GLUT_KEY_DOWN:
        camera.rotateX(-a);
        break;
    case GLUT_KEY_LEFT:
        camera.rotateY(a);
        break;
    case GLUT_KEY_RIGHT:
        camera.rotateY(-a);
        break;
    }

    glutPostRedisplay();
}

void time(int val)
{
    c++;
    glutPostRedisplay();
    glutTimerFunc(10000,time,0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1240, 680);
    glutInitWindowPosition(60, 90);

    glutCreateWindow("Lab 5");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    glutTimerFunc(0,time,0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
    
    return 0;
}
