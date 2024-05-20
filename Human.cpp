#define STB_IMAGE_IMPLEMENTATION
#define MY_PI 3.14159265358979323846
#define TEXTURE_NUM 1 // 총 텍스처 개수
#define TEXTURE_BODY 0 // 몸통 텍스처 번호

#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <GL\GLU.h>
#include <GL\GL.h>
#include <glut.h>
#include <glaux.h>
#include <direct.h>
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

double rotX = 0; // X축 회전 각도
double rotY = 0; // Y축 회전 각도
    
double user_theta = 0;
double user_height = 0;

double user_theta_l = 1;
double user_height_l = 0;
double zoomFactor = 1.0;

GLuint textureID[TEXTURE_NUM]; // 텍스처 저장
const char* texture[TEXTURE_NUM] = { "Image\\acadmic_dress.bmp"};


void drawSphere(double r, int lats, int longs, float red, float green, float blue) {
    int i, j;
    glColor3f(red, green, blue); // Set the color for the sphere
    for (i = 0; i <= lats; i++) {
        double lat0 = MY_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = MY_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double lng = 2 * MY_PI * (double)(j - 1) / longs;
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


void drawEllipsoid(double rx, double ry, double rz, int stacks, int slices, float red, float green, float blue) {
    int i, j;
    glColor3f(red, green, blue);
    for (i = 0; i < stacks; ++i) {
        double phi1 = MY_PI * (-0.5 + (double)i / stacks);
        double phi2 = MY_PI * (-0.5 + (double)(i + 1) / stacks);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= slices; ++j) {
            double theta = 2 * MY_PI * (double)j / slices;
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
    // glBindTexture(GL_TEXTURE_2D, 0);
}


void drawMustache(float baseRadius, float topRadius, float height, float x_pos, float y_pos, float z_pos, float theta) {
    glPushAttrib(GL_CURRENT_BIT);
    GLUquadric* quadric = gluNewQuadric(); // 원기둥을 그리기 위한 쿼드릭 객체 생성

    glPushMatrix(); // 현재 행렬 상태를 푸시

    // 원기둥을 그리기 위한 위치 조정
    glTranslatef(x_pos - 0.07, y_pos, z_pos - 0.3); // 원기둥 위치로 이동
    glRotatef(theta, 1.0, 0.0, 0.0); // 원기둥을 z축에서 x축으로 회전시킴

    // 원기둥 그리기
    gluCylinder(quadric, baseRadius, topRadius, height, 20, 20); // 원기둥 객체, 기저 반경, 상단 반경, 높이, 원주분할수, 높이분할수

    glPushMatrix();  // 하단 위치로 이동
    drawSphere(baseRadius, 20, 20, 0.0, 0.0, 0.0);  // 구 그리기
    glPopMatrix();

    // 상단 구 그리기: 원기둥의 상단에 위치
    glTranslatef(0, 0, height);  // 원기둥의 높이만큼 이동하여 상단에 구를 배치
    drawSphere(topRadius, 20, 20, 0.0, 0.0, 0.0);  // 구 그리기


    glPopMatrix(); // 원래 행렬 상태로 복원
    gluDeleteQuadric(quadric); // 쿼드릭 객체 메모리 해제
    glPopAttrib();  // 상태 복원
}


void drawEye(double r_big, double r_small_ratio, double theta) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta);
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = -0.25; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y, z + 0.02); // 새로운 위치로 이동
    drawSphere(r_small, 10, 10, 0.0, 0.0, 0.0); // 작은 구를 검은색으로 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}


void drawNose(double r_big, double r_small_ratio, double theta) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta);
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = -0.5; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x - 0.3, y, z + 0.1); // 새로운 위치로 이동
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
    double z = -0.5; // 단순화를 위해 z를 0으로 가정하여 적용

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x - 0.1, y, z + z_up + 0.1); // 새로운 위치로 이동
    drawSphere(r_small, 30, 30, 255, 255, 255); // 작은 구를 white color 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}


void drawEar(double r_big, double r_small_ratio, double theta, float red, float green, float blue, float y_up) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * sin(theta);
    double y = 0.1;
    double z = r_big * cos(theta) - 0.3;

    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y + y_up, z); // 새로운 위치로 이동
    drawSphere(r_small, 30, 30, red, green, blue); // 작은 구를 얼굴과 같은 색으로 그리기
    // 이전 좌표로 복원
    glPopMatrix();
}


