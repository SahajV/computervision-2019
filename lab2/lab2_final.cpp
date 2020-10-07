#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
double pointsArr[8];                 //Array that will house the random points that were generated
const int width = 800, height = 800; //We define the resolution fo the image that we are making
int pic[width][height] = {0};        //Make a Matrix that consists of all 0's

void printMatrix(int temp[width][height]) // Method to print a Matrix
{
  ofstream img("output.ppm");
  img << "P3"
      << "  " << width << "  " << height << "  "
      << "1" << endl;
  for (int x = 0; x < height; x++)
  {
    for (int y = 0; y < width; y++)
    {
      if (pic[x][y] == 0)
        img << "0 0 0 ";
      if (pic[x][y] == 1)
        img << "1 1 1 ";
    }
    img << endl;
  }
}

void bresenham(int x1, int y1, int x2, int y2) // The algorithm to draw a line
{
  int diffX, diffY;
  int sX, sY;

  diffX = x2 - x1;
  diffY = y2 - y1;

  if (!(diffY >= 0))
  {
    diffY = -1 * diffY;
    sY = -1;
  }
  else
  {
    sY = 1;
  }

  if (!(diffX >= 0))
  {
    diffX = -1 * diffX;
    sX = -1;
  }
  else
  {
    sX = 1;
  }

  diffY = diffY * 2;
  diffX = diffX * 2;

  if (!!((0 <= x1) && (x1 < 800) && (0 <= y1) && (y1 < 800)))
    pic[x1][y1] = 1;

  if (!(diffX <= diffY))
  {
    int frac = diffY - (diffX >> 1);

    while (!(x1 == x2))
    {
      x1 = x1 + sX;
      if (!(frac < 0))
      {
        y1 = y1 + sY;
        frac -= diffX;
      }
      frac = frac + diffY;
      if (!!((0 <= x1) && (x1 < 800) && (0 <= y1) && (y1 < 800)))
        pic[x1][y1] = 1;
    }
  }
  else
  {
    int frac = diffX - (diffY >> 1);

    while (!(y1 == y2))
    {
      if (!(frac < 0))
      {

        x1 = x1 + sX;
        frac -= diffY;
      }
      y1 = y1 + sY;
      frac = frac + diffX;
      if (!!((0 <= x1) && (x1 < 800) && (0 <= y1) && (y1 < 800)))
        pic[x1][y1] = 1;
    }
  }
}

void set_pixel(int x, int y) //Helper method for drawCircle that sets the point in the matrix
{

  if (x <= 800 && x >= 0 && y <= 800 && y >= 0)
  {
    pic[x][y] = 1;
  }
}

void drawCircle(int radius, int r, int cx, int cy) //Method to draw a circle from radius and center
{
  int x, y, xmax, y2, y2_new, ty;
  xmax = (int)(radius * 0.70710678); // maximum x at radius/sqrt(2)
  y = r;
  y2 = y * y;
  ty = (2 * y) - 1;
  y2_new = y2;
  for (x = 0; x <= xmax; x++)
  {
    if ((y2 - y2_new) >= ty)
    {
      y2 -= ty;
      y -= 1;
      ty -= 2;
    }

    set_pixel(cx + x, cy + y);
    set_pixel(cx + x, cy - y);
    set_pixel(cx - x, cy + y);
    set_pixel(cx - x, cy - y);
    set_pixel(cx + y, cy + x);
    set_pixel(cx + y, cy - x);
    set_pixel(cx - y, cy + x);
    set_pixel(cx - y, cy - x);
    y2_new -= (2 * x) - 3;
  }
}

class Point // This class will help a lot when I need to return a point for a method. (I don't need two methods to return x and y)
{
private:
  double xP, yP;

public:
  Point(double x = 0.0, double y = 0.0)
  {
    xP = x;
    yP = y;
  }

  double x()
  {
    return xP;
  }
  double y()
  {
    return yP;
  }
};

double distance(double x1, double y1, double x2, double y2) //helper method to find the distance between two points
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double intercept(double slope, double x, double y) //Helper method for findVertices that will help me find the equation by giving the itnercept of the equation
{
  return y - slope * x;
}

void DrawFullLine(double x1, double y1, double x2, double y2)
{
  double slope1 = (y2 - y1) / (x2 - x1);
  double inter1 = intercept(slope1, x1, y1);
  double py1_1 = slope1 * 0 + inter1;
  double py1_2 = slope1 * 0.99875 + inter1;
  int pI1_1 = round(py1_1 * 800);
  int pI1_2 = round(py1_2 * 800);
  bresenham(0, pI1_1, 799, pI1_2);
}

