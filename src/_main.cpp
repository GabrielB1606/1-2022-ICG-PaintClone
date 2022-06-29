// #include <stdio.h>
// #include "GL/freeglut.h"

// void disp(void)
// {
//     glClearColor(1, 1, 1, 1);
//     glClear(GL_COLOR_BUFFER_BIT);
//     glBegin(GL_TRIANGLE_FAN);
//         glVertex3f(-0.5, -0.5, 0.0);
//         glVertex3f( 0.5, -0.5, 0.0);
//         glVertex3f( 0.5,  0.5, 0.0);
//         glVertex3f(-0.5,  0.5, 0.0);
//     glEnd();
//     glFlush();
// }

// int main(int argc, char** argv)
// {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_SINGLE);
//     glutInitWindowSize(500, 500);
//     glutCreateWindow("Hello World");
//     glutDisplayFunc(disp);
//     glutMainLoop();
//     return 0;
// }