# Lab 5 - Materials and Lights

*You may work individually on this assignment.
To receive credit, demonstrate your
completed program during lab or create a tag called `lab05`push your
code (and tag) up to Bitbucket and submit the hash to D2L prior to class on
the due date.*

**You may NOT use a linear algebra library for this lab.**

In this lab, you will implement the Phong shading. The Phong model combines
diffuse reflections, which mimic rough surfaces; specular reflections which
mimic shiny surfaces; and ambient reflections, which mimic the small amount of
light that is uniformly scattered around the scene.

I have provided a lot of code for this one (much of it from my solution to
lab4).  You do not need to use my code but are welcome to use as much of it as
you like.  Whatever pieces you use (if any), I highly recommend understanding
what the code is doing.  If you have any questions or if you feel like you are
struggling with the content and would have completely drowned on this lab
without the starting code, I encourage you to come to my office hours (or
schedule a time to meet if you cannot attend my office hours).

## Recommended Reading

Transforming Normal Vectors - FoCG 6.2.2
Phone Shading - FoCG 10.1-2
[Phong Shading](https://learnopengl.com/Lighting/Basic-Lighting)
[Phong reflection model](http://en.wikipedia.org/wiki/Phong_reflection_model)

## Part 1 - Normals

To calculate any of the lighting equations, we need to have the normal at the
point we’re shading. The normal is part of the underlying geometry. We need to
specify the normal at each point, just like we specify position. In the previous
labs we implemented a few primitives.  Modify your primitives to also generate
normals. You’ll need a normal for every vertex and you will need to send the
vertex to the GPU. I recommend updating your VBO and VAO such that each vertex
is defined by three values to represent the position, three values to represent
the normal, and three values to represent the color.  The normal at each point
should be a unit vector that is perpendicular to the face that the point is a
part of. For example, the top face of the cube has a normal of (0,1,0), so all
points that define the top face of the cube should have that normal assigned to
them.  I have included my shapes. If you had some trouble creating shapes in the
last lab, you can use mine as a starting point  You must add normals to at least
TWO shapes.

## Part 2 - Vertex Shader

We will calculate our lighting equations in our fragment shader. To do so, we
will need to send vertex and normal data to the fragment shader.  First, add a
normal input variable on your vertex shader. Then, create position and normal
outputs in the vertex shader and add corresponding inputs in the fragment
shader.

Be sure to apply the model matrix. Recall that to transform normal vectors you
will need to multiply by the inverse transpose of the transformation matrix and
normalize the resulting vector, whereas your position vectors can just be
multiplied by the transformation matrix. If you just multiply the normals by the
transformation matrix, you will see shading errors when your model matrix
includes scaling.  You should implement the inverse and transpose of the model
matrix on the CPU and transfer it to the GPU.

## Part 3 - Fragment Shader

### Light

#### Position

We will need a light source if we want to shade our shapes. In your fragment shader,
create a uniform vec3 variable that will store the light position.  In the
fragment shader, transform the light position into eye space. Any uniform
variables you have in your vertex shader can also be used in your fragment
shader as long as you declare them as uniform variables exactly like you did in
your vertex shader (no changes are needed in your C++ code).

#### Color and intensity (optional)

The color of your light can be another uniform vec3 variable that represents the
color of your light source. It can be multiplied in to your lighting equations
anywhere material color is multiplied in. If you don't specify a light color,
the result will be as if the light is fully white (1,1,1). Another option would
be to add a uniform float intensity variable as a multiplier on the brightness
of your light. These are just ways to adjust the lighting in your scene from
your program. For this lab, just do these if you're interested in playing around
with it.

### Material

#### Diffuse reflections

Implement diffuse reflections using the light position and the position and
normals on your shapes. You’ll need to calculate the light direction (a unit
vector that points in the direction of your light source), and then take the dot
product of that vector with your normal. Clamp the dot product between 0 and 1,
as we don't want any negative values. Multiply the resulting value by the color
of your shapes and assign that color to the output color.

#### Specular reflections

To calculate specular reflections we’ll need to calculate the view direction (a
unit vector that points toward the camera) and a reflection direction (a unit
vector of our light direction reflected about the normal). We’ll also need a
shininess uniform variable, which is a floating point or integer value. The
higher the shininess value the shinier the surface (0 would be not shiny and
all, 128 would be very shiny). Take the dot product of the view direction and
reflection direction vectors, clamp the value to between 0 and 1 and raise it to
the power of your shininess variable. Since specular reflections represents
shiny reflections, it should mostly reflect the color of your light source. If
you added a light color, multiply the value you just calculated by the light
color and add it to the diffuse color before assigning it to the output color.
If you didn't make a light color variable, just multiply by white (1,1,1).

#### Ambient reflections

Ambient reflections can be implemented by adding a small amount of color to your
output color. Use a multiplier such as .1, and multiply it by the shape's color
and add it to the specular and diffuse components before assigning it to the
output color.

## Part 4 - Demo

Demo your work by moving the light and/or your object (either procedurally or
through keyboard commands).  If you make controls, please be sure to provide a
description of how to use the controls in the assignment submission.  Your demo
should be able to change between at least two of your shapes (e.g. a sphere and
a torus).

## Things to notice

When the light is directly above a surface is when the diffuse shading will be
brightest. Rotate the shape around and see how the shape looks from different
angles. If you have implemented specular reflections, you should be able to see
a specular highlight when the camera is in line with the light's reflection.
When the material is very shiny, the highlight will be small, whereas when the
material isn't very shiny the highlight will be larger. To maintain realism,
you'll need to dial back the intensity of the specular highlight for objects
that aren't very shiny.
