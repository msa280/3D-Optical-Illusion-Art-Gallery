//  ========================================================================
//  COSC363 Computer Graphics (2023), University of Canterbury
//
//  Name: Muhammad Haider Saeed (msa280)
//  FILE NAME: Assignment 1.cpp
//
//
// Theme: Optical Illusion
// Project Time Taken: 84 Hours
//
// Program displays three different models in a spatial arrangement in a room.
// The user can view the models using controls.
//
// Use the following controls:
//
// ↑ Arrow - Move Forwards
// ↓ Arrow - Move Backwards
// ← Arrow - Turn Left
// → Arrow - Turn Right
// 1 - AAO1 View (Ames Window)
// 2 - AAO2 View (Moire Patterns)
// 3 - AAO3 View (3D Road Scene)
// 0 - Gallery View
// F1 - Move Up
// F2 - Move Down
// F5 - Turn Red Traffic Signal On
// F6 - Turn Yellow Traffic Signal On
// F7 - Turn Green Traffic Signal On
//
//  ========================================================================



//-- Including all relevant headers and libraries --------------------------
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "Headers/loadTGA.h"
#include "Headers/loadBMP.h"

using namespace std;

//-- Globals ---------------------------------------------------------------
GLuint txId[4];                             // Texture ID List
float cam_hgt = 20;                         // Camera Height
float angle = 0;                            // View angle
float eye_x = 1, eye_y = 1, eye_z = 1;      // Eye parameters
float look_x = 0, look_y = 0, look_z = 0;   // Look parameters
float amesWinAngle = 0;                     // Ames Window rotation rate
float fan_rot = 0;                          // Windmill fan rotation angle
float aao2_height = 19;                     // AAO2 height
float aao2_height_2 = 35;                   // AAO2 height
float aao2_height_dir = 0;                  // AAO2 height direction
float aao2_height_dir_2 = 0;                // AAO2 height direction 2
float car_pos_z = -185;                     // Car position
float speed_rate = 1;                       // Car speed increase rate
float light_rate = 1;                       // Rate of light movement
float sl_source_angle = 0;                  // Rotation angle for the spotlight source
float sl_dir = 0;                           // Spotlight direction
int red = 1, yellow = 0, green = 0;         // States of traffic light



