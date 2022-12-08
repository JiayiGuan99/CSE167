# CSE167
Done by: Jiayi Guan, Yuheng Ge


*Code based on HW3-CSE167-FA22.*

**Image class:**

We created this Image class to store the RGB colors for each pixel in arrays of pixels. Calling Raytrace will assign pixel values to the image. This class includes properties of an Image object, which is width, height, and the array to store colors; default construtor and another constructor that takes in the width and height, initializes corresponding variables, and the pixels array needs to contain colors for all pixels, so the size of it is width * height. To show an image on screen, we store it as a texture and transfer it to the frame buffer. Then we created the draw function to display the image.

**Triangle class:**


We created the triangle class to store the position and color of each pixels. The reason why we used triangle instead of buffers is that triagnles are more memory efficient and also can be sorted. In this class, each triangle contains three vertices, three normal vectors and material pointer which represents the material of the area. One more reason why we used triangles instead of buffers is that triangle is the smallest two dimensional primitives, so that triangles are eariser to render.

**Intersection class:**

Intersection class contains the three variables,"P" the position of the intersection, "N" the surface normal vector, "V" the directiom to the incoming ray, "triangle" that contains the material information and "dist" which is the disctance to the source of ray . The class basically represents the intersection point of a ray with a triangle or a scence. We will be finding the color of each pixel with intersect and lights.

**RTGeometry class:**

This class was modified from geometry class that we replaced the buffers with triangles to make the memory more efficient and make it eaiser to render.