void drawCheek(double r_big, double r_small_ratio, double theta) {
    double r_small = r_big * r_small_ratio; // 큰 구의 반지름에 따른 작은 구의 반지름 계산
    // 큰 구 표면에 작은 구의 중심이 위치하도록 계산
    double x = r_big * cos(theta) - 0.34;
    double y = r_big * sin(theta);
    // 작은 구의 반지름만큼 z를 조정하지 않아도 됨
    double z = -0.35; // 단순화를 위해 z를 0으로 가정하여 적용

    // 색상 설정
    float r = 255.0 / 255;
    float g = 150.0 / 255;
    float b = 115.0 / 255;
    // 현재 좌표 저장
    glPushMatrix();

    glTranslatef(x, y, z + 0.05); // 새로운 위치로 이동
    drawSphere(r_small, 50, 50, r, g, b); // 큰 구를 그리기
    glPopMatrix();
}


void drawRightArm(float baseRadius, float topRadius, float height) {
    glPushAttrib(GL_CURRENT_BIT);
    double r_small = 1.0 * 0.05;
    GLUquadric* quadric = gluNewQuadric(); // 원기둥을 그리기 위한 쿼드릭 객체 생성

    glPushMatrix(); // 현재 행렬 상태를 푸시
    float armR = 254.0 / 255.0;
    float armG = 220.0 / 255.0;
    float armB = 133.0 / 255.0;
    
    // 소매 그리기
    glTranslatef(0.2, 0.325, -1.0);
    glColor3f(0, 0, 0); // 검정색 설정
    glRotatef(50, -1, 0, 0); // 원기둥을 회전시켜 사선으로 위치시킴. x축 기준 50도 회전
    gluCylinder(quadric, baseRadius, topRadius, height / 4 * 3, 30, 20);
    
    // 팔 그리기
    glTranslatef(0, 0, height / 4 * 3);
    glColor3f(armR, armG, armB); // 팔 색 설정
    gluCylinder(quadric, baseRadius, topRadius, height / 4, 30, 20);
    glTranslatef(0, 0, height / 4);

    // 손, 냥젤리 그리기
    drawSphere(baseRadius, 5, 20, armR, armG, armB);

    float r = 255.0 / 255;
    float g = 150.0 / 255;
    float b = 115.0 / 255;

    glTranslatef(0.15, -0.07, 0);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, 0.07, 0.03);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, 0.07, -0.03);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, -0.07, -0.1);
    drawSphere(r_small + 0.02, 5, 20, r, g, b);

    glPopMatrix(); // 원래 행렬 상태로 복원
    gluDeleteQuadric(quadric); // 쿼드릭 객체 메모리 해제
    glPopAttrib();  // 상태 복원
}


void drawLeftArm(float baseRadius, float topRadius, float height) {
    glPushAttrib(GL_CURRENT_BIT);
    double r_small = 1.0 * 0.05;
    GLUquadric* quadric = gluNewQuadric(); // 원기둥을 그리기 위한 쿼드릭 객체 생성

    glPushMatrix(); // 현재 행렬 상태를 푸시
    float armR = 254.0 / 255.0;
    float armG = 220.0 / 255.0;
    float armB = 133.0 / 255.0;

    // 소매 그리기
    glTranslatef(0.24, -0.7, -1.2);
    glColor3f(0, 0, 0); // 검정색 설정
    glRotatef(30, -1, 0, 0); // x축 기준 30도 회전
    gluCylinder(quadric, baseRadius, topRadius, height / 2, 30, 20);

    // 팔 그리기
    glTranslatef(0, 0, -height / 6);
    glColor3f(armR, armG, armB); // 팔 색 설정
    gluCylinder(quadric, baseRadius, topRadius, height / 6, 30, 20);

    // 손, 냥젤리 그리기
    glRotatef(90, 0, 0, 1);
    drawSphere(baseRadius, 5, 20, armR, armG, armB);

    float r = 255.0 / 255;
    float g = 150.0 / 255;
    float b = 115.0 / 255;

    glTranslatef(0.15, -0.07, -0.05);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, 0.07, -0.03);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, 0.07, 0.03);
    drawSphere(r_small - 0.02, 5, 20, r, g, b);
    glTranslatef(0, -0.07, 0.1);
    drawSphere(r_small + 0.02, 5, 20, r, g, b);

    glPopMatrix(); // 원래 행렬 상태로 복원
    gluDeleteQuadric(quadric); // 쿼드릭 객체 메모리 해제
    glPopAttrib();  // 상태 복원
}


