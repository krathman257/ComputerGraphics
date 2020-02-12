# Lab 4 - Transformation Matrices

*You may work individually on this assignment. To receive credit,
demonstrate your
completed program during lab or create a tag called `lab04`push your
code (and tag) up to Bitbucket and submit the hash to D2L prior to class on
the due date.*

**You may NOT use a linear algebra library for this lab.**

In this lab, we go 3D!  You will generalize the 2D transformation in the last
lab to 3D, and implement an orthographic and perspective projections.  We will
start with a box, you will implement either a parameterized sphere or a cylinder
(and can receive extra credit for implementing additional primitives such as a
sphere, cylinder, or torus).

You’ll also learn about model and view transformation matrices that are often
used to move objects and cameras around to compose a scene. A unit cube,
centered at the origin, is provided with its faces set to red, green and blue
using color attributes like the previous labs.

You’ll transform your object using translation, rotation and scale matrices
(like in the last lab, but in a higher dimension). Lastly, you’ll construct a
view matrix. All three matrices (view, model, and perspective) will be combined
in your vertex shader to transform the points in your scene.

## Recommended Reading

Transformations

* FoCG Section 6.1,.3
* [Translation Matrix](https://en.wikipedia.org/wiki/Translation_%28geometry%29)
* [Scale Matrix](http://en.wikipedia.org/wiki/Scaling_%28geometry%29)
* [Rotation Matrix](https://en.wikipedia.org/wiki/Rotation_matrix)

Projections

* FoCG 7.1-7.3
* [Perspective projection](http://www.songho.ca/opengl/gl_projectionmatrix.html)
* [Another perspective matrix derivation](http://ogldev.atspace.co.uk/www/tutorial12/tutorial12.html)

Primitives

One of the easiest ways to understand how to build parameterized shapes is by
looking at parameterizations:

* [Cylinders](https://en.wikipedia.org/wiki/Cylindrical_coordinate_system)
* [Sphere](https://en.wikipedia.org/wiki/Spherical_coordinate_system)
* [Torus](https://en.wikipedia.org/wiki/Torus#Geometry)

## Part 1 - View Matrix

The camera should be positioned at *z*=-20.  Use the `w` and `s` keys to move
position the camera along the *y*-axis between -20 and 20.  Construct a view
matrix such that the camera is focused on the origin.  Update the vertex shader
to accept a uniform variable for your view matrix and use it to transform your
vertex data.

## Part 2 - Model Matrix

Use translation, rotation, and scale values to produce a model matrix that
will be passed to your vertex shader to adjust the object's transformation.
Update your vertex shader to accept a uniform variable for the model matrix and
use it to transform your vertex data. Make sure that the scale and rotation are
performed relative to the center of the cube. Use the arrow keys to translate
the model in *x* and *y* and the `,`, and `.` keys to translate the model in
*z*.  Use the `-` and `=` to scale the model down and up, respectively. Final
use `u`, `i`, to increase and decrease rotation around *x*-axis, `o` and `p` to
rotate around *y*-axis and `[` and `]` to rotate around the *z*-axis.

## Part 3 - Projection

Construct and use an orthogonal and perspective projection matrix.  Given the
aspect ratio of your window, the desired field of view (FOV), and a near and far
plane, create a 4x4 matrix that will be passed to your vertex shader as the
projection matrix. Pick values for the FOV and near and far plane that allow you
to see most of the space.

Use the `\` to switch between projection types.

## Part 4 - Additional Primitive

Boxes are fun, but other primitives are even better.  Implement an additional
primitive (such as a sphere or a cylinder).  Switch between primitives using the
space bar.

Let's dissect how to represent a cylinder. We can represent a curved surface by
a collection of linear surfaces.  For example, the caps of the cylinder are
circles that can be approximated by an regular *n*-gon.  If we increase *n*, we get
a smoother looking cylinder (the smoothness will become very obvious once we
start implementing lighting effects).

Once we can represent a circle (parameterized by *n*), we can make two circles
*V* and *U*.  Let both circles be on an *xz*-parallel plane with radius 1.  *U*
has center at (0, 0, 0) and *V* has center at (0, 1, 0).  Lets call the vertices
of *V*, {*v1*, *v2*, ..., *vn*} and the vertices of *U*, {*u1*, *u2*, ...,
*un*}.  We can form a cylinder by "wrapping" the two circles with triangles.
The "wrapping" triangles will have edges (*ui*, *vi*) for each *i*, which will
"warp" the cylinder in squares.  Turn the squares into triangles by adding
diagonals.

## Part 5 - Extra Credit: Even More Primitives
Fill out the rest of your primitives library by implementing parameterized
versions of sphere, cylinder, torus, etc.

You will receive 5 extra credit points (on a 100 point scale) for each
additional primitive, up to 2 extra (i.e., 10 points).

## Things to notice

Notice how when we switched from an orthographic projection to a perspective
projection, lines that were once parallel may not be any more. Objects also get
smaller as they get further away. This approximates how we see the real world
and is great for 3D games or viewing a scene in a 3D editor. Orthographic
projections, on the other hand, are great for engineering or modeling
environments when we want to focus on a front, top or side view.

Our 3 rotation values are called Euler angles and are common in 3D modeling and
animation software, but they have their problems. Set the Y rotation to 90
degrees. Now try changing the X or Z rotations. Notice how they both now rotate
the cube on the same axis. By rotating around Y 90 degrees, we've made the X and
Z gimbals parallel, so we lose a degree of freedom. This is called gimbal lock.
Gimbal lock artifacts can happen when animating Euler angles.  Quaternions can
be useful in overcoming gimbal lock (we will discuss quaternions later in the
semester).

Notice how the provided code includes glEnable(GL_DEPTH_TEST) and how the
glClearColor also clears the GL_DEPTH_BUFFER_BIT.  The depth buffer is used so
objects that are closer to the camera cover objects behind them. When rendered,
the depth value of the object is written to a separate buffer. When the depth
test is enabled, each fragment tests whether its current depth is less than the
one currently in the depth buffer. If it is, then it renders the fragment,
otherwise it discards it. This behavior can be changed using the glDepthFunc.
