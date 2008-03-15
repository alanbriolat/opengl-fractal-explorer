#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "ui.h"

#define ESCAPE 27

GLint window;

GLvoid ui_transform(GLint width, GLint height)
{
  glViewport(0, 0, width, height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,width/height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

GLvoid ui_drawscene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);
        glShadeModel(GL_SMOOTH);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0, 0, -6);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 1, -6);
    glEnd();

    glBegin(GL_QUADS);
        glShadeModel(GL_SMOOTH);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-2, -2, -6);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-2, -1, -6);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1, -1, -6);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(-1, -2, -6);
    glEnd();

    glFlush();
    printf("drawing scene\n");
}

GLvoid ui_init(int *argc, char **argv, GLint width, GLint height)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0,0);
    window = glutCreateWindow("Hello, world!");
    ui_reinit(width, height);
    glutDisplayFunc(ui_drawscene);
    glutReshapeFunc(ui_resize);
    glutKeyboardFunc(ui_keypress);
}

GLvoid ui_reinit(GLint width, GLint height)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
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
    }
}