void drawLeg(float baseRadius, float topRadius, float height) {
    glPushAttrib(GL_CURRENT_BIT);
    GLUquadric* quadric = gluNewQuadric(); // 원기둥을 그리기 위한 쿼드릭 객체 생성

    glPushMatrix(); // 현재 행렬 상태를 푸시
    GLfloat legR = 161.0 / 255.0;
    GLfloat legG = 119.0 / 255.0;
    GLfloat legB = 67.0 / 255.0;

    // 오른쪽 다리 그리기
    glTranslatef(0, 0.35, -0.8);
    glColor3f(legR, legG, legB); // 원기둥 색 설정
    gluCylinder(quadric, baseRadius, topRadius, height / 3, 20, 20); // 원기둥 객체, 기저 반경, 상단 반경, 높이, 원주분할수, 높이분할수
    
    glTranslatef(0, 0, height / 3);
    glColor3f(0.0, 0.0, 0.0);
    gluCylinder(quadric, baseRadius, topRadius, height, 20, 20); // 원기둥 객체, 기저 반경, 상단 반경, 높이, 원주분할수, 높이분할수

    // 왼쪽 다리 그리기
    glTranslatef(0, -0.7, -height / 3);
    glColor3f(legR, legG, legB); // 원기둥 색 설정
    gluCylinder(quadric, baseRadius, topRadius, height / 3, 20, 20); // 원기둥 객체, 기저 반경, 상단 반경, 높이, 원주분할수, 높이분할수

    glTranslatef(0, 0, height / 3);
    glColor3f(0.0, 0.0, 0.0);
    gluCylinder(quadric, baseRadius, topRadius, height, 20, 20); // 원기둥 객체, 기저 반경, 상단 반경, 높이, 원주분할수, 높이분할수

    glPopMatrix(); // 원래 행렬 상태로 복원
    gluDeleteQuadric(quadric); // 쿼드릭 객체 메모리 해제
    glPopAttrib();  // 상태 복원
}


void drawBody(double rx, double ry, double rz, int stacks, int slices) {
    glBindTexture(GL_TEXTURE_2D, textureID[TEXTURE_BODY]);
    for (int i = 0; i < stacks; ++i) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            double phi1 = MY_PI * (-0.5 + (double)i / stacks);
            double phi2 = MY_PI * (-0.5 + (double)(i + 1) / stacks);
            double theta = 2 * MY_PI * (double)j / slices;
            double x1 = cos(theta) * cos(phi1);
            double y1 = sin(theta) * cos(phi1);
            double z1 = sin(phi1);

            double x2 = cos(theta) * cos(phi2);
            double y2 = sin(theta) * cos(phi2);
            double z2 = sin(phi2);

            double s = (double)j / slices; // 텍스처 가로
            double t1 = 0.5 + sin(phi1) * 0.5; // 텍스처 세로
            double t2 = 0.5 + sin(phi2) * 0.5;

            glTexCoord2d(s, t1); // 텍스처 좌표 지정
            glVertex3d(rx * x1, ry * y1, rz * z1);

            glTexCoord2d(s, t2);
            glVertex3d(rx * x2, ry * y2, rz * z2);
        }
        glEnd();
    }
}


void computeLocation() {
    double x = 2 * cos(user_theta);
    double y = 2 * sin(user_theta);
    double z = user_height;
    double d = sqrt(x * x + y * y + z * z) / zoomFactor; // Apply zoom factor

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, d - 1.1, d + 1.1);
    gluLookAt(x + 0.5, y, z - 0.5, 0, 0, 0, 0, 0, 1);
}


void loadTexture(void) { // 텍스처 이미지 파일 열기
    for (GLint loop = 0; loop < TEXTURE_NUM; loop++) {
        GLubyte* data = NULL;
        GLint width, height;
        FILE* fp = NULL;
        const char* filename = texture[loop];
         if (fopen_s(&fp, filename, "rb")) {
            printf("Error: Open failed!\n");
            return;
        }

        fseek(fp, 18, SEEK_SET); // BMP 파일에서 너비와 높이 읽기
        fread(&width, 4, 1, fp);
        fread(&height, 4, 1, fp);

        data = (GLubyte*) malloc(width * height * 3); // 이미지 데이터를 위한 메모리 할당
        fseek(fp, 54, SEEK_SET); // BMP 파일의 데이터 읽기
        fread(data, width * height * 3, 1, fp);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glGenTextures(TEXTURE_NUM, textureID);
        glBindTexture(GL_TEXTURE_2D, textureID[loop]);
        fclose(fp);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free(data);
    }
}


