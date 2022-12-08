# CSE167
Done by: Jiayi Guan, Yuheng Ge


*Code based on HW3-CSE167-FA22.*

**Image:**

We created this Image class to store the RGB colors for each pixel in arrays of pixels. Calling Raytrace will assign pixel values to the image. This class includes properties of an Image object, which is width, height, and the array to store colors; default construtor and another constructor that takes in the width and height, initializes corresponding variables, and the pixels array needs to contain colors for all pixels, so the size of it is width * height. To show an image on screen, we store it as a texture and transfer it to the frame buffer. Then we created the draw function to display the image.

**Triangle:**

We created the triangle class to store the position and color of each pixels. The reason why we used triangle instead of buffers is that triagnles are more memory efficient and also can be sorted. In this class, each triangle contains three vertices, three normal vectors and material pointer which represents the material of the area. One more reason why we used triangles instead of buffers is that triangle is the smallest two dimensional primitives, so that triangles are eariser to render.

**Ray:**

Ray class contains two variables, basepoint and a direction. As basepoint is where the ray is shotting out from in dir direction.

**Intersection:**

Intersection class contains the three variables,"P" the position of the intersection, "N" the surface normal vector, "V" the directiom to the incoming ray, "triangle" that contains the material information and "dist" which is the disctance to the source of ray . The class basically represents the intersection point of a ray with a triangle or a scence. We will be finding the color of each pixel with intersect and lights.

**RTGeometry:**

This class was modified from geometry class that we replaced the buffers with triangles to make the memory more efficient and make it eaiser to render.

**RTCube:**

In this class, we assign/initialize the 3 positions and 3 normals for each triangle with given datas, and insert each triangle into elements defined in RTGeometry.

**RTObj:**

RTObj.h is modified as RTGeometry is created.
In RTObj.cpp, we wrote the post processing of triangles. Similar to RTCube.h, given datas, we store the positions and normals at corresponding index into each triangle and insert it into elements list.

**RTScene:**

In RTScene, we traverse through a scene in camera view using stack and dfs algorithm which is same as HW3. When drawing all the models, we are working on world coordinate, using the modelview matrix * position and inverse transpose * normal to transform the positions and normals correctly, as discussed in Lighting lecture.

**RayTracer:**

In Raytracer, we have a basic function call that calls: RayThruPixel - generates a ray originated from the camera position through the center of the (i,j) pixel into the world; Intersect(ray, scene) - searches over all geometries in the scene and returns the closest hit, it calls Intersect(ray, triangle) that performs the Ray-triangle intersection discussed in class; FindColor - shade the light color seen by the in-coming ray which is basically the same as HW3's lighting fragment, but in world coordinate, for each light, we generate its color, also for specular we loop on recursion_depth that is the number of reflected lights, adding up the color in the end will give us the right result.
