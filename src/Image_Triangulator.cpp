#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include <fstream>
#include <sstream>
#include <cstdio>

#include "delaunator.hpp"
#include "CVT.h"

using namespace cv;
using namespace std;

bool parserCommand(int argc, char ** argv, Mat &img , int &N, int &E,int &pointSize, int &lineSize, bool &drawRepeat, bool &drawTriangulation, bool &onlyTriangulation) {
	const String keys =
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
	    ;

	CommandLineParser parser(argc, argv, keys);
	parser.about("Weighted Voronoi Redering.");
	if (parser.has("help")) {
		parser.printMessage();
		return false;
	}

	// Open Image.
	String imgPath = parser.get<String>(0);
	if (imgPath.empty() && !parser.has("onlyTriangulation")) {
		parser.printMessage();
		return false;
	}

	img = imread(imgPath);
	if (img.empty() && !parser.has("onlyTriangulation")) {
		cout << "Error Loading Image" << endl;
		return false;
	}

	if(!img.empty()){
		cvtColor(img, img, COLOR_BGR2GRAY);
		const Size size = img.size();
	// Set Points Number.
		N = (size.height + size.width) * 2;
			if (parser.has("n"))
				N = parser.get<int>("n");
	}		

	// Inverse.
	if (parser.has("inverse"))
		img = ~img;

	// Set Epochs Number.
	E = 100;
	if (parser.has("epoch"))
		E = parser.get<int>("epoch");
	
	// Set Point Size.
	pointSize = 1;
	if (parser.has("pointSize"))
		pointSize = parser.get<int>("pointSize");

	// Set Point Size.
	lineSize = 1;
	if (parser.has("lineSize"))
		lineSize = parser.get<int>("lineSize");

	// Show Stippling Processing.
	drawRepeat = false;
	if (parser.has("draw"))
		drawRepeat = true;

	// Show Triangulation Process
	drawTriangulation = false;
	if (parser.has("drawTriangulation"))
		drawTriangulation = true;

	// Show Stippling Processing.
	onlyTriangulation = false;
	if (parser.has("onlyTriangulation"))
		onlyTriangulation = true;

	// Check Parser Error.
	if (!parser.check()) {
		parser.printErrors();
		return false;
	}

	return true;
}


void appendRandomPoint(RNG &rng, vector<Point2f> &points, Size size, int N) {
	for (int i = 0; i < N; ++i) {
		float x = rng.uniform((float)0, (float)size.width - 1);
		float y = rng.uniform((float)0, (float)size.height - 1);
		points.push_back(Point2f(x, y));
	}
}

//Method to check during reading the stippled_points.txt file in case the stippling process was skipped (-ot (or -onlyTriangulation) option) that the give image as outpupt is big enough to contain all
//the input points
bool pointInsideImage(int x2, int y2, double x, double y) 
{ 
    if (x >= 0 and x <= x2 and y >= 0 and y <= y2) 
        return true; 
  
    return false; 
} 