void init() {
    GLfloat sun_direction[] = { 1.0, 0, 1.0, 1.0 };
    GLfloat sun_intensity[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat ambient_intensity[] = { 0.7, 0.7, 0.7, 1.0 };

    loadTexture();
    computeLocation();
    glClearColor(1.0, 1.0, 1.0, 0.0);
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Reset the model-view matrix
    // Apply rotation from mouse motion
    // glRotatef(rotX, 1.0, 0.0, 0.0); // Rotate about the x-axis
    glRotatef(rotY, 0.0, 0.0, 1.0); // Rotate about the z-axis

    glDisable(GL_TEXTURE_2D);
    float face_red = 254.0 / 255.0;
    float face_green = 220.0 / 255.0;
    float face_blue = 133.0 / 255.0;
    drawEllipsoid(1.0, 1.1, 1.0, 20, 20, face_red, face_green, face_blue);

    glPushMatrix();
    glRotatef(20, 0, 1, 0);
    glTranslatef(0, 0, 0.6); // Move up from the center of the sphere
    glColor3f(0.1, 0.1, 0.1); // Set the color of the cylinder
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, 0.5, 0.5, 0.5, 30, 30); // Cylinder parameters
    gluDeleteQuadric(quadric);
    glPopMatrix();

    glPushMatrix();
    glRotatef(20, 0, 1, 0);
    // Translate the matrix to the top of the cylinder
    glTranslatef(0, 0, 0.5);
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_QUADS);
    glVertex3f(-0.6, -0.6, 0.6);
    glVertex3f(0.6, -0.6, 0.6);
    glVertex3f(0.6, 0.6, 0.6);
    glVertex3f(-0.6, 0.6, 0.6);
    glEnd();

    glTranslatef(0.55, 0.55, 0.4);
    glColor3f(1, 1, 0);
    GLUquadric* quadric1 = gluNewQuadric();
    gluCylinder(quadric1, 0.02, 0.02, 0.2, 30, 30); // Cylinder parameters
    gluDeleteQuadric(quadric1);

    glTranslatef(0, 0, -0.05);
    GLUquadric* quadric2 = gluNewQuadric();
    gluCylinder(quadric2, 0.06, 0.02, 0.1, 30, 30); // Cylinder parameters
    gluDeleteQuadric(quadric2);

    // Restore the matrix state
    glPopMatrix();

    // drawSphere(1.0, 10, 10, 0.9725, 0.8431, 0.5216); // Draw the large sphere with its color

    glColor3f(0.0, 0.0, 0.0); // 원기둥 색 설정
    drawMustache(0.025, 0.025, 0.17, 1.0, 0.1, -0.1, -67);//오른쪽 위
    drawMustache(0.025, 0.025, 0.17, 1.0, 0.25, -0.2, 67);//오른쪽 아래
    drawMustache(0.025, 0.025, 0.17, 1.0, -0.1, -0.1, 67);//왼쪽 위
    drawMustache(0.025, 0.025, 0.17, 1.0, -0.25, -0.2, -67);//왼쪽 아래

    drawEye(1, 0.04, 0.18); // Eyes
    drawEye(1, 0.04, -0.18);

    drawNose(1.2, 0.1, 0.08); // Nose
    drawNoseBridge(1.0, 0.07, -0.05, 0.05);
    drawNoseBridge(1.0, 0.07, 0.03, 0.06);
    drawNoseBridge(1.0, 0.07, 0.00, 0.06);
    drawNoseBridge(1.0, 0.07, -0.03, 0.06);
    drawNoseBridge(1.0, 0.07, -0.05, 0.05);
    drawNose(1.2, 0.1, -0.08);

    drawEar(1.0, 0.2, 0.5, face_red, face_green, face_blue, 0.55); // Ear
    drawEar(1.0, 0.2, 0.5, face_red, face_green, face_blue, -0.7);

    drawCheek(1.0, 0.3, 0.25); // Cheek
    drawCheek(1.0, 0.3, -0.25);

    glTranslatef(0.0, 0.0, -1);
    drawLeg(0.23, 0.23, 0.5); // Leg
    glTranslatef(0.0, 0.0, 1);

    drawRightArm(0.17, 0.17, 1.0);
    drawLeftArm(0.17, 0.17, 1.0);

    glEnable(GL_TEXTURE_2D); // 텍스처 활성화
    glTranslatef(0.0, 0.0, -1);
    drawBody(0.65, 0.6, 0.65, 20, 20);
    glTranslatef(0.0, 0.0, 1);
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


void mouseMotion(int x, int y) {
    static int lastX = 0, lastY = 0;

    // 현재 마우스 위치와 이전 마우스 위치의 차이에 따라 회전 각도를 계산
    rotX += (y - lastY);
    rotY += (x - lastX);

    lastX = x;
    lastY = y;

    glutPostRedisplay(); // 화면 다시 그리기
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': // Zoom in
        zoomFactor *= 0.9;
        break;
    case 'z': // Zoom out
        zoomFactor /= 0.9;
        break;
    }
    computeLocation();
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);;
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("ChoonSik");
    GLenum err = glewInit();
    if (GLEW_OK != err) printf("Error: GLEW failed!\n");

    init();
    glutMotionFunc(mouseMotion); // 마우스 드래깅 이벤트 핸들러 등록
    glutDisplayFunc(draw);
    glutVisibilityFunc(visible);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}