//---- A function to load textures in TGA format ----
void loadTextures()
{
    glGenTextures(4, txId);     // Create 4 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //First texture
    loadTGA("TGAs/Wall.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //First texture
    loadTGA("TGAs/Illusion_A.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[2]);  //First texture
    loadTGA("TGAs/Illusion_B.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[3]);  //First texture
    loadTGA("TGAs/Road.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}



//---- Function to compute the normal vector using three vertices -----------------------------------
void normal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    float nx, ny, nz;
    nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
    ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
    nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    glNormal3f(nx, ny, nz);
}



//---- Camera Control Functions --------------
void controls(int key, int y, int z)
{
    if (key == GLUT_KEY_LEFT)
    {   //Change direction to left
        angle -= 0.05;
    }
    else if (key == GLUT_KEY_RIGHT)
    {   //Change direction to right
        angle += 0.05;
    }
    else if (key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 4 * sin(angle);
        eye_z += 4 * cos(angle);
    }
    else if (key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 4 * sin(angle);
        eye_z -= 4 * cos(angle);
    }
    else if (key == GLUT_KEY_F1)
    {   // Camera Height increases
        cam_hgt += 1;
    }
    else if (key == GLUT_KEY_F2)
    {   // Camera Height decreases
        cam_hgt -= 1;
    }
    else if (key == GLUT_KEY_F5)
    {   // Red Signal On
        red = 1, yellow = 0, green = 0;
    }
    else if (key == GLUT_KEY_F6)
    {   // Yellow Signal On
        red = 0, yellow = 1, green = 0;
    }
    else if (key == GLUT_KEY_F7)
    {   // Green Signal On
        red = 0, yellow = 0, green = 1;
    }

    look_x = eye_x + 10 * sin(angle);
    look_z = eye_z - 10 * cos(angle);
    glutPostRedisplay();
}



//-- AAO and Gallery Controls -----------------------------
void handleKeypress(unsigned char key, int x, int y)
{
    if (key == '1') {         // AAO1 - Ames Window
        cam_hgt = 20;
        angle = 0;
        eye_x = -60;
        eye_z = 0;
    }
    else if (key == '2') {    // AAO2 - Optical Illusion
        cam_hgt = 20;
        angle = -2.0;
        eye_x = 100;
        eye_z = 0;
    }
    else if (key == '3') {    // AAO3 - 3D Road Scene
        cam_hgt = 100;
        angle = 1.5708;
        eye_x = -220;
        eye_z = 0;
    }
    else if (key == '0') {    // Gallery View
        cam_hgt = 150;
        angle = 1.5708;
        eye_x = -450;
        eye_z = 0;
    }

    look_x = eye_x + 10 * sin(angle);
    look_z = eye_z - 10 * cos(angle);

    glutPostRedisplay();
}



//-- AAO1 Timer ----------------------------
void aao1_timer(int value)
{
    amesWinAngle += 5;
    if (amesWinAngle > 360)
    {
        amesWinAngle = 0;
    }
    glutTimerFunc(50, aao1_timer, value);
    glutPostRedisplay();
}



//-- AAO2 Timer 1 ----------------------------
void aao2_timer(int value)
{
    aao2_height += 0.1 * aao2_height_dir;
    if (aao2_height <= 19) {
        aao2_height_dir = 1;             // Going Up
    }
    else if (aao2_height >= 28) {
        aao2_height_dir = -1;            // Going Down
    }
    glutTimerFunc(50, aao2_timer, value);
    glutPostRedisplay();
}



//-- AAO2 Timer 2 ----------------------------
void aao2_timer_2(int value)
{
    aao2_height_2 += 0.1 * aao2_height_dir_2;
    if (aao2_height_2 >= 35) {
        aao2_height_dir_2 = -1;             // Going Down
    }
    else if (aao2_height_2 <= 26) {
        aao2_height_dir_2 = 1;              // Going Up
    }
    glutTimerFunc(50, aao2_timer_2, value);
    glutPostRedisplay();
}



//-- This is the timer for car behavior which is dependent on
//-- traffic light and car speed.
void aao3_timer(int value)
{
    if (car_pos_z < 200 and green == 1) {
        car_pos_z += 0.5 * speed_rate;
        if (speed_rate < 15) {            // Max car speed
            speed_rate += 0.1;            // Acceleration rate increasing by 0.1 if car has not reached the max acceleration.
        }
    }
    else if ((red == 1 or yellow == 1) and car_pos_z < 200) {
        if (speed_rate > 0) {
            car_pos_z += 0.5 * speed_rate;
            if (car_pos_z > 200) {
                car_pos_z = -200;
            }
            speed_rate -= 0.25;           // Car decelerating on red and yellow light. Deceleration is faster than acceleration.
        }
        else if (car_pos_z > 200) {
            car_pos_z = -200;
        }
    }
    else {
        car_pos_z = -200;
    }

    glutTimerFunc(50, aao3_timer, value);
    glutPostRedisplay();
}



//-- Windmill Timer ----------------------------
void windmill_timer(int value)
{
    fan_rot += 3;
    if (fan_rot > 360)
    {
        fan_rot = 0;
    }
    glutTimerFunc(50, windmill_timer, value);
    glutPostRedisplay();
}



//-- This is the timer for the spotlight and its source to change angle. --------
void spotlight_timer(int value)
{
    sl_dir += 0.5 * light_rate;
    sl_source_angle += 2 * light_rate;
    if (sl_dir <= -5 and sl_source_angle <= -45) {
        light_rate = 1;
    }
    else if (sl_dir >= 5 and sl_source_angle >= 45)
    {

        light_rate = -1;
    }
    glutTimerFunc(50, spotlight_timer, value);
    glutPostRedisplay();
}



//-- Draw a Floor Plane -------------------
void draw_floor()
{
    bool flag = false;
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

    for (int x = -200; x <= 200; x += 20) {
        for (int z = -200; z <= 200; z += 20) {

            if (flag) glColor3f(1.0, 1.0, 1.0);
            else glColor3f(0.0, 0.0, 0.0);
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z + 20);
            glVertex3f(x + 20, 0, z + 20);
            glVertex3f(x + 20, 0, z);
            flag = !flag;
        }
    }
    glEnd();
}



//-- Creates a tunnel for the 3D Road Scene ---
void create_tunnel(void)
{
    glColor3f(0, 0, 0);
    glNormal3f(0, 1, 0);

    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glVertex2f(40, 0);
    glVertex2f(40, 20);
    glVertex2f(0, 20);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}



//-- Creates the walls for the illusion gallery ------
void create_walls(void)
{
    glColor3f(0, 0, 0);
    glNormal3f(0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glBegin(GL_QUADS);

    // FRONT WALL
    glTexCoord2f(0., 0.);
    glVertex3f(-200, 0, -200);
    glTexCoord2f(3., 0.);
    glVertex3f(200, 0, -200);
    glTexCoord2f(3., 3.);
    glVertex3f(200, 200, -200);
    glTexCoord2f(0., 3.);
    glVertex3f(-200, 200, -200);

    // Side Wall
    glTexCoord2f(0., 0.);
    glVertex3f(200, 0, 200);
    glTexCoord2f(3., 0.);
    glVertex3f(-200, 0, 200);
    glTexCoord2f(3., 3.);
    glVertex3f(-200, 200, 200);
    glTexCoord2f(0., 3.);
    glVertex3f(200, 200, 200);

    // Back Wall
    glTexCoord2f(0., 0.);
    glVertex3f(200, 0, 200);
    glTexCoord2f(3., 0.);
    glVertex3f(200, 0, -200);
    glTexCoord2f(3., 3.);
    glVertex3f(200, 200, -200);
    glTexCoord2f(0., 3.);
    glVertex3f(200, 200, 200);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}



//-- Creates the first static illusion ------------
void create_first_Static_Illusion(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);

    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glTexCoord2f(0., 0.);
    glVertex2f(0, 0);
    glTexCoord2f(1., 0.);
    glVertex2f(10, 0);
    glTexCoord2f(1., 1.);
    glVertex2f(10, 10);
    glTexCoord2f(0., 1.);
    glVertex2f(0, 10);

    glEnd();
    glDisable(GL_TEXTURE_2D);

}



//-- First Static Illusion -----------------
void create_second_Static_Illusion(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);

    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glTexCoord2f(0., 0.);
    glVertex2f(0, 0);
    glTexCoord2f(1., 0.);
    glVertex2f(10, 0);
    glTexCoord2f(1., 1.);
    glVertex2f(10, 10);
    glTexCoord2f(0., 1.);
    glVertex2f(0, 10);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}



//-- Ames Window Structure--
void create_Ames_Window(void)
{
    glNormal3f(0, 0, 1);

    glTranslatef(6.9, 0, 0);

    glBegin(GL_QUADS);

    glColor3f(0, 0, 1);        // Blue Colored Area
    glVertex3f(0, 9.8, 0);
    glVertex3f(1, 9.4, 0);
    glVertex3f(1, 0.4, 0);
    glVertex3f(0, 0, 0);

    glVertex3f(-1, 8.7, 0);
    glVertex3f(-1, 8.1, 0);
    glVertex3f(-4.6, 7.7, 0);
    glVertex3f(-5.4, 8, 0);

    glVertex3f(-4.6, 7.7, 0);
    glVertex3f(-4.6, 5.3, 0);
    glVertex3f(-5.4, 5.3, 0);
    glVertex3f(-5.4, 8, 0);

    glVertex2f(-5.4, 4.5);
    glVertex2f(-4.6, 4.5);
    glVertex2f(-4.6, 2.1);
    glVertex2f(-5.4, 1.8);

    glVertex2f(-4.6, 2.1);
    glVertex2f(-5.4, 1.8);
    glVertex2f(-1, 1.1);
    glVertex2f(-1, 1.6);

    glVertex2f(-9.8, 7.4);
    glVertex2f(-9.1, 7.2);
    glVertex2f(-6.2, 7.5);
    glVertex2f(-6.2, 7.9);

    glVertex2f(-9.1, 7.2);
    glVertex2f(-9.8, 7.4);
    glVertex2f(-9.8, 5.2);
    glVertex2f(-9.1, 5.2);

    glVertex2f(-9.8, 4.6);
    glVertex2f(-9.1, 4.6);
    glVertex2f(-9.1, 2.7);
    glVertex2f(-9.8, 2.4);

    glVertex2f(-9.1, 2.7);
    glVertex2f(-9.8, 2.4);
    glVertex2f(-6.2, 1.9);
    glVertex2f(-6.2, 2.3);

    glVertex2f(-10.4, 7.3);
    glVertex2f(-10.4, 7);
    glVertex2f(-12.8, 6.7);
    glVertex2f(-13.3, 6.9);

    glVertex2f(-13.3, 5.1);
    glVertex2f(-12.8, 5.1);
    glVertex2f(-12.8, 6.7);
    glVertex2f(-13.3, 6.9);

    glVertex2f(-10.4, 2.8);
    glVertex2f(-12.8, 3.1);
    glVertex2f(-13.3, 2.9);
    glVertex2f(-10.4, 2.5);

    glVertex2f(-13.3, 4.7);
    glVertex2f(-12.8, 4.7);
    glVertex2f(-12.8, 3.1);
    glVertex2f(-13.3, 2.9);

    glColor3f(1, 1, 1);     // White Colored Area
    glVertex2f(0, 9.8);
    glVertex2f(0, 8.8);
    glVertex2f(-13.8, 6.8);
    glVertex2f(-13.8, 7.4);

    glVertex2f(0, 8.8);
    glVertex2f(-1, 8.7);
    glVertex2f(-1, 0.2);
    glVertex2f(0, 0);

    glVertex2f(0, 1);
    glVertex2f(0, 0);
    glVertex2f(-13.8, 2.4);
    glVertex2f(-13.8, 3.1);

    glVertex2f(-1, 5.4);
    glVertex2f(-1, 4.4);
    glVertex2f(-5.4, 4.5);
    glVertex2f(-5.4, 5.3);

    glVertex2f(-5.4, 5.3);
    glVertex2f(-5.4, 4.5);
    glVertex2f(-9.8, 4.6);
    glVertex2f(-9.8, 5.2);

    glVertex2f(-6.2, 7.9);
    glVertex2f(-5.4, 8);
    glVertex2f(-5.4, 1.8);
    glVertex2f(-6.2, 1.9);

    glVertex2f(-9.8, 7.4);
    glVertex2f(-10.4, 7.3);
    glVertex2f(-10.4, 2.5);
    glVertex2f(-9.8, 2.4);

    glVertex2f(-13.3, 6.9);
    glVertex2f(-13.3, 2.9);
    glVertex2f(-13.8, 3.1);
    glVertex2f(-13.8, 6.8);

    glVertex2f(-10.4, 5.2);
    glVertex2f(-10.4, 4.6);
    glVertex2f(-13.3, 4.7);
    glVertex2f(-13.3, 5.1);

    glEnd();
}



//-- Creates a circle
void create_Circle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 200; //The number of triangles used to draw circle

    GLfloat twicePi = 2.0f * 3.1415926f;

    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y + (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}



//-- Creates aao2 circle pattern --------------
void create_pattern()
{
    glColor3f(1.0, 0.0, 0.0);
    for (float i = 0.2; i < 8.0; i += 0.2) {
        create_Circle(3, 3, i);
    }
}



//-- Creates a road. -------------------------
void create_road(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);

    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(400, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(400, 0, 40);
    glTexCoord2f(0, 1);
    glVertex3f(0, 0, 40);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}



//-- Creates a car. ---------------------------
void create_car()
{
    glPushMatrix();
    glColor3f(1, 0, 1);
    glScalef(4, 2, -5);
    glutSolidCube(3);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 0, 0.5);
    glScalef(3, 1.5, -2.5);
    glTranslatef(0, 3, 0);
    glutSolidCube(3);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glRotatef(90, 0, 1, 0);
    glTranslatef(5, -2, -7);
    glutSolidCylinder(2.5, 1, 20, 2);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-4, -2, -7);
    glutSolidCylinder(2.5, 1, 20, 2);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-4, -2, 6);
    glutSolidCylinder(2, 1, 20, 2);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-4, -2, 6);
    glutSolidCylinder(2.5, 1, 20, 2);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glRotatef(90, 0, 1, 0);
    glTranslatef(5, -2, 6);
    glutSolidCylinder(2.5, 1, 20, 2);
    glPopMatrix();

    glEnd();
}



