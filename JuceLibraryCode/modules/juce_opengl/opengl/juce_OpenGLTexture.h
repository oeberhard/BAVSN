/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __JUCE_OPENGLTEXTURE_JUCEHEADER__
#define __JUCE_OPENGLTEXTURE_JUCEHEADER__

//==============================================================================
/**
    Creates an openGL texture from an Image.
*/
class JUCE_API  OpenGLTexture
{
public:
    OpenGLTexture();
    ~OpenGLTexture();

    /** Creates a texture from the given image.

        Note that if the image's dimensions aren't a power-of-two, the texture may
        be created with a larger size.

        The image will be arranged so that its top-left corner is at texture
        coordinate (0, 1).
    */
    void loadImage (const Image& image);

    /** Creates a texture from a raw array of pixels.
        The width and height provided must be valid - i.e. power-of-two unless
        the underlying GL system allows otherwise.
        The data is sent directly to the OpenGL driver without being flipped vertically,
        so the first pixel will be mapped onto texture coordinate (0, 0).
        bottom-left corner of the texture
    */
    void loadARGB (const PixelARGB* pixels, int width, int height);

    /** Creates a texture from a raw array of pixels.
        This is like loadARGB, but will vertically flip the data so that the first
        pixel ends up at texture coordinate (0, 1), and if the width and height are
        not powers-of-two, it will compensate by using a larger texture size.
    */
    void loadARGBFlipped (const PixelARGB* pixels, int width, int height);

    /** Creates an alpha-channel texture from an array of alpha values.
        The width and height provided must be valid - i.e. power-of-two unless
        the underlying GL system allows otherwise.
        The data is sent directly to the OpenGL driver without being flipped vertically,
        so the first pixel will be mapped onto texture coordinate (0, 0).
        bottom-left corner of the texture
    */
    void loadAlpha (const uint8* pixels, int width, int height);

    /** Frees the texture, if there is one. */
    void release();

    /** Binds the texture to the currently selected openGL context. */
    void bind() const;

    /** Unbinds the texture to the currently selected openGL context. */
    void unbind() const;

   #if JUCE_USE_OPENGL_FIXED_FUNCTION
    /** Draws this texture into the current context, with the specified corner positions. */
    void draw2D (float x1, float y1,
                 float x2, float y2,
                 float x3, float y3,
                 float x4, float y4,
                 const Colour& colour) const;

    /** Draws this texture into the current context, with the specified corner positions. */
    void draw3D (float x1, float y1, float z1,
                 float x2, float y2, float z2,
                 float x3, float y3, float z3,
                 float x4, float y4, float z4,
                 const Colour& colour) const;
   #endif

    /** Returns the GL texture ID number. */
    GLuint getTextureID() const noexcept        { return textureID; }

    int getWidth() const noexcept               { return width; }
    int getHeight() const noexcept              { return height; }

    /** Returns true if a texture can be created with the given size.
        Some systems may require that the sizes are powers-of-two.
    */
    static bool isValidSize (int width, int height);

private:
    GLuint textureID;
    int width, height;
    OpenGLContext* ownerContext;

    void create (int w, int h, const void*, GLenum type);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLTexture);
};


#endif   // __JUCE_OPENGLTEXTURE_JUCEHEADER__
