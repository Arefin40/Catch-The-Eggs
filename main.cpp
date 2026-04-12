#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "src/utils.hpp"

void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_TRIANGLES);
   COLORS::RED.apply();
   glVertex2f(-0.5f, -0.5f);

   COLORS::GREEN.apply();
   glVertex2f(0.5f, -0.5f);

   COLORS::BLUE.apply();
   glVertex2f(0.0f, 0.5f);
   glEnd();

   glFlush();
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
   glutInitWindowSize(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT);

   int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
   int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
   int windowX = (screenWidth - Utils::WINDOW_WIDTH) / 2;
   int windowY = (screenHeight - Utils::WINDOW_HEIGHT) / 2;
   glutInitWindowPosition(windowX, windowY);

   glutCreateWindow("Catch The Eggs");
   glEnable(GL_MULTISAMPLE);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   glutDisplayFunc(display);
   glEnable(GL_MULTISAMPLE);
   glutMainLoop();

   return 0;
}