//-- Creates a traffic light. ---------------
void create_traffic_light()
{
    glPushMatrix();
    glColor3f(0.545, 0.271, 0.075);
    glTranslatef(0, 2, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCylinder(4, 3, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5, 0.6, 0.7);
    glTranslatef(0, 2, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCylinder(1, 20, 9, 2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(2, 5, 2);
    glTranslatef(0, 5, 0);
    glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(red, 0, 0);
    glTranslatef(0, 28, 2);
    glutSolidSphere(1.2, 20, 15);
    glPopMatrix();

    glPushMatrix();
    glColor3f(yellow, yellow, 0);
    glTranslatef(0, 25, 2);
    glutSolidSphere(1.2, 20, 15);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, green, 0);
    glTranslatef(0, 22, 2);
    glutSolidSphere(1.2, 20, 15);
    glPopMatrix();

}



//-- Creates a spotlight with light shining. ---------------------
void create_spotlight(void)
{
    float light1_pos[4] = { 0, 25, -180, 1 };
    float light1_dir[3] = {sl_dir, 0, 25};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);

    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(0, 25, 3);
    glRotatef(135, 1, 0, 0);
    glRotatef(sl_source_angle, 0, 1, 0);
    glutSolidCone(4, 5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 0, 0);
    glutSolidCube(7);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5, 0.6, 0.7);
    glTranslatef(0, 2, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCylinder(1, 20, 9, 2);
    glPopMatrix();
}


//-- Creates the windmill fans -----------
void create_windmill_fans(void)
{
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0, 45, 20);
    glutSolidCylinder(1, 10, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.275, 0.510, 0.706);
    glTranslatef(-4, 46, 25);
    glRotatef(0, 1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 27);
    glVertex2f(8, 27);
    glVertex2f(8, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.275, 0.510, 0.706);
    glTranslatef(-4, 44, 25);
    glRotatef(-180, 1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 27);
    glVertex2f(8, 27);
    glVertex2f(8, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.275, 0.510, 0.706);
    glTranslatef(1, 49, 27);
    glRotatef(-90, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 27);
    glVertex2f(8, 27);
    glVertex2f(8, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.275, 0.510, 0.706);
    glTranslatef(-1, 41, 27);
    glRotatef(90, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 27);
    glVertex2f(8, 27);
    glVertex2f(8, 0);
    glEnd();
    glPopMatrix();
}