int main(int argc, char ** argv) {

//Beggining of the code for the stippling
	RNG rng(time(0));
	Mat img;
	int N, E, pointSize, lineSize;
	bool drawRepeat, drawTriangulation, onlyTriangulation;
	ofstream myfile;
	Mat imgTriangulated;

	if (!parserCommand(argc, argv, img, N, E, pointSize, lineSize, drawRepeat, drawTriangulation, onlyTriangulation )) {
		return -1;
	}

	// Point Set.
	vector<Point2f> points;

if(!onlyTriangulation){

	if( remove( "stippled_points.txt" ) != 0 )
 		perror( "Error deleting stippled_points.txt:" );
 	else
   		puts( "File stippled_points.txt successfully deleted" );

   	puts( "Stippling starts \n" );

	const Size size = img.size();


	// To avoid the Density of dark region being too small.
	img = img * 254 / 255 + 1;


	// Add Points.
	appendRandomPoint(rng, points, size, 200);

	Mat imgVoronoi(size.height, size.width, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < E; ++i) {

		// Append Random Points.
		if (i < (N / 200))
			appendRandomPoint(rng, points, size, 200);

		// Remove Out of range Points.
		for (auto i = points.begin(); i != points.end(); ++i) {
			if (i->x > size.width || i->x < 0 || i->y > size.height || i->y < 0) {
				points.erase(i);
				i--;
			}
		}

		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		imgVoronoi = Scalar(255, 255, 255);
		points = CVT::drawVoronoi(img, imgVoronoi, subdiv, pointSize);

		// Show Image.
		if (drawRepeat) {
			waitKey(10);
			imshow( "imgVoronoi", imgVoronoi);

		}

		cout << "(" << i << "/" << E << ")" << endl;

	}
	cout << endl << "done." << endl;

	imwrite("stippled_image.jpg", imgVoronoi);

	myfile.open ("stippled_points.txt");

	for (auto &Point2f : points) // access by reference to avoid copying
	{  
	//Checks that the actual value obtained it is a point within the image and not outside (there can appear some very big numbers due to numerical singularities)
		if(Point2f.x < size.width && Point2f.y<size.height ){
			myfile << Point2f.x << " " << Point2f.y << "\n";
		}
	}

	myfile.close();

std::puts( "Stippling done!");
std::puts( "The created file stippled_points.txt contains the (x,y) coordinates of the stippling points, and stippled_image was also created");
}
else
{
std::puts( "Stippling skipped! Proceding directly to the triangulation of the input points contained in the file given by the user stippled_points.txt" );
}
//End of the code for the stippling



//Beggining of the code correspoding to the triangulation

puts( "Triangulation starts: \n" );

//This object will be passed to the Delaunator
std::vector<double> coords;

//Normal processing
if (!onlyTriangulation){
Size size = img.size();
// Creates the image that will be given as triangulation output
imgTriangulated = Mat(size.height, size.width, CV_32F);
}
//Walkaround in case we only wanted to do the triangulation, skipping the stippling process
else{
int height;
int width;
bool imageBigEnough=false;

std::ifstream file("stippled_points.txt");
std::string str;

//Converting the original pair of coordinates obtained by the stippling into a vector of non-differenciated-by-points coordinates that Delanuator takes as input (coords), reading the from a file called 
//stippled_points.txt with two columns with the format "xCoordinate yCoordinate" and each row being one point

std::string   fileLine;
int linesCounter=0;
while(std::getline(file, fileLine))
{
linesCounter++;
        std::stringstream  lineStream(fileLine);

        double value;
        while(lineStream >> value)
        {
            coords.push_back(value);
        }
}

//Checks that the file was correctly read
if(coords.size()/2 != linesCounter)
{
cout << "There is an error in your input file stippled_points.txt and not all the points were well read. Please check the file and try again!\n \n \n";
return 0;
}

//The previously read coordinates are transformed into pairs of points
int counter=0;

for(int i=0; i<coords.size(); i++){
	counter++;
	if(counter==2){
		Point2f point(coords[i-1],coords[i]);
		points.push_back(point);
		counter=0;
	}
}


while(1) {

cout<< "\nEnter the height of the output image (integer number of pixels)\n";
cin>>height;

while(1)
{
if(cin.fail())
{
cin.clear();
cin.ignore(numeric_limits<streamsize>::max(),'\n');
cout<<"You have entered wrong input. Try again to enter the height of the output image (integer number of pixels)"<<endl;
cin>>height;
}
if(!cin.fail())
{
cout<<"\n";
break;
}
}

cout<<"\nEnter the width of the output image (integer number of pixels)\n";
cin>>width;
while(1)
{
if(cin.fail())
{
cin.clear();
cin.ignore(numeric_limits<streamsize>::max(),'\n');
cout<<"You have entered wrong input. Try again to enter the width of the output image (integer number of pixels)"<<endl;
cin>>height;
}
if(!cin.fail())
{
cout<<"\n";
break;
}
}

for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
{
Point2f pointAux = *it;
if (!pointInsideImage(width, height, pointAux.x, pointAux.y))
{
		cout << "The size of the image you gave is not big enough to contain the points. Please try again!\n \n \n \n \n";
		imageBigEnough=false;
		break;
}
else
	imageBigEnough=true;

}

if(imageBigEnough)
	break;


}

imgTriangulated = Mat(height, width, CV_32F);
}
//End of the walkaround in case we want to skip the stippling process


//The background color for the image is set here
imgTriangulated.setTo(cv::Scalar(255,255,255));
vector<Point> pt(3);
Size size = imgTriangulated.size();

//The color of the triangles is set by delaunay_colorLines
Scalar delaunay_colorLines(0,0,0);

string win_delaunay = "Delanuy dynamic window";

if(!onlyTriangulation)
{
//Converting the original pair of coordinates obtained by the stippling into a vector of non-differenciated-by-points coordinates that Delanuator takes as input (coords)
for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
{
	Point2f pointAux= *it;
	coords.push_back(pointAux.x);
	coords.push_back(pointAux.y);
}
}
//triangulation happens here
delaunator::Delaunator d(coords);

//Here it draws the image with the delanuy tesselation and creates a file "triangles.txt" containing the triangles generated
if( std::remove( "triangles.txt" ) != 0 )
	std::perror( "Error deleting previous triangles.txt file (not found)" );
 	else
   		std::puts( "Previous triangles.txt successfully deleted" );

		std::ofstream myFileTriangulation;
		myFileTriangulation.open ("triangles.txt");

	for(std::size_t i = 0; i < d.triangles.size(); i+=3) {

		pt[0] = Point(d.coords[2 * d.triangles[i]], d.coords[2 * d.triangles[i]+1]);
		pt[1] = Point(d.coords[2 * d.triangles[i + 1]], d.coords[2 * d.triangles[i + 1]+1]);
		pt[2] = Point(d.coords[2 * d.triangles[i + 2]], d.coords[2 * d.triangles[i + 2]+1]);

		if(((pt[0].x>0 && pt[0].x<size.width) && (pt[1].x>0 && pt[1].x<size.width) && (pt[2].x>0 && pt[2].x<size.width) && (pt[0].y>0 && pt[0].y<size.height) && (pt[1].y>0 && pt[1].y<size.height) && (pt[2].y>0 && pt[2].y<size.height)))
		{
		line(imgTriangulated, pt[0], pt[1], delaunay_colorLines, lineSize, CV_8S, 0);
		line(imgTriangulated, pt[1], pt[2], delaunay_colorLines, lineSize, CV_8S, 0);
		line(imgTriangulated, pt[2], pt[0], delaunay_colorLines, lineSize, CV_8S, 0);
       
		if (drawTriangulation)
     		{
			imshow(win_delaunay, imgTriangulated);
			waitKey(1);
    		}
	
		myFileTriangulation << d.coords[2 * d.triangles[i]] << " " <<  d.coords[2 * d.triangles[i] + 1] << " " <<
		  d.coords[2 * d.triangles[i+1]] << " " <<  d.coords[2 * d.triangles[i+1] + 1] << " " <<
		  d.coords[2 * d.triangles[i+2]] << " " <<  d.coords[2 * d.triangles[i+2] + 1] << "\n";
		}
    }

myFileTriangulation.close();

//Writes the triangulated image
imwrite( "Triangulated_Image.jpg", imgTriangulated);

std::puts( "Triangulation done!" );
std::puts( "The created file trinagles.txt contains the two coordinates x,y of each point P1 P2 P3 for each triangle: P1x P1y P2x P2y P3x P3y");

return 0;
}
