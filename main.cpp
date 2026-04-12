#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include "src/texture.hpp"

Texture menuChickenTex;

void init()
{
   if (!menuChickenTex.load("assets/textures/menu-chicken.png"))
   {
      std::cerr << "Failed to load menuChicken texture\n";
   }
}

void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity(); // IMPORTANT

   menuChickenTex.bind();

   glBegin(GL_QUADS);
   glTexCoord2f(0, 0);
   glVertex2f(-0.5f, -0.5f);
   glTexCoord2f(1, 0);
   glVertex2f(0.5f, -0.5f);
   glTexCoord2f(1, 1);
   glVertex2f(0.5f, 0.5f);
   glTexCoord2f(0, 1);
   glVertex2f(-0.5f, 0.5f);
   glEnd();

   menuChickenTex.unbind();

   glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   float aspect = (float)w / (float)h;
   glOrtho(-aspect, aspect, -1, 1, -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

   glutInitWindowSize(800, 600);
   glutCreateWindow("Texture Test");

   init();

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);

   glutMainLoop();

   return 0;
}