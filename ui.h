#ifndef _UI_H_
#define _UI_H_

#include <GL/glut.h>

GLvoid ui_init(int *argc, char **argv, GLint width, GLint height);
GLvoid ui_run();

GLvoid ui_transform(GLint width, GLint height);
GLvoid ui_reinit(GLint width, GLint height);
GLvoid ui_resize(GLint width, GLint height);
GLvoid ui_keypress(GLubyte key, GLint x, GLint y);
GLvoid ui_drawscene();

extern GLubyte *texture;

#endif
