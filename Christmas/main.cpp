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
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

class Vector3f {
public:
    float x, y, z;

    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator+(Vector3f &v) {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f operator-(Vector3f &v) {
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
        eye = (eye + right) * d;
        center = (center + right) * d;
    }

    void moveY(float d) {
        Vector3f zk= up.unit();
        eye = (eye + zk) * d;
        center = (center + zk) * d;
    }

    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        eye = (eye + view) * d;
        center = (center + view) * d;
    }

    void rotateX(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        Vector3f cc =view * cos(DEG2RAD(a));
        Vector3f ss = up * sin(DEG2RAD(a));
        view = cc+ss;
        up = view.cross(right);
        center = eye + view;
    }

    void rotateY(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        Vector3f cc =view * cos(DEG2RAD(a));
        Vector3f ss = right * sin(DEG2RAD(a));
        view = cc+ss;
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
//void drawTableLeg(double thick, double len) {
//    glPushMatrix();
//    glTranslated(0, len / 2, 0);
//    glScaled(thick, len, thick);
//    glutSolidCube(1.0);
//    glPopMatrix();
//}

//void drawTable(double topWid, double topThick, double legThick, double legLen){
//    glPushMatrix();
//    glTranslated(0, legLen, 0);
//    glScaled(topWid, topThick, topWid);
//    glutSolidCube(1.0);
//    glPopMatrix();
//
//    double dist = 0.95*topWid / 2.0 - legThick / 2.0;
//    glPushMatrix();
//    glTranslated(dist, 0, dist);
//    drawTableLeg(legThick, legLen);
//    glTranslated(0, 0, -2 * dist);
//    drawTableLeg(legThick, legLen);
//    glTranslated(-2 * dist, 0, 2 * dist);
//    drawTableLeg(legThick, legLen);
//    glTranslated(0, 0, -2 * dist);
//    drawTableLeg(legThick, legLen);
//    glPopMatrix();
//}

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
    GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 640 / 480, 0.001, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
}

void drawSnowMan(float x, float y, float z) {

glPushMatrix();

 glColor3f(1.0f, 1.0f, 1.0f);
// Draw Body
glPushMatrix();
glTranslatef(x, y-0.4f, z);
glutSolidSphere(0.33f,20,20);
glPopMatrix();
    
glPushMatrix();
glTranslatef(x, y+0.25f, z);
glColor3f(1.0f, 0.0f , 0.0f);
glRotatef(90, 1, 0, 0);
glutSolidTorus(0.05f, 0.09f, 20, 20);
glPopMatrix();
    
glPushMatrix();
glColor3f(1.0f, 1.0f , 1.0f);
glTranslatef(x ,y, z);
glutSolidSphere(0.20f,30,20);
glPopMatrix();
    
// Draw Head
 glPushMatrix();
 glTranslatef(x, y+0.35f, z);
 glutSolidSphere(0.10f,20,20);
 glPopMatrix();

// Draw Hat
glPushMatrix();
glTranslatef(x+0.18, y+0.52, 0.18f);
glColor3f(1.0f, 0.0f , 0.0f);
glRotated(-90,1, 0, 0);
glutSolidCone(0.055f,0.15f,10,2);
glPopMatrix();
        
glPushMatrix();
glColor3f(1.0f, 1.0f , 1.0f);
glTranslatef(x+0.18, y+0.65f, 0.18f);
glutSolidSphere(0.03f,20,20);
glRotated(50,0, 1, 0);
glPopMatrix();
    
//Draw Nose
glPushMatrix();
glColor3f(1.0f, 0.0f , 0.0f);
glTranslatef(x+0.18, y+0.45f, 0.18f);
glRotatef(45.0f,0.0f, 1.0f, 0.0f);
glutSolidCone(0.01f,0.18f,10,2);
glPopMatrix();
    
    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(x-0.15f, y+0.3f, 0);
    glutSolidSphere(0.01f,10,10);
    glPopMatrix();
//glPushMatrix();
//glTranslatef(x+0.18, 0.47f, 0);
//glutSolidSphere(0.01f,10,10);
//glPopMatrix();
    
glPopMatrix();
}

void Display() {
    setupCamera();
    setupLights();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   
    glPushMatrix();
//    glTranslated(0.6, 0.38, 0.5);
//    glRotated(30, 0, 1, 0);
//    glutSolidTeapot(0.08);
//    glPopMatrix();
//    glPushMatrix();
//    glTranslated(0.25, 0.42, 0.35);
//    glutSolidSphere(0.1, 15, 15);
//    glPopMatrix();
    drawSnowMan(0.0f,0.0f,0.0f);
//    glPushMatrix();
//    glTranslated(0.4, 0.0, 0.4);
//    drawTable(0.6, 0.02, 0.02, 0.3);
//    glPopMatrix();
//    drawWall(0.02);
    glPushMatrix();
//    glRotated(0, 0, 1.0, 0);
//    drawWall(0.02);
//    glPopMatrix();
//    glPushMatrix();
//    glRotated(-90, 1.0, 0.0, 0.0);
//    drawWall(0.02);
//    glPopMatrix();

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1240, 680);
    glutInitWindowPosition(60, 90);

    glutCreateWindow("Lab 5");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);

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