//-- Creates the windmill structure --------
void create_windmill(void)
{
    glPushMatrix();
    glColor3f(0.627, 0.322, 0.176);
    glTranslatef(0, 0, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCylinder(20, 45, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.412, 0.412, 0.412);
    glTranslatef(0, 45, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidSphere(20, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 45, 20);
    glRotatef(fan_rot, 0, 0, 1);
    glTranslatef(0, -45, -20);
    create_windmill_fans();
    glPopMatrix();
}


//--------------------------------------------  Display Module --------------------------------------------------------
//
// This is the main display module containing function calls for generating
// the scene.

void display()
{
    float lpos[4] = {30., 80., 80., 1.0};                                  // Light's position
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);                               // Set light position

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                             // GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye_x, cam_hgt, eye_z, look_x, cam_hgt, look_z, 0, 1, 0);   // Starting looking position

    draw_floor();                         // Floor

    create_walls();                       // Walls

    glPushMatrix();
    glTranslatef(-175, 0, 0);
    glRotatef(90, 0, 1, 0);              // Windmill
    create_windmill();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-62, 90, -199);
    glRotatef(90, 1, 0, 0);
    create_spotlight();                   // Spotlight
    glPopMatrix();

    glPushMatrix();
    glTranslatef(130, 1, 199);
    create_tunnel();                      // Tunnel 1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(130, 1, -199);
    create_tunnel();                      // Tunnel 2
    glPopMatrix();

    glPushMatrix();
    glScalef(0.8, 0.8, 1);
    glTranslatef(187, 6, car_pos_z);
    create_car();                         // Car
    glPopMatrix();

    glPushMatrix();
    glTranslatef(130, 1, 200);
    glRotatef(90, 0, 1, 0);
    create_road();                        // Road
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180, 1, -155);
    glRotatef(300, 0, 1, 0);
    create_traffic_light();               // Traffic Light 1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180, 1, 10);
    glRotatef(268, 0, 1, 0);
    create_traffic_light();               // Traffic Light 2
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180, 1, 170);
    glRotatef(225, 0, 1, 0);
    create_traffic_light();               // Traffic Light 3
    glPopMatrix();

    glPushMatrix();
    glScalef(5, 5, 5);
    glTranslatef(39, 4, -10);
    glRotatef(90, 0, 1, 0);
    create_first_Static_Illusion();        // Static Illusion 1
    glPopMatrix();

    glPushMatrix();
    glScalef(5, 5, 5);
    glTranslatef(39, 4, 24);
    glRotatef(90, 0, 1, 0);
    create_second_Static_Illusion();       // Static Illusion 2
    glPopMatrix();

    glPushMatrix();
    glScalef(3, 3, 3);
    glTranslatef(-40, aao2_height_2, 40);
    glRotatef(135, 0, 1, 0);
    create_pattern();                       // AAO2 Pattern 1
    glPopMatrix();

    glPushMatrix();
    glScalef(3, 3, 3);
    glTranslatef(-40, aao2_height, 40);
    glRotatef(135, 0, 1, 0);
    create_pattern();                       // AAO2 Pattern 1
    glPopMatrix();

    glPushMatrix();
    glScalef(3, 3, 3);
    glTranslatef(-20, 2, -50);
    glRotatef(amesWinAngle, 0, 1, 0);
    create_Ames_Window();                   // Ames Window
    glPopMatrix();

    glutSwapBuffers();

}





//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    loadTextures();

    glEnable(GL_LIGHTING);                         // Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);   // Background colour set to sky blue

    float grey[] = { 0.2, 0.2, 0.2, 1.0 };
    float yellow[] = { 1.0, 1.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);       // Creating Yellow Spotlight
    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellow);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100);

    glEnable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 30, 1000);  // The camera view volume

    glDisable(GL_TEXTURE_2D);
}





//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(400, 10);
    glutCreateWindow("Assignment 1");
    initialize();

    glutDisplayFunc(display);
    glutSpecialFunc(controls);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(50, aao1_timer, 0);
    glutTimerFunc(50, aao2_timer, 0);
    glutTimerFunc(50, aao2_timer_2, 0);
    glutTimerFunc(50, aao3_timer, 0);
    glutTimerFunc(50, spotlight_timer, 0);
    glutTimerFunc(50, windmill_timer, 0);
    glutMainLoop();
    return 0;
}
