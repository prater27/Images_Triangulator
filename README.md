#Image triangultor
This code merges and adds minor improvements to the implementation of the Weighted Voronoi Stippling in openCV done by frankr0 (https://frankr0.github.io/CV/Weighted-Voronoi-Stippling) for stippling an image and uses the delanuator-cpp  developed by delfrrr also using openCV (https://github.com/delfrrr/delaunator-cpp) to create the Delanuy mesh based on the (previously) stippled points. The stippling can be skipped by giving the option -ot (or -onlyTriangulation), so the code can also by used to create directly a triangulation by giving the coordinates of the points. 
Different input parameters that can be added when calling:
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

It gives at outputs: stippled_image.jpg, stippled_points.txt (x y coordinates of the points), triangulated_image.jpg and triangles.txt (for each point in the triangle P1,P2,P3, gives the two coordinates x y: x1 y1 x2 y2 x3 y3).

# Weighted Voronoi Stippling

This is an implementation of the following article using OpenCV:

> Weighted Voronoi Stippling, Adrian Secord. In: Proceedings of the 2nd International Symposium on Non-photorealistic Animation and Rendering. NPAR ’02. ACM, 2002, pp. 37– 43.

## Result
![result](https://frankr0.github.io/CV/Weighted-Voronoi-Stippling)

## Pre-requisites
This implementation has been written and tested on Manjaro using the following packages:
- GCC 8.2.1
- OpenCV 4.0.0

## Usage
```
 Usage: Stippling [params] image 

	-?, -h, --help, --usage (value:true)
		print this message
	-N, -n, --number
		points number
	-d, --draw
		draw result repeatedly
	-e, --epoch
		epochs number
	-i, --inverse
		inverse image

	image
		image  for rendering
```
