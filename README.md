# Computer Graphics CSCI 441

**NOTE: This is a live document and is subject to change throughout the
semester.**

Computer graphics is a field of computer science that involves generating visual
content.  Applications of computer graphics includes modeling and animation
software, CAD design software, image manipulation packages such as Photoshop or
Illustrator, video games, medical visualisation tools and much, much more. In
this class, we will cover some of the fundamentals of computer graphics; from
transformations and camera projections, to light and shading models, to
simulations that approximate phenomena in the real world. We will use C++ and
OpenGL to explore practical, realtime, graphics programming while learning the
theory behind it so you can apply it to other environments.

## Assignments

Class will be a mix of lecture and in class lab assignments.  There will be many
lab assignments that are designed to give you a better understanding of the
topics taught in lecture. Code will be provided to get you up and running with
the labs. You may work in groups of two or three.  You must demo your labs to me
or the TA in class or office hours and push your code up to Bitbucket in a
repository you've shared with me prior to class on the due date.  There will
also be a few major programming assignments as well. The program assignments
will take significantly more time than the labs and are designed to inspire
individual creativity and self expression. I’ll try to keep lectures as short as
possible to give you time in class to work on your assignments with me available
to ask questions.  All assignments and other resources for the class will be
made available via Bitbucket:

https://bitbucket.org/msu-cs/csci-441-spring2020

## Meeting Times

Tuesdays and Thursdays 16:30 - 17:45 in Reid Hall 202.

## Instructor

David L. Millman, Ph.D.

**Email**: david.millman@montana.edu

**Office hours**: Tue 12:00 - 12:50, Thurs 17:45 - 18:35, and by appointment

**Bitbucket username**: david_millman

**Bitbucket email**: dmillman@gmail.com

## Grader

Adiesha Liyanaralalage

**Email**: a.liyanaralalage@student.montana.edu

**Office hours**: Monday 10.00 - 11.00 BARH 259, Tuesday and Thursday after the class at the lecture hall.

**Bitbucket username**: adiesha

**Bitbucket email**: adiesha@gmail.com

## Textbook

Textbooks are optional but recommended

