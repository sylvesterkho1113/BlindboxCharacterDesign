#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <GL/glut.h>

#include "RationalMain.hpp"

//Ideally, you should include only the files that you wish
//  to use, however, all of them are included here just for
//  convenience.
// #include "CGLab02.hpp"
#include "ProjectMain.hpp"

//IMPORTANT:
//  Change the namespace scope below corresponding to
//  to the lab number which you wish to use.
// using CGLab02::MyVirtualWorld;
using ProjectMain::MyVirtualWorld;

MyVirtualWorld myvirtualworld;

using namespace std;

MyWindow   window;
MyWorld    world;
MyViewer   viewer;
MySetting  setting;
MyAxis     worldaxis;

void myDisplayFunc(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glPushMatrix();

    glTranslatef(world.posX, world.posY, world.posZ);
    glRotatef(world.rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(world.rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(world.rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(world.scaleX, world.scaleY, world.scaleZ);

    worldaxis.draw();

    myvirtualworld.draw();

 glPopMatrix();

 glFlush();   // send any buffered output to be rendered
 glutSwapBuffers();

  if (myvirtualworld.autoRotatefunc) {
        world.rotateY += 1.0f; // Rotate around Y-axis
        if (world.rotateY >= 360.0f)
            world.rotateY -= 360.0f;
    }

 myvirtualworld.tickTime(); //tick the clock
 glutPostRedisplay();//force openGL to call myDisplayFunc() again
}

void myReshapeFunc(int width, int height)
{
 window.width  = width;
 window.height = height;
 glViewport(0, 0, width, height);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
 GLfloat xinc,yinc,zinc;
 xinc = yinc = zinc = 0.0;
 switch (key)
 {
    case 'a': case 'A': xinc = -setting.posInc;  break;
    case 'd': case 'D': xinc =  setting.posInc;  break;
    case 'q': case 'Q': yinc = -setting.posInc;  break;
    case 'e': case 'E': yinc =  setting.posInc;  break;
    case 'w': case 'W': zinc = -setting.posInc;  break;
    case 's': case 'S': zinc =  setting.posInc;  break;

    case 27  : exit(1); break;
 }

 world.move(xinc, yinc, zinc);

 glutPostRedisplay();
}

void mySpecialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN:
        world.rotateX -= setting.angleInc;
        break;
    case GLUT_KEY_UP:
        world.rotateX += setting.angleInc;
        break;
    case GLUT_KEY_LEFT:
        world.rotateY -= setting.angleInc;
        break;
    case GLUT_KEY_RIGHT:
        world.rotateY += setting.angleInc;
        break;
    case GLUT_KEY_HOME:
        myDataInit();
        break;
    case GLUT_KEY_F1:
        setting.shadingMode = !setting.shadingMode;
        if (setting.shadingMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case GLUT_KEY_F2:
        worldaxis.toggle();
        break;
    case GLUT_KEY_F3:
        myvirtualworld.toggleTextures();
        break;
    case GLUT_KEY_F4:
        myvirtualworld.autoRotatefunc = !myvirtualworld.autoRotatefunc;
        break;
    case GLUT_KEY_F5:
        GLboolean lightingIsOn;
        glGetBooleanv(GL_LIGHTING, & lightingIsOn);
        if (lightingIsOn == GL_TRUE)
            glDisable(GL_LIGHTING);
        else glEnable(GL_LIGHTING);
        break;
    case GLUT_KEY_F6:
        myvirtualworld.toggleLight(0);
        break;
    case GLUT_KEY_F7:
        myvirtualworld.toggleLight(1);
        break;
    case GLUT_KEY_F8:
        myvirtualworld.toggleLight(2);
        break;
    case GLUT_KEY_F9: //myvirtualworld.minunAttacking = true;
        if (!myvirtualworld.minunAttacking) {
            myvirtualworld.minunAttacking = true;
            myvirtualworld.minunGoingForward = true;
        } // Start attack animation
        break;
    case GLUT_KEY_F10: //myvirtualworld.weddleAttacking = true;
        if (!myvirtualworld.weedleAttacking) {
            myvirtualworld.weedleAttacking = true;
            myvirtualworld.weedleGoingForward = true;
        } // Start attack animation

        break;
    }
    glutPostRedisplay();
}

void myMouseFunc(int button, int state, int x, int y)
{
 y = window.height - y;
 switch (button)
 {
    case GLUT_RIGHT_BUTTON:
       if (state==GLUT_DOWN && !setting.mouseRightMode)
       {
          setting.mouseX = x;
          setting.mouseY = y;
          setting.mouseRightMode = true;
       }
       if (state==GLUT_UP && setting.mouseRightMode)
       {
          setting.mouseRightMode = false;
       }
       break;
    case GLUT_LEFT_BUTTON:
       if (state==GLUT_DOWN && !setting.mouseLeftMode)
       {
          setting.mouseX = x;
          setting.mouseY = y;
          setting.mouseLeftMode = true;
       }
       if (state==GLUT_UP &&  setting.mouseLeftMode)
       {
          setting.mouseLeftMode = false;
       }
       break;
 }
}

void myMotionFunc(int x, int y)
{
 y = window.height - y;
 GLint xinc = x - setting.mouseX;
 GLint yinc = y - setting.mouseY;

 if(setting.mouseRightMode)
 {
    world.rotate(0.0f, 0.0f, -xinc*0.5);
 }
 if(setting.mouseLeftMode)
 {
    world.rotate(-yinc*0.5, xinc*0.5, 0.0f);
 }

 setting.mouseX = x;
 setting.mouseY = y;
 glutPostRedisplay();
}

void myDataInit()
{
 window.title = "TCG6223 Computer Graphics";
 window.posX = 100;
 window.posY = 100;
 window.width  = 800;
 window.height = 500;

 world.rotateX  = 0.0;
 world.rotateY  = 0.0;
 world.rotateZ  = 0.0;
 world.posX     = 0.0;
 world.posY     = 0.0;
 world.posZ     = 0.0;
 world.scaleX   = 1.0;
 world.scaleY   = 1.0;
 world.scaleZ   = 1.0;

 viewer.eyeX    = 35.0;
 viewer.eyeY    = 30.0;
 viewer.eyeZ    = 35.0;
 viewer.centerX = 0.0;
 viewer.centerY = 0.0;
 viewer.centerZ = 0.0;
 viewer.upX     = 0.0;
 viewer.upY     = 1.0;
 viewer.upZ     = 0.0;
 viewer.zNear   = 0.1;
 viewer.zFar    = 500.0;
 viewer.fieldOfView = 60.0;
 viewer.aspectRatio = static_cast<GLdouble> (window.width) / window.height;

 setting.posInc   = 1.0;
 setting.angleInc = 2.0;
 setting.mouseX   = 0;
 setting.mouseY   = 0;

 setting.mouseRightMode = false;
 setting.mouseLeftMode = false;

 setting.shadingMode = true;
}

void myViewingInit()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(viewer.fieldOfView,
                viewer.aspectRatio,
                viewer.zNear,
                viewer.zFar);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(viewer.eyeX,   viewer.eyeY,   viewer.eyeZ,
           viewer.centerX,viewer.centerY,viewer.centerZ,
           viewer.upX,    viewer.upY,    viewer.upZ );
}

void myLightingInit()
{
 static GLfloat  ambient[] = { 0.0f,  0.0f,  0.0f, 1.0f };
 static GLfloat  diffuse[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat  specref[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat position[] = {10.0f, 10.0f, 10.0f, 1.0f };
  short shininess = 128;

 glDisable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
 glLightfv(GL_LIGHT0, GL_POSITION, position);
 glEnable(GL_LIGHT0);

 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 glEnable(GL_COLOR_MATERIAL);

 glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
 glMateriali(GL_FRONT, GL_SHININESS, shininess);

 glEnable(GL_NORMALIZE);
}

void myInit()
{
 myDataInit();

 glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
 glutInitWindowPosition(window.posX, window.posY); // Set top-left position
 glutInitWindowSize(window.width, window.height); //Set width and height
 glutCreateWindow(window.title.c_str());// Create display window

 glutDisplayFunc(myDisplayFunc);  // Specify the display callback function
 glutReshapeFunc(myReshapeFunc);
 glutKeyboardFunc(myKeyboardFunc);
 glutSpecialFunc(mySpecialFunc);
 glutMotionFunc(myMotionFunc);
 glutMouseFunc(myMouseFunc);

 glPointSize(4.0);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LESS);
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 glFrontFace(GL_CCW);
 glShadeModel (GL_SMOOTH);
 // Using for setting background color on pront out result
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

 glEnable(GL_CULL_FACE);

 myViewingInit();

 myLightingInit();

 myvirtualworld.init();
}

void myWelcome()
{
 cout << "*****************************************************************\n";
 cout << "*                   TCG6223 Computer Graphics                   *\n";
 cout << "*                  FIST, Multimedia University                  *\n";
 cout << "*             Project by Section 1E - Group Rational            *\n";
 cout << "*****************************************************************\n";
 cout << "| Press:                                                        |\n";
 cout << "|   <a>,<d>,<w>,<s>,<q>,<e> => move world                       |\n";
 cout << "|   <arrows>                => rotate world                     |\n";
 cout << "|   HOME                    => restore defaults                 |\n";
 cout << "|   ESC                     => exit                             |\n";
 cout << "|                                                               |\n";
 cout << "|   F1                      => toggle shading / wire-frame mode |\n";
 cout << "|   F2                      => toggle rendering of axes         |\n";
 cout << "|   F3                      => toggle texture on / off          |\n";
 cout << "|   F4                      => Auto rotation from battle field  |\n";
 cout << "|                                                               |\n";
 cout << "|   F5                      => toggle lighting on / off         |\n";
 cout << "|   F6                      => toggle white light (sun)         |\n";
 cout << "|   F7                      => toggle red light (weedle lamp)   |\n";
 cout << "|   F8                      => toggle blue light (minun lamp)   |\n";
 cout << "|                                                               |\n";
 cout << "|   F9                      => (Blue side) Minun skill          |\n";
 cout << "|   F10                     => (Red side) Weedle skill          |\n";
 cout << "|                                                               |\n";
 cout << "| Mouse (Left Drag or Right Drag) => rotate world               |\n";
 cout << "|                                                               |\n";
 cout << "*****************************************************************\n";
 cout << "|                      H A V E   F U N  !!!                     |\n";
 cout << "*****************************************************************\n";
}

//--------------------------------------------------------------------
int main(int argc, char **argv)
{
 glutInit(&argc, argv);

 myWelcome();

 myInit();

 glutMainLoop(); // Display everything and wait
}
//--------------------------------------------------------------------
