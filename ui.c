#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "ui.h"

#define ESCAPE 27

GLint window;
GLubyte *texture;
GLfloat zoom = 1.0;
GLint xoffset = 0;
GLint yoffset = 0;

GLvoid ui_transform(GLint newwidth, GLint newheight)
{
    static GLint width, height;
    width = (newwidth > 0) ? newwidth : width;
    height = (newheight > 0) ? newheight : height;

    glViewport(0, 0, width, height);              /* Set the viewport */
    glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
    glLoadIdentity();				                /* Reset The Projection Matrix */
    float w = width * zoom;
    float h = height * zoom;
    glOrtho(-(w / 2.0),
            w / 2.0, 
            -(h / 2.0),
            h / 2.0,
            -1, 100);
    glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

GLvoid ui_drawscene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPushMatrix();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D,      // Target
            0,                  // Level
            GL_RGB,             // Internal format
            512,                // Width
            512,                // Height
            0,                  // Border
            GL_RGB,             // Format
            GL_UNSIGNED_BYTE,   // Type
            texture);           // Pointer to the texture
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-256, -256, 0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-256, 256, 0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(256, 256, 0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(256, -256, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //glPopMatrix();

    glFlush();
    glutSwapBuffers();
    printf("drawing scene\n");
}

GLvoid ui_init(int *argc, char **argv, GLint width, GLint height)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    window = glutCreateWindow("Hello, world!");

    ui_reinit(width, height);
    glutDisplayFunc(ui_drawscene);
    glutReshapeFunc(ui_resize);
    glutKeyboardFunc(ui_keypress);
}

GLvoid ui_reinit(GLint width, GLint height)
{
    glClearColor(0.0, 0.0, 15.0/255.0, 1.0);
    glPointSize(4.0);
    glLineWidth(2.0);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ui_transform(width, height);
}

GLvoid ui_run()
{
    glutMainLoop();
}

GLvoid ui_resize(GLint width, GLint height)
{
    if ( width < 1 ) width = 1;
    if ( height < 1 ) height = 1;
    ui_transform(width, height);
}

GLvoid ui_keypress(GLubyte key, GLint x, GLint y)
{
    switch ( key )
    {
        case ESCAPE:
            printf("Exiting...");
            glutDestroyWindow(window);
            exit(0);
        case '+':
            zoom -= 0.05;
            printf("New zoom: %f\n", zoom);
            ui_transform(0, 0);
            break;
        case '-':
            zoom += 0.05;
            printf("New zoom: %f\n", zoom);
            ui_transform(0, 0);
            break;
    }
    glutPostRedisplay();
}