* [Fundamentals of Computer Graphics, 4th Edition](https://www.amazon.com/Fundamentals-Computer-Graphics-Fourth-Marschner/dp/1482229390),
(FoCG in recommended reading below).
* [OpenGL Programming Guide, 9th Edition](https://www.amazon.com/OpenGL-Programming-Guide-Official-Learning/dp/0134495497)
(referred to as GL Guide in recommended reading below).
* [Computational Geometry: Algorithms and Applications 3rd Edition](https://link-springer-com.proxybz.lib.montana.edu:3443/book/10.1007/978-3-540-77974-2)
(DutchBook in the recommended reading below, book is available online through
the MSU library)

## Online Resources

* [OpenGL Reference Pages](https://www.opengl.org/sdk/docs/man/)
* [Learn OpenGL Website](https://learnopengl.com/)(referred to as LoGL in recommended reading below)
* [C++ Documentation](http://www.cplusplus.com/doc/tutorial/)

## Class schedule

The lecture schedule is subject to change throughout the semester, but here is
the current plan. Assignments and due dates will be updated as they're assigned in class.

### January

| Date | Description                                                        | Assigned              | Due                   | Recommended Reading                           |
|------|--------------------------------------------------------------------|-----------------------|-----------------------|-----------------------------------------------|
| 1/14 | [Intro - applications of computer graphics](./notes/01-14.pdf)     | [Lab 0](./labs/lab0/) |                       | FoCG Ch 1                                     |
| 1/16 |  C++                                                               |                       |                       |                                               |
|      |                                                                    |                       |                       |                                               |
| 1/21 | [Lin Alg, Coords](./notes/01-21.pdf)                               | [Lab 1](./labs/lab1/) | [Lab 0](./labs/lab0/) | FoCG Ch 2 & 3                                 |
| 1/23 | [Coords & Colors](./notes/01-23.pdf)                               |                       |                       | FoCG Ch 2 & 3                                 |
|      |                                                                    |                       |                       |                                               |
| 1/28 | [Graphics Pipeline / OpenGL Basics](./notes/01-28.pdf)             | [Lab 2](./labs/lab2)  | [Lab 1](./labs/lab1/) | LoGL "Hello Tri", GL Ch 1, FoCG Ch 17         |
| 1/30 | [Transformations](./notes/01-30.pdf)                               |                       |                       | LoGL Ch 6                                     |

### February

| Date | Description                                                        | Assigned              | Due                   | Recommended Reading                           |
|------|--------------------------------------------------------------------|-----------------------|-----------------------|-----------------------------------------------|
| 2/04 | [Affine Transformations](./notes/02-04.pdf)                        | [Lab 3](./labs/lab3)  | [Lab 2](./labs/lab2)  | LoGL Ch 6                                     |
| 2/06 | [Affine Transformations](./notes/02-06.pdf)                        |                       |                       | LoGL Ch 6                                     |
|      |                                                                    |                       |                       |                                               |
| 2/11 | [Projections](./notes/02-11.pdf)                                   | [Lab 4](./labs/lab4)  | [Lab 3](./labs/lab3)  | LoGL Ch 7                                     |
| 2/13 | [Camera](./notes/02-13.pdf)                                        |                       |                       | LoGL Ch 7                                     |
|      |                                                                    |                       |                       |                                               |
| 2/18 | [Basic Shading](./notes/02-18.pdf)                                 | [Lab 5](./labs/lab5)  | [Lab 4](./labs/lab4)  | LoGL Ch 10                                    |
| 2/20 | [Phong Shading](./notes/02-20.pdf)                                 |                       |                       | LoGL Ch 10                                    |
|      |                                                                    |                       |                       |                                               |
| 2/25 | [Triangulation Data Structs](./notes/02-25.pdf)                    | [Lab 6](./labs/lab6)  | [Lab 5](./labs/lab5)  | LoGL Ch 12                                    |
| 2/27 | [Texture Mapping](./notes/02-27.pdf)                               |                       |                       | LoGL Ch 11                                    |

### March

| Date | Description                                                        | Assigned                  | Due                   | Recommended Reading                           |
|------|--------------------------------------------------------------------|---------------------------|-----------------------|-----------------------------------------------|
| 3/03 |  NO CLASS (Dave Sick)                                              | [Lab 7](./labs/lab7)      | [Lab 6](./labs/lab6)  | LoGL Ch 11                                    |
| 3/05 | [Texture Mapping](./notes/03-05.pdf)                               |                           |                       | LoGL Ch 11                                    |
|      |                                                                    |                           |                       |                                               |
| 3/10 | Review                                                             |                           | [Lab 7](./labs/lab7)  |                                               |
| 3/12 | Midterm                                                            |                           |                       |                                               |
|      |                                                                    |                           |                       |                                               |
| 3/17 | No class (Spring Break)                                            |                           |                       |                                               |
| 3/19 | No class (Spring Break)                                            |                           |                       |                                               |
|      |                                                                    |                           |                       |                                               |
| 3/24 |                                                                    | [Proj 1](./projs/proj1)   |                       |                                               |
| 3/26 | No class (~~NCUR 2020~~)                                           |                           |                       |                                               |
|      |                                                                    |                           |                       |                                               |
| 3/31 |                                                                    |                           |                       |                                               |

### April

| Date | Description                                                        | Assigned              | Due                       | Recommended Reading                           |
|------|--------------------------------------------------------------------|-----------------------|---------------------------|-----------------------------------------------|
| 4/02 |                                                                    |                       |                           |                                               |
|      |                                                                    |                       |                           |                                               |
| 4/07 |                                                                    |  Lab 8                | [Proj 1](./projs/proj1)   |                                               |
| 4/09 |                                                                    |                       |                           |                                               |
|      |                                                                    |                       |                           |                                               |
| 4/14 |                                                                    |  Lab 9                |  Lab 8                    |                                               |
| 4/16 |                                                                    |                       |                           |                                               |
|      |                                                                    |                       |                           |                                               |
| 4/21 |                                                                    |  Proj 2               |  Lab 9                    |                                               |
| 4/23 |                                                                    |                       |                           |                                               |
|      |                                                                    |                       |                           |                                               |
| 4/28 |                                                                    |                       |                           |                                               |
| 4/30 |                                                                    |                       |  Proj 2                   |                                               |

### May

| Date | Description                                 |
|------|---------------------------------------------|
| 5/06 | Final - 16:00-17:50                         |

### Potential Upcoming topics

Below are the topics that may be covered in the future:

- Ray Tracing
- Quaternions
- Spatial Data Structure

## Catalog Information

PREREQUISITE: M 221 and CSCI 232. High resolution computer graphics. 3D graphics
programming using a high level API. Vector mathematics for graphics. Graphics
primitives.  Curve and surface representations. Transformations using matrices
and quaternions.  Representing natural objects with particle systems and
fractals. Shading and lighting models.  Global illumination models. Color
representations.

## Course Outcomes

At the end of the course, students should be able to

* Develop basic graphical applications using OpenGL and C++
* Use graphics hardware to write realtime graphics applications
* Relate primitives from linear algebra to geometric transformations
* Implement common shading and lighting models for displaying 3D models

## Course Evaluation

* Labs 40%
* Projs 20%
* Midterm 20%
* Final 20%

## Discussion Board

Group discussions, questions, and announcements will take place using
Brightspace.  It is OK to send me an email if you have a question that you feel
is not appropriate to share with the class.  If, however, you send me an email
with a question for which the response would be useful to the rest of the class,
I will likely ask you to post it.

## Policy on Collaboration and Academic Misconduct

Collaboration IS encouraged, however, all submitted individual work must be your
own and you must acknowledge your collaborators at the beginning of the
submission.

On any group project, every team member is expected to make a substantial
contribution. The distribution of the work, however, is up to the team.

A few specifics for the assignments.  You may:

* Work with the other people on your team.
* Share ideas with other teams.
* Help other teams debug their code.

You may NOT:

* Share code that you wrote with other teams.
* Submit code that someone on your team did not write.
* Modify another's code and claim it as your own.

Using resources in addition to the course materials is encouraged. But, be sure
to properly cite additional resources. Remember, it is NEVER acceptable to pass
others work off as your own.

Paraphrasing or quoting another's work without citing the source is a form of
academic misconduct. Even inadvertent or unintentional misuse or appropriation
of another's work (such as relying heavily on source material that is not
acknowledged) is considered plagiarism. If you have any questions about using
and citing sources, you are expected to ask for clarification. My rule of thumb
is if I am in doubt, I cite.

By participating in this class, you agree to abide by the [student code of
conduct](http://www.montana.edu/policy/student_conduct/).  Please review the
policy.

## Policy on Assignments

All assignments must be submitted by the start of class on the due date. Late
assignments will not be accepted.

For descriptive assignments and reports, the submission should be typed up and
submitted as a PDF (LaTeX suggested but not required). For code assignments,
well organized source code with clear comments should be submitted.

## Classroom Etiquette

Except for note taking and coding, please keep electronic devices off during
class, they can be distractions to other students. Disruptions to the class will
result in you being asked to leave the lecture.

## Special needs information

If you have a documented disability for which you are or may be requesting an
accommodation(s), you are encouraged to contact me and Disability Services as
soon as possible.

## NCUR 2020

On Thursday, March 26th and Friday, March 27th, MSU will be hosting the National
Conference on Undergraduate Research (NCUR). During those two days, campus
facilities will be used to support this exciting event and students will have
the opportunity to attend, be inspired by, and participate in this once in a
lifetime learning and professional development opportunity. 

Classes are scheduled as usual on March 23-25th of that week, and on March 26
and March 27 while the conference is in session, we expect you to attend the
conference instead of coming to class.  Many of your undergraduate colleagues
from MSU will be joining students from across the country to present their
research and creative projects and we encourage you to support them. Although
regularly scheduled classes on 3/26 and 3/27 are canceled, you will NOT have
this week off.

Note that it is free for MSU students to attend the Conference.

Requirements for March 26 and 27:

At the Conference, you will have the opportunity to observe the impactful
research conducted by your peers at MSU and across the country. Please view
attending the conference as an important extension of your learning in this
course and I will do my best to make this experience a relevant part of the
course.  Therefore, there will be a short, graded assignment attached to
attending conference sessions that will be included in the final grade for the
course as part of Proj 1. 