class Square //This class is used to easily store and outpout the squares that I find
{
private:
  Point vP1, vP2, vP3, vP4;

public:
  Square(Point v1, Point v2, Point v3, Point v4)
  {
    vP1 = v1;
    vP2 = v2;
    vP3 = v3;
    vP4 = v4;
  }

  Point v1()
  {
    return vP1;
  }
  Point v2()
  {
    return vP2;
  }
  Point v3()
  {
    return vP3;
  }
  Point v4()
  {
    return vP4;
  }
  double area()
  {
    double side = distance(vP1.x(), vP1.y(), vP2.x(), vP2.y());
    double ar = pow(side, 2);
    return ar;
  }

  void draw()
  {

    double x1 = vP1.x();
    double y1 = vP1.y();
    double x2 = vP2.x();
    double y2 = vP2.y();
    double x3 = vP3.x();
    double y3 = vP3.y();
    double x4 = vP4.x();
    double y4 = vP4.y();

    DrawFullLine(x1, y1, x2, y2);
    DrawFullLine(x2, y2, x3, y3);
    DrawFullLine(x3, y3, x4, y4);
    DrawFullLine(x4, y4, x1, y1);
  }
};

double findArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool isPresent(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y) //Checks if point inside (part1 helper)
{
  double A = findArea(x1, y1, x2, y2, x3, y3);

  double A1 = findArea(x, y, x2, y2, x3, y3);
  double A2 = findArea(x1, y1, x, y, x3, y3);
  double A3 = findArea(x1, y1, x2, y2, x, y);

  return (A == A1 + A2 + A3);
}

bool collin(double x1, double y1, double x2, double y2, double x3, double y3) //Helper method to chick if lines are collinear
{
  double a = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

  return (a == 0);
}

void part1()
{
  srand(time(0));

  double tx1 = rand() / ((double)(RAND_MAX + 1.0));
  double ty1 = rand() / ((double)(RAND_MAX + 1.0));

  double tx2 = rand() / ((double)(RAND_MAX + 1.0));
  double ty2 = rand() / ((double)(RAND_MAX + 1.0));

  double tx3 = rand() / ((double)(RAND_MAX + 1.0));
  double ty3 = rand() / ((double)(RAND_MAX + 1.0));

  double tx4 = rand() / ((double)(RAND_MAX + 1.0));
  double ty4 = rand() / ((double)(RAND_MAX + 1.0));

  while (collin(tx1, ty1, tx2, ty2, tx3, ty3))
  {
    tx1 = rand() / ((double)(RAND_MAX + 1.0));
    ty1 = rand() / ((double)(RAND_MAX + 1.0));

    tx2 = rand() / ((double)(RAND_MAX + 1.0));
    ty2 = rand() / ((double)(RAND_MAX + 1.0));

    tx3 = rand() / ((double)(RAND_MAX + 1.0));
    ty3 = rand() / ((double)(RAND_MAX + 1.0));
  }

  while (isPresent(tx1, ty1, tx2, ty2, tx3, ty3, tx4, ty4))
  {
    tx4 = rand() / ((double)(RAND_MAX + 1.0));
    ty4 = rand() / ((double)(RAND_MAX + 1.0));
  }

  ofstream myfile;
  myfile.open("points.txt");

  myfile.precision(17);

  myfile << "(";
  myfile << tx1;
  myfile << ",";
  myfile << ty1;

  myfile << ") , (";
  myfile << tx2;
  myfile << ",";
  myfile << ty2;

  myfile << ") , (";
  myfile << tx3;
  myfile << ",";
  myfile << ty3;

  myfile << ") , (";
  myfile << tx4;
  myfile << ",";
  myfile << ty4;
  myfile << ")" << endl;

  myfile.close();
}

void readFile(string fileName)
{
  string line, sout;
  ifstream file(fileName);
  if (file.is_open())
  {
    while (getline(file, line))
    {
      sout += line + '\n';
    }
    file.close();

    sscanf(sout.c_str(), "(%lf,%lf) , (%lf,%lf) , (%lf,%lf) , (%lf,%lf)", &pointsArr[0], &pointsArr[1],
           &pointsArr[2], &pointsArr[3], &pointsArr[4], &pointsArr[5], &pointsArr[6], &pointsArr[7]); //I don't know regex so sscanf was used
  }
  else
    cout << "FIle was not able to be opened"; //I added this but the lab said that we can assume the problem is well defined
}

