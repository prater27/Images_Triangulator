This code merges and adds minor improvements to the implementation of the Weighted Voronoi Stippling in openCV done by frankr0 (https://frankr0.github.io/CV/Weighted-Voronoi-Stippling) for stippling an image and uses the delanuator-cpp  developed by delfrrr also using openCV (https://github.com/delfrrr/delaunator-cpp) to create the Delanuy mesh based on the (previously) stippled points. The stippling can be skipped by giving the option -ot (or -onlyTriangulation) if stippled points are provided, so the code can also by used to create directly a triangulation by giving the coordinates of the points. 
Different input parameters that can be added when calling:
>
	    "{help h usage ? |      | print this message      }"
	    "{@image         |      | image for stippling     }"
	    "{N n number     |      | points amount (-N= -n= , integer value}"
	    "{inverse i      |      | inverse image, usually needed to get better results}"
	    "{epoch e        |      | epochs (-e= -epoch=, integer value}"
	    "{pointSize ps   |      | point size (-ps= or -pointSize= , integer value)}"
	    "{lineSize ls    |      | line size (-ls= or -lineSize= , integer value)}"
	    "{draw d         |      | show iterate processing (proccess of stippling)}"
	    "{drawTriangulation dt  |      | show the triangulation process }"
	    "{onlyTriangulation ot  |      | skips the stippling process }"
	    

It gives as outputs: stippled_image.jpg, stippled_points.txt (x y coordinates of the points), triangulated_image.jpg and triangles.txt (for each point in the triangle P1,P2,P3, gives the two coordinates x y: x1 y1 x2 y2 x3 y3).

# Weighted Voronoi Stippling

This is an implementation by frankr0 of the following article using OpenCV:

> Weighted Voronoi Stippling, Adrian Secord. In: Proceedings of the 2nd International Symposium on Non-photorealistic Animation and Rendering. NPAR ’02. ACM, 2002, pp. 37– 43.

# Delaunay-Triangulation

An implementation by delfrrr. It claims to be one of the fastest open source implementations available.

# Pre-requisites
It uses the following packages:
- GCC version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1) 
- OpenCV 4.0.0

# Example of use and results
https://github.com/prater27/Images_Triangulator/tree/master/Results

![alt text](https://github.com/prater27/Images_Triangulator/blob/master/Results/stippled_image1.jpg)

![alt text](https://github.com/prater27/Images_Triangulator/blob/master/Results/Triangulated_Image1.jpg "Example_1")



