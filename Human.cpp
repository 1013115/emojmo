#include <Windows.h>
#include <stdlib.h> 
#include <math.h> 
#include <stdio.h>
#include <GL\GLU.h>
#include <glut.h>
#include <glaux.h>
#include <direct.h>

double user_theta = 0;
double user_height = 0;

double user_theta_l = 1;
double user_height_l = 0;

const float M_PI = 3.14159265358979323846;
// commit please

void drawSphere(double r, int lats, int longs, float red, float green, float blue) {
    int i, j;
    glColor3f(red, green, blue); // Set the color for the sphere
    for (i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
}

void drawSmallSphereAttached(double r_big, double r_small_ratio, double theta) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta);
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = 0.1; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y, z); // 새로운 위치로 이동
    drawSphere(r_small, 10, 10, 0.0, 0.0, 0.0); // 작은 구를 검은색으로 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}

void drawNoseAttached(double r_big, double r_small_ratio, double theta) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta);
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = -0.1; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y, z); // 새로운 위치로 이동
    drawSphere(r_small, 10, 10, 255, 255, 255); // 작은 구를 white color 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}

void drawNoseBridge(double r_big, double r_small_ratio, double theta, float z_up) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta);
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = -0.1; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y, z + z_up); // 새로운 위치로 이동
    drawSphere(r_small, 10, 10, 255, 255, 255); // 작은 구를 white color 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}

void drawEllipsoid(double rx, double ry, double rz, int stacks, int slices, float red, float green, float blue) {

    int i, j;
    glColor3f(red, green, blue);
    for (i = 0; i < stacks; ++i) {
        double phi1 = M_PI * (-0.5 + (double)i / stacks);
        double phi2 = M_PI * (-0.5 + (double)(i + 1) / stacks);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= slices; ++j) {
            double theta = 2 * M_PI * (double)j / slices;
            double x1 = cos(theta) * cos(phi1);
            double y1 = sin(theta) * cos(phi1);
            double z1 = sin(phi1);

            double x2 = cos(theta) * cos(phi2);
            double y2 = sin(theta) * cos(phi2);
            double z2 = sin(phi2);

            glVertex3d(rx * x1, ry * y1, rz * z1);
            glVertex3d(rx * x2, ry * y2, rz * z2);
        }
        glEnd();
    }
}


void computeLocation() {
    double x = 2 * cos(user_theta);
    double y = 2 * sin(user_theta);
    double z = user_height;
    double d = sqrt(x * x + y * y + z * z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, d - 1.1, d + 1.1);
    gluLookAt(x, y, z, 0, 0, 0, 0, 0, 1);
}

void init() {
    GLfloat sun_direction[] = { 1.0, 0, 1.0, 1.0 };
    GLfloat sun_intensity[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat ambient_intensity[] = { 0.7, 0.7, 0.7, 1.0 };

    glClearColor(1.0, 1.0, 1.0, 0.0);
    computeLocation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_LIGHT0);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    float face_red = 239.0 / 255.0;
    float face_green = 203.0 / 255.0;
    float face_blue = 117.0 / 255.0;

    drawEllipsoid(1.0, 1.1, 1.0, 20, 20, face_red, face_green, face_blue);
    // drawSphere(1.0, 10, 10, 0.9725, 0.8431, 0.5216); // Draw the large sphere with its color
    drawSmallSphereAttached(1.0, 0.04, 0.18); // Draw the small sphere attached with its radius and color
    drawSmallSphereAttached(1.0, 0.04, -0.18);
    drawNoseAttached(1.0, 0.1, 0.08);
    drawNoseBridge(1.0, 0.05, -0.05, 0.05);
    drawNoseBridge(1.0, 0.05, 0.03, 0.06);
    drawNoseBridge(1.0, 0.05, 0.00, 0.06);
    drawNoseBridge(1.0, 0.05, -0.03, 0.06);
    drawNoseBridge(1.0, 0.05, -0.05, 0.05);
    drawNoseAttached(1.0, 0.1, -0.08);

    glutSwapBuffers();
}

void idle() {
    static int lastTime = 0;
    int time = glutGet(GLUT_ELAPSED_TIME);

    if (lastTime == 0 || time >= lastTime + 40) {
        lastTime = time;
        glutPostRedisplay();
    }
}

void visible(int vis) {
    glutIdleFunc(vis == GLUT_VISIBLE ? idle : NULL);
}

void special(int k, int x, int y) {
    switch (k) {
    case GLUT_KEY_UP:    user_height += 0.1; break;
    case GLUT_KEY_DOWN:  user_height -= 0.1; break;
    case GLUT_KEY_LEFT:  user_theta += 0.1; break;
    case GLUT_KEY_RIGHT: user_theta -= 0.1; break;
    }
    computeLocation();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(300, 300);
    glutCreateWindow("Sphere");

    init();
    glutDisplayFunc(draw);
    glutVisibilityFunc(visible);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}
