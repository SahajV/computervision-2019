#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <list>
#include <iterator>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <stack>

using namespace std;

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
	bool equal(Point t2)
	{
		if (t2.x() == xP && t2.y() == yP)
			return true;
		else
			return false;
	}

	void print()
	{
		cout << "(";
		cout << xP;
		cout << ",";
		cout << yP;
		cout << ")";
	}
};

class PointPair
{
private:
	Point p1, p2;

public:
	PointPair(Point a, Point b)
	{
		p1 = a;
		p2 = b;
	}

	Point a()
	{
		return p1;
	}

	Point b()
	{
		return p2;
	}
};

const int width = 800, height = 800; //We define the resolution fo the image that we are making
int pic[width][height] = {0};		 //Make a Matrix that consists of all 0's

void printMatrix(int temp[width][height]) // Method to print a Matrix
{
	ofstream img("cv.ppm");
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
			if (pic[x][y] == 2)
				img << "255 0 0 ";
		}
		img << endl;
	}
}

void set_pixel(int x, int y) //Helper method for drawCircle that sets the point in the matrix
{

	if (x <= 800 && x >= 0 && y <= 800 && y >= 0)
	{
		pic[x][y] = 1;
	}
}

void set_Red_pixel(int x, int y) //Helper method for drawCircle that draws a red cirlce instead
{

	if (x <= 800 && x >= 0 && y <= 800 && y >= 0)
	{
		pic[x][y] = 2;
	}
}

void drawCircle(int radius, int r, int cx, int cy, bool red) //Method to draw a circle from radius and center
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

		if (red)
		{
			set_Red_pixel(cx + x, cy + y);
			set_Red_pixel(cx + x, cy - y);
			set_Red_pixel(cx - x, cy + y);
			set_Red_pixel(cx - x, cy - y);
			set_Red_pixel(cx + y, cy + x);
			set_Red_pixel(cx + y, cy - x);
			set_Red_pixel(cx - y, cy + x);
			set_Red_pixel(cx - y, cy - x);
			y2_new -= (2 * x) - 3;
		}
		else
		{
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
}

void drawPoints(vector<Point> randL)
{
	vector<Point>::iterator it;
	for (it = randL.begin(); it != randL.end(); it++)
	{
		double x = it->x();
		double y = it->y();

		drawCircle(2, 2, x * 800, y * 800, false);
	}
}

void drawRedPoints(vector<Point> randL)
{
	vector<Point>::iterator it;
	for (it = randL.begin(); it != randL.end(); it++)
	{
		double x = it->x();
		double y = it->y();

		drawCircle(2, 2, x * 800, y * 800, true);
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
		pic[x1][y1] = 2;

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
				pic[x1][y1] = 2;
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
				pic[x1][y1] = 2;
		}
	}
}

vector<Point> randomVector(int num)
{
	srand(time(0));
	vector<Point> randV;

	for (int x = 0; x < num; x++)
	{
		double a = rand() / ((double)(RAND_MAX + 1.0));
		double b = rand() / ((double)(RAND_MAX + 1.0));

		randV.push_back(Point(a, b));
	}

	return randV;
}

void printVector(vector<Point> vectorL)
{
	cout << "Beginning of Vector" << endl;
	for (auto it = vectorL.begin(); it != vectorL.end(); it++)
	{
		double x = (it->x());
		double y = (it->y());
		cout << x << " " << y << endl;
	}

	cout << "End of Vector" << endl;
}

vector<Point> swap(vector<Point> points, int n1, int n2)
{
	Point temp = points[n1];
	points[n1] = points[n2];
	points[n2] = temp;

	return points;
}

// bool compare(Point p1, Point p2)
// {
// 	return (p1.x() < p2.x());
// }

double distance(Point p1, Point p2)
{
	double dist = sqrt(pow(p2.x() - p1.x(), 2) + pow(p2.y() - p1.y(), 2));
	return dist;
}

Point p0;

double angle(Point p, Point p1)
{
	return atan2(p1.y() - p.y(), p1.x() - p.x());
}

bool compare(Point p1, Point p2)
{
	double p1A = angle(p0, p1);
	double p2A = angle(p0, p2);

	if (p1A < p2A)
		return true;
	else if (p1A > p2A)
		return false;
	else
	{
		if (distance(p0, p1) < distance(p0, p1))
			return true;
		else
			return false;
	}
}

Point peekUnder(stack<Point> stk)
{
	Point temp = stk.top();
	stk.pop();
	Point scnd = stk.top();
	stk.push(temp);
	return scnd;
}

int direction(Point p1, Point p2, Point p3)
{
	double length = (p2.y() - p1.y()) * (p3.x() - p2.x()) - (p2.x() - p1.x()) * (p3.y() - p2.y());

	if (length == 0)
		return 0;
	else
		return (length > 0) ? 1 : 2;
}

void convexHull(vector<Point> points, int n)
{
	int bottomInd = 0;

	for (int i = 1; i < n; i++)
	{
		if (points[i].y() < points[bottomInd].y())
		{
			bottomInd = i;
		}
	}
	points = swap(points, 0, bottomInd);
	// cout << bottomVal << endl;

	p0 = points[0];
	sort(next(points.begin()), points.end(), compare);

	// printVector(points);

	int newSize = 1;
	for (int i = 1; i < n; i++)
	{
		while (i < n - 1 && direction(p0, points[i], points[i + 1]) == 0)
			i++;

		points[newSize] = points[i];
		newSize++;
	}

	if (newSize < 3)
		return;

	stack<Point> stk;
	stk.push(points[0]);
	stk.push(points[1]);
	stk.push(points[2]);

	for (int i = 3; i < newSize; i++)
	{
		while (direction(peekUnder(stk), stk.top(), points[i]) != 2)
			stk.pop();
		stk.push(points[i]);
	}

	vector<Point> testV;

	while (!stk.empty())
	{
		Point p = stk.top();
		testV.push_back(p);
		stk.pop();
	}

	for (int i = 1; i < testV.size(); i++)
	{
		bresenham(testV[i - 1].x() * 800, testV[i - 1].y() * 800, testV[i].x() * 800, testV[i].y() * 800);
	}
	bresenham(testV[testV.size() - 1].x() * 800, testV[testV.size() - 1].y() * 800, testV[0].x() * 800, testV[0].y() * 800);

	drawRedPoints(testV);
	printVector(testV);
}

int main()
{
	vector<Point> randV = randomVector(50);
	drawPoints(randV);
	convexHull(randV, randV.size());

	printMatrix(pic);
	return 0;
}