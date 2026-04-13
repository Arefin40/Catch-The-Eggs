#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string>
#include <iostream>
#pragma endregion

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
private:
   GLuint textureId;
   int width, height, channels;
   bool loaded;

public:
   Texture() : textureId(0), width(0), height(0), channels(0), loaded(false) {}

   ~Texture()
   {
      unload();
   }

   bool load(const std::string &filepath, bool repeat = false)
   {
      unload();
      stbi_set_flip_vertically_on_load(true);

      unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
      if (!data)
      {
         std::cerr << "Failed to load texture: " << filepath << std::endl;
         loaded = false;
         return false;
      }

      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glBindTexture(GL_TEXTURE_2D, 0);
      stbi_image_free(data);

      loaded = true;
      return true;
   }

   void unload()
   {
      if (textureId != 0)
      {
         glDeleteTextures(1, &textureId);
         textureId = 0;
      }

      width = height = channels = 0;
      loaded = false;
   }

   void bind() const
   {
      if (loaded && textureId != 0)
      {
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, textureId);
      }
   }

   void unbind() const
   {
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   GLuint getId() const { return textureId; }
   int getWidth() const { return width; }
   int getHeight() const { return height; }
   bool isLoaded() const { return loaded; }
};