Point intersect(double s1, double i1, double s2, double i2) //Method to find the intersect between two lines
{
  double x = (i1 - i2) / (s2 - s1);
  double y = x * s1 + i1;
  Point re(x, y);
  return re;
}

Point findOtherPoint(Point source, Point check, double l, double m) //Method to find E
{
  Point a, b;
  double x1, y1, x2, y2;

  if (m == 0)
  {
    x1 = source.x() + l;
    y1 = source.y();

    x2 = source.x() - 1;
    y2 = source.y();
  }
  else
  {
    double dx = (l / sqrt(1 + (m * m)));
    double dy = m * dx;
    x1 = source.x() + dx;
    y1 = source.y() + dy;
    x2 = source.x() - dx;
    y2 = source.y() - dy;
  }

  double d1 = distance(x1, y1, check.x(), check.y());
  double d2 = distance(x2, y2, check.x(), check.y());

  if (d1 < d2)
  {
    return Point(x1, y1);
  }
  else
  {
    return Point(x2, y2);
  }
}

Point smallFindOtherPoint(Point source, Point check, double l, double m) //Method to find the second possible E point
{
  Point a, b;
  double x1, y1, x2, y2;

  // slope is 0
  if (m == 0)
  {
    x1 = source.x() + l;
    y1 = source.y();

    x2 = source.x() - 1;
    y2 = source.y();
  }
  else
  {
    double dx = (l / sqrt(1 + (m * m)));
    double dy = m * dx;
    x1 = source.x() + dx;
    y1 = source.y() + dy;
    x2 = source.x() - dx;
    y2 = source.y() - dy;
  }

  double d1 = distance(x1, y1, check.x(), check.y());
  double d2 = distance(x2, y2, check.x(), check.y());

  if (d1 > d2)
  {
    return Point(x1, y1);
  }
  else
  {
    return Point(x2, y2);
  }
}

Square findVertices(Point A, Point B, Point C, Point D) //We assume that the problem is well defined so no infinite solutions
{
  //https://www.cut-the-knot.org/Curriculum/Geometry/GeoGebra/SquareFromFourPoints1.shtml
  //following this algorithm for the process of finding the vertices

  double x1 = A.x();
  double y1 = A.y();

  double x2 = B.x();
  double y2 = B.y();

  double x3 = C.x();
  double y3 = C.y();

  double x4 = D.x();
  double y4 = D.y();

  double slopeAC = (y3 - y1) / (x3 - x1);
  double slopeBE = -1 * ((x3 - x1) / (y3 - y1));

  double interAC = intercept(slopeAC, x3, y3);
  double interBE = intercept(slopeBE, x2, y2);

  //The distance from A to C has to be the same as the distance from B to E
  double distanceAC = distance(x1, y1, x3, y3);

  Point source(x2, y2);

  Point check(x4, y4);

  Point E = findOtherPoint(source, check, distanceAC, slopeBE);

  double slopeDE = (y4 - E.y()) / (x4 - E.x());
  double pSlope = -1 * ((x4 - E.x()) / (y4 - E.y()));

  double iD = intercept(slopeDE, x4, y4);
  double iB = intercept(slopeDE, x2, y2);
  double iA = intercept(pSlope, x1, y1);
  double iC = intercept(pSlope, x3, y3);

  Point vert1 = intersect(slopeDE, iD, pSlope, iA);
  Point vert2 = intersect(slopeDE, iB, pSlope, iA);
  Point vert3 = intersect(slopeDE, iB, pSlope, iC);
  Point vert4 = intersect(slopeDE, iD, pSlope, iC);

  Square finalSquare(vert1, vert2, vert3, vert4);
  return finalSquare;
}

