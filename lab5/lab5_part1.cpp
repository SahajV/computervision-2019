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
int pic[width][height] = {0};        //Make a Matrix that consists of all 0's

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

void fillFile(int num)
{
    srand(time(0));

    ofstream myfile;
    myfile.open("points.txt");
    myfile.precision(17);

    for (int x = 0; x < num; x++)
    {
        double xR = rand() / ((double)(RAND_MAX + 1.0));
        double yR = rand() / ((double)(RAND_MAX + 1.0));
        myfile << xR << " " << yR << endl;
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
        double x = it->x();
        double y = it->y();
        cout << x << " " << y << endl;
    }

    cout << "End of Vector" << endl;
}

int minMax(vector<Point> points, int n)
{

    int maxX = 0;
    int minX = 0;

    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].x() > points[maxX].x())
            maxX = i;
        if (points[i].x() < points[minX].x())
            minX = i;
    }

    if (n == 1)
        return maxX;
    else
        return minX;
}

vector<Point> finalHull;
vector<Point> setHull;

double distance(Point p1, Point p2, Point p3)
{
    double dist = (p3.y() - p1.y()) * (p2.x() - p1.x()) - (p2.y() - p1.y()) * (p3.x() - p1.x());
    return dist;
}

int side(Point p1, Point p2, Point p3)
{
    double dist = (p3.y() - p1.y()) * (p2.x() - p1.x()) - (p2.y() - p1.y()) * (p3.x() - p1.x());
    // p1.print();
    // cout << endl;
    // p2.print();
    // cout << endl;
    // p3.print();
    // cout << endl;

    if (dist < 0)
        return -1;
    if (dist > 0)
        return 1;
    return 0;
}

double intercept(double slope, double x, double y) //Helper method for findVertices that will help me find the equation by giving the itnercept of the equation
{
    return y - slope * x;
}

void recurHull(vector<Point> points, Point p1, Point p2, int leftRight, int n)
{
    double maxDistance = 0.0;
    int temp = -1;

    for (int i = 0; i < n; i++)
    {
        double temp2 = abs(distance(p1, p2, points[i]));
        // cout << side(p1, p2, points[i]) << endl;
        if (side(p1, p2, points[i]) == leftRight && temp2 > maxDistance)
        {
            maxDistance = temp2;
            temp = i;
        }
    }

    if (temp == -1)
    {
        // cout << "point 1: ";
        // p1.print();
        // cout << "point 2: ";
        // p2.print();
        finalHull.push_back(p1);
        finalHull.push_back(p2);

        bresenham(p1.x() * 800, p1.y() * 800, p2.x() * 800, p2.y() * 800);

        return;
    }

    recurHull(points, points[temp], p1, -side(points[temp], p1, p2), n);
    recurHull(points, points[temp], p2, -side(points[temp], p2, p1), n);
}

void quickHullPrint(vector<Point> points, int n)
{
    int maxX = minMax(points, 1);
    int minX = minMax(points, -1);

    recurHull(points, points[minX], points[maxX], 1, n);
    recurHull(points, points[minX], points[maxX], -1, n);

    //WE PRINT THE HULL HERE

    cout << "The points for Convex Hull are:\n";

    drawRedPoints(finalHull);

    for (int j = 0; j < finalHull.size(); j++)
    {
        for (int k = j + 1; k < finalHull.size(); /*no increment*/)
        {
            if (finalHull[j].x() == finalHull[k].x())
            {
                finalHull.erase(finalHull.begin() + k);
            }
            else
            {
                k++; // increment only if no element was erased !
            }
        }
    }
    printVector(finalHull);
}

int main()
{
    vector<Point> randV = randomVector(50);
    drawPoints(randV);
    quickHullPrint(randV, randV.size());
    printMatrix(pic);

    return 0;
}