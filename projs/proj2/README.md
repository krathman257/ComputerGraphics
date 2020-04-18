# Ray Tracing

*You may work individually or in groups of up to four on this assignment.
To receive credit, demonstrate your
completed program over video conference or create a tag called `proj02`
push your code (and tag) up to Bitbucket and submit the hash to D2L prior to
class on the due date.*

## Summary

We have learned a lot of techniques for rasterization over the semester.  Many
of them apply just as well to ray tracing.  In project 2, you have the
opportunity to transfer some of the techniques from the previous labs in a
different context and explore additional techniques.

Write a ray tracer that can render a collection of triangles.  (While you do not
need to read in triangles from an obj file, you are welcome to do so.)

Your ray tracer must support Phong shading.

You must add two feature (listed below) of your choosing.
For features, you
will be expected to identify one or more sources that help you learn a technique
(lecture, course notes, and book are acceptable)
and then implement the technique into your ray tracer.  Some techniques may
include:

- procedural textures/perlin noise (good for making textures like marble)
- texture mapping (alternative method for natural surfaces)
- constructive solid geometry with quadrics (good for modeling curved surfaces)
- bezier surfaces (also good for modeling curved surfaces)
- reflections (good for modeling mirrored surfaces)
- refractions (good for modeling glass surfaces)
- anti-aliasing (good for removing sharp edges)
- motion blur (good for modeling moving objects)
- depth of field (good for modeling blury backgrounds and foregrounds)
- soft shadows (good for making shadows)
- CPU based parallelization (e.g. threads) (good for improving performance)
- GPU implementation (e.g. CUDA) (good for improving performance)

You do NOT need to use one of the suggested techniques, but if you go with
something outside the list, please speak with me first to check if it is
acceptable.  (Some techniques seem really easy, but in fact are very
challenging).  You have a lot of freedom on this program.  Note that the
complexity of learning the techniques range greatly based on your background.
For example making a ray tracer on the GPU is not too hard if you already know
CUDA, but is really hard if you have no experience with parallel computing.

## Required Materials

Your proj2 directory must include:

* All source code for the completed program.
* An image rendered by your ray tracer
* A README.md file with any necessary instructions for using your program.
* A PDF document that describes the technique that you implemented.

## Detailed Requirements

*Your program must*

*Be an original program written by you.*

You may use code from labs as a starting point. You may talk with other students
about the program, but looking at their code is not allowed.  You are welcome to
use code that I have provided from previous labs. But, if you are using code I
provided, you will learn more if you use my code as a direction for your own
library.  You may use GLM.

*Render and shade triangles using ray tracing*

Implement Phong shading and a light source in your program to render triangles.
The triangle intersection implementation that I provided in lab 9 is very slow,
you may
want to investigate faster intersection algorithms and use your spatial data
structure from lab 9 to improve performance.

*Two features*

Implement two features from the above list (or features discussed with me).

*A README.md file.*

Write up what your program does, and how to use it, along with any extra
information you want me to know about your program.

*A PDF file*

A pdf file with a technical description of two of your features. The
description should be in your own words with citations to the references that
you used to learn the material.  For many of the techniques, it will be useful
to add a figure or two to help you describe the technique.  Please try to keep
your description at around a three pages (or maybe a few extras if you
have lots of figures).

You are ENCOURAGED to read other students' writeup and provide them feedback.

*Demonstrate creativity and self expression.*

Put effort into your program beyond the minimum requirements. Experiment, put
extra features in, organize your code exquisitely, it's really up to you.
Explain what you did in your README.md file.

## Point breakdown

| Points | Description |
|--------|-------------|
| 05 | Ray tracing triangles |
| 05 | Phong shading |
| 20 | General (code style, execution, creativity, self expression and README.md) |
| 20 | Feature 1 |
| 15 | Technical writeup of feature 1 |
| 20 | Feature 2 |
| 15 | Technical writeup of feature 2 |
| 20 (EC) | For each additional feature |
| *100* | *Total points* w/ at most 60 extra EC points (e.g. 3 additional features)|
