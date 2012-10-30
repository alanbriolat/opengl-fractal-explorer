#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "ui.h"
#include "fractal.h"
#include "worker.h"

#define ESCAPE 27
#define MAX(x, y)  x < y ? y : x

GLint window;
Fractal *f;
GLubyte *texture;
GLfloat zoom = 1.0;
GLfloat xoffset = 0;
GLfloat yoffset = 0;

int mouse_state = GLUT_UP;
int mouse_dragx, mouse_dragy;

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
    glOrtho(-(w / 2.0)/* + (xoffset)*/,
            w / 2.0/* + (xoffset)*/, 
            -(h / 2.0)/* + (yoffset)*/,
            h / 2.0/* + (yoffset)*/,
            -1, 100);
    gluLookAt(xoffset, yoffset, 100, xoffset, yoffset, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

GLvoid ui_drawscene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D,      // Target
            0,                  // Level
            GL_RGB,             // Internal format
            1024,               // Width
            1024,               // Height
            0,                  // Border
            GL_RGB,             // Format
            GL_UNSIGNED_BYTE,   // Type
            texture);           // Pointer to the texture
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-512, -512, 0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-512, 512, 0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(512, 512, 0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(512, -512, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //glPopMatrix();

    glFlush();
    glutSwapBuffers();
    printf("drawing scene\n");
}

GLvoid ui_init(int *argc, char **argv, GLint width, GLint height)
{
    // Create the initial fractal
    f = fractal_new(1024, 1024, complex_new(-2, -2), complex_new(2, 2));
    runfractal(f);
    texture = fractal_bitmap_RGB(f);

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    window = glutCreateWindow("Hello, world!");

    ui_reinit(width, height);
    glutDisplayFunc(ui_drawscene);
    glutReshapeFunc(ui_resize);
    glutKeyboardFunc(ui_keypress);
    glutSpecialFunc(ui_specialkeypress);
    glutMouseFunc(ui_mouseevent);
    glutMotionFunc(ui_mousemotion);
}

GLvoid ui_mouseevent(GLint button, GLint state, GLint x, GLint y)
{
    if ( button == GLUT_LEFT_BUTTON )
    {
        printf("Mouse event: %s\n", state == GLUT_DOWN ? "down" : "up");
        if ( state == GLUT_DOWN )
        {
            mouse_dragx = x;
            mouse_dragy = y;
        }
        mouse_state = state;
    }
}

GLvoid ui_mousemotion(GLint x, GLint y)
{
    if ( mouse_state == GLUT_DOWN )
    {
        printf("Mouse drag: (%d,%d) from (%d,%d)\n",
                x - mouse_dragx, y - mouse_dragy,
                mouse_dragx, mouse_dragy);
        xoffset -= (x - mouse_dragx) * zoom;
        yoffset += (y - mouse_dragy) * zoom;
        mouse_dragx = x;
        mouse_dragy = y;
        ui_transform(0,0);
        glutPostRedisplay();

        complex_t center = complex_new(
                real(f->min) + (real(f->max) - real(f->min))/2 + (xoffset * real(f->pixelsize)),
                imag(f->min) + (imag(f->max) - imag(f->min))/2 + (yoffset * imag(f->pixelsize)));
        printf("Center is: (%f, %f)\n", real(center), imag(center));
    }
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
            if ( zoom <= 0.5 ) 
            {
                complex_t center = complex_new(
                        (real(f->min + f->max) / 2) + (xoffset * real(f->pixelsize)),
                        (imag(f->min + f->max) / 2) + (yoffset * imag(f->pixelsize)));
                // Range of real either side of the center, divided by 2 for zoom
                _complex_t px = (real(f->max) - real(f->min)) / 4;
                // Range of imaginary either side of the center, divided by 2 for zoom
                _complex_t py = (imag(f->max) - imag(f->min)) / 4;
                Fractal *f2 = fractal_new(1024, 1024,
                        complex_new(real(center) - px, imag(center) - py),
                        complex_new(real(center) + px, imag(center) + py));
                runfractal(f2);
                fractal_destroy(f);
                free(texture);
                f = f2;
                texture = fractal_bitmap_RGB(f);
                zoom = 1.0;
                xoffset = 0;
                yoffset = 0;
            }
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

GLvoid ui_specialkeypress(GLint key, GLint x, GLint y)
{
    switch ( key )
    {
        case GLUT_KEY_UP:
            yoffset += 25;
            ui_transform(0, 0);
            break;
        case GLUT_KEY_DOWN:
            yoffset -= 25;
            ui_transform(0, 0);
            break;
        case GLUT_KEY_LEFT:
            xoffset -= 25;
            ui_transform(0, 0);
            break;
        case GLUT_KEY_RIGHT:
            xoffset += 25;
            ui_transform(0, 0);
            break;
    }
    glutPostRedisplay();
}
