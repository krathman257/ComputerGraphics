# Project 2

This program renders a scene in outer space, featuring a gas planet, an Earth-like planet, and a moon, as well as two spaceships. The purpose is to demonstrate the usage of two main graphical features, being reflective surfaces (seen on the spaceships) and Perlin noise (seen on the planets).

## Usage

Starting the program will create a terminal prompt asking for a resolution to render at and various features to be enabled or disabled.
The resolution must be two integers separated by a space.
The features can be take either a 0 or a 1 as input.

Once the program has its arguments, the image will be rendered. Progress is shown as a floating point percent continuously being printed by the console. The final image will be saved as "RenderedImage.bmp" in the same directory as the executable.

## Non-Trivial Features Added

-Mapping textures to spheres
-Loading .OBJ file as a vector of triangles
-Normal smoothing for .OBJ imports
-Reflections
-Procedural texture generations with Perlin noise

## Example Input

Enter a resolution to be rendered at (Suggest 320 240, 800 600, 2048 1536):

800 600

Simple or detailed model? (0=Simple, 1=Detailed)

1

Normal smoothing? (0=No, 1=Yes)


0

Reflections? (0=No, 1=Yes)

1

Rendering image...

0

0.166667

0.333333

0.5

0.666667

0.833333

1

1.16667

...

## Notes

Rendering can take a long time, particularly with the detailed model. At 2048X1536, an image with the detailed model, normal smoothing and reflections took around 90 minutes to render, though at lower resolutions, rendering can take less than 1 minute.

The space ships can be difficult to make out with reflections turned on, as they become perfectly reflective surfaces. Turning reflections off will make them easier to see.