Square smallFindVertices(Point A, Point B, Point C, Point D) //Same method as above but finds the second possible square from the same set of points
{
  double x1 = A.x();
  double y1 = A.y();
  double x2 = B.x();
  double y2 = B.y();
  double x3 = C.x();
  double y3 = C.y();
  double x4 = D.x();
  double y4 = D.y();

  double slopeAC = (y3 - y1) / (x3 - x1);
  double slopeBE = -1 * ((x3 - x1) / (y3 - y1));

  double interAC = intercept(slopeAC, x3, y3);
  double interBE = intercept(slopeBE, x2, y2);

  double distanceAC = distance(x1, y1, x3, y3);

  Point source(x2, y2);

  Point check(x4, y4);

  Point E = smallFindOtherPoint(source, check, distanceAC, slopeBE);

  double slopeDE = (y4 - E.y()) / (x4 - E.x());
  double pSlope = -1 * ((x4 - E.x()) / (y4 - E.y()));

  double iD = intercept(slopeDE, x4, y4);
  double iB = intercept(slopeDE, x2, y2);
  double iA = intercept(pSlope, x1, y1);
  double iC = intercept(pSlope, x3, y3);

  Point vert1 = intersect(slopeDE, iD, pSlope, iA);
  Point vert2 = intersect(slopeDE, iB, pSlope, iA);
  Point vert3 = intersect(slopeDE, iB, pSlope, iC);
  Point vert4 = intersect(slopeDE, iD, pSlope, iC);

  Square finalSquare(vert1, vert2, vert3, vert4);
  return finalSquare;
}

int findSmallestElement(double arr[], int n) //Helper method that finds the smallest area for us
{
  int temp = 0;
  for (int i = 0; i < n; i++)
  {
    if (arr[temp] > arr[i])
    {
      temp = i;
    }
  }
  return temp;
}

void drawPoints(double pointsArr[8]) //Method that displays the circles with radius = 2
{
  double x1 = pointsArr[0];
  double y1 = pointsArr[1];
  drawCircle(2, 2, pointsArr[0] * 800, pointsArr[1] * 800);

  double x2 = pointsArr[2];
  double y2 = pointsArr[3];
  drawCircle(2, 2, pointsArr[2] * 800, pointsArr[3] * 800);

  double x3 = pointsArr[4];
  double y3 = pointsArr[5];
  drawCircle(2, 2, pointsArr[4] * 800, pointsArr[5] * 800);

  double x4 = pointsArr[6];
  double y4 = pointsArr[7];
  drawCircle(2, 2, pointsArr[6] * 800, pointsArr[7] * 800);
}

void part2()
{
  readFile("points.txt");
  drawPoints(pointsArr);

  Point A(pointsArr[0], pointsArr[1]);
  Point B(pointsArr[2], pointsArr[3]);
  Point C(pointsArr[4], pointsArr[5]);
  Point D(pointsArr[6], pointsArr[7]);

  Square s1 = findVertices(A, B, C, D);
  Square s2 = smallFindVertices(A, B, C, D);

  Square s3 = findVertices(A, C, B, D);
  Square s4 = smallFindVertices(A, C, B, D);

  Square s5 = findVertices(A, B, D, C);
  Square s6 = smallFindVertices(A, B, D, C);

  double allAreas[6] = {s1.area(), s2.area(), s3.area(), s4.area(), s5.area(), s6.area()};
  Square allSquares[6] = {s1, s2, s3, s4, s5, s6};
  int smallestArea = findSmallestElement(allAreas, 6);
  allSquares[smallestArea].draw();
  cout << "Smallest Area: " << allSquares[smallestArea].area() << endl;

  ofstream myfile; // We begin to print the values into the output.txt file here
  myfile.open("output.txt");
  myfile.precision(17);

  //This is the part that prints out the points
  myfile << "(";
  myfile << pointsArr[0];
  myfile << ",";
  myfile << pointsArr[1];

  myfile << ") , (";
  myfile << pointsArr[2];
  myfile << ",";
  myfile << pointsArr[3];

  myfile << ") , (";
  myfile << pointsArr[4];
  myfile << ",";
  myfile << pointsArr[5];

  myfile << ") , (";
  myfile << pointsArr[6];
  myfile << ",";
  myfile << pointsArr[7];
  myfile << ")" << endl;

  for (int w = 0; w < 6; w++) //This prints out all the details about each square
  {
    myfile << "(";
    myfile << allSquares[w].v1().x();
    myfile << ",";
    myfile << allSquares[w].v1().y();

    myfile << ") , (";
    myfile << allSquares[w].v2().x();
    myfile << ",";
    myfile << allSquares[w].v2().y();

    myfile << ") , (";
    myfile << allSquares[w].v3().x();
    myfile << ",";
    myfile << allSquares[w].v3().y();

    myfile << ") , (";
    myfile << allSquares[w].v4().x();
    myfile << ",";
    myfile << allSquares[w].v4().y();
    myfile << ") Area = ";
    myfile << allSquares[w].area() << endl;
  }

  myfile.close();
}

int main()
{
  part1();
  part2();

  printMatrix(pic);
  //system("output.ppm");

  return 0;
}