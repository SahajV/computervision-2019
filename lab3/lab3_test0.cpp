#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <list>
#include <iterator>

using namespace std;

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

        if(red)
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

list<Point> fillList(list<Point> randL)
{
    srand(time(0));

    for (int x = 0; x < 50; x++)
    {
        double xR = rand() / ((double)(RAND_MAX + 1.0));
        double yR = rand() / ((double)(RAND_MAX + 1.0));
        randL.push_front(Point(xR, yR));
    }
    return randL;
}

void makePointFile(list<Point> randL)
{
    ofstream myfile;
    myfile.open("points.txt");
    myfile.precision(17);

    list<Point>::iterator it;
    for (it = randL.begin(); it != randL.end(); it++)
    {
        double x = it->x();
        double y = it->y();
        myfile << x << " " << y << endl;
    }
}

double distance(Point p1, Point p2)
{
    double dist = sqrt(pow(p2.x() - p1.x(), 2) + pow(p2.y() - p1.y(), 2));
    return dist;
}

PointPair closestPoints(list<Point> randL)
{

    double minD = INFINITY;
    Point minP1;
    Point minP2;

    int n = 0;

    list<Point>::iterator it1;
    list<Point>::iterator it2;
    for (it1 = randL.begin(); it1 != randL.end(); it1++)
    {
        for (it2 = it1; it2 != randL.end(); it2++)
        {

            Point temp1 = *it1;
            Point temp2 = *it2;

            if (!(temp1.equal(temp2)))
            {
                n++;
                double dist = distance(temp1, temp2);
                if (dist < minD)
                {
                    minD = dist;
                    minP1 = temp1;
                    minP2 = temp2;
                }
            }
        }
    }

    cout << "The Closest Pair of Points are:" << endl;
    minP1.print();
    cout << " and ";
    minP2.print();
    cout << endl;

    cout << "The distance between these two points is ";
    cout << minD;
    cout << " or ";
    cout << minD * 800;
    cout << " when scaled" << endl;

    cout << "The Code Ran " << n << " times." << endl;

    return PointPair(minP1, minP2);
}

void drawPoints(list<Point> randL, PointPair closestPair)
{
    list<Point>::iterator it;
    for (it = randL.begin(); it != randL.end(); it++)
    {
        double x = it->x();
        double y = it->y();

        drawCircle(2, 2, x * 800, y * 800, false);
    }

    drawCircle(2, 2, closestPair.a().x() * 800, closestPair.a().y() * 800, true);
    drawCircle(2, 2, closestPair.b().x() * 800, closestPair.b().y() * 800, true);
}

void part1()
{
    list<Point> rand50List;
    rand50List = fillList(rand50List);

    makePointFile(rand50List);

    PointPair closestPair = closestPoints(rand50List);

    drawPoints(rand50List, closestPair);
}

int main()
{
    part1();

    printMatrix(pic);
    system("display output.ppm &");

    return (0);
}
