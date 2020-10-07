#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int width = 800, height = 800; //We define the resolution fo the image that we are making
int pic[width][height] = {0};  //Make a Matrix that consists of all 0's

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

double distance(double x1, double y1, double x2, double y2) //helper method to find the distance between two points
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


double findInRadius(double x1, double y1, double x2, double y2, double x3, double y3) //Method to find the radius of the inCirlce
{
    double a = distance(x1, y1, x2, y2);
    double b = distance(x1, y1, x3, y3);
    double c = distance(x2, y2, x3, y3);

    double s = (a + b + c) / 2;
    double r = sqrt(((s - a) * (s - b) * (s - c)) / s);

    return r;
}

void drawCircle(int radius, int r, int cx, int cy) //Method to draw a circle from radius and center
{
    int x, y, xmax, y2, y2_new, ty;
    xmax = (int)(radius * 0.70710678) + 2; // maximum x at radius/sqrt(2)
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

double findInCenterx(double x1, double y1, double x2, double y2, double x3, double y3) //Method to find the x of the center of the inCircle
{
    double a = distance(x1, y1, x2, y2);
    double b = distance(x1, y1, x3, y3);
    double c = distance(x2, y2, x3, y3);

    double p = a + b + c;
    double ox = ((a * x3) + (b * x2) + (c * x1)) / p;

    return ox;
}

double findInCentery(double x1, double y1, double x2, double y2, double x3, double y3) //Method to find the y of the center of the inCircle
{
    double a = distance(x1, y1, x2, y2);
    double b = distance(x1, y1, x3, y3);
    double c = distance(x2, y2, x3, y3);

    double p = a + b + c;
    double oy = ((a * y3) + (b * y2) + (c * y1)) / p;

    return oy;
}

double intercept(double slope, double x, double y) //Helper method for findCircumCenter that finds the intercept of the equation
{
    return y - slope * x;
}

double findCircumCenterx(double x1, double y1, double x2, double y2, double x3, double y3) //Method to find the x of the circum circle
{

    double midABX = (x1 + x2) / 2;
    double midABY = (y1 + y2) / 2;
    double midACX = (x2 + x3) / 2;
    double midACY = (y2 + y3) / 2;

    double slopeAB = -1 * ((x2 - x1) / (y2 - y1));
    double slopeAC = -1 * ((x3 - x2) / (y3 - y2));

    double interAB = intercept(slopeAB, midABX, midABY);
    double interAC = intercept(slopeAC, midACX, midACY);

    double centerx = (interAC - interAB) / (slopeAB - slopeAC);
    double centery = slopeAB * centerx + interAB;

    return centerx;
}

double findCircumCentery(double x1, double y1, double x2, double y2, double x3, double y3) //Method to find the y of the circum circle
{
    double midABX = (x1 + x2) / 2;
    double midABY = (y1 + y2) / 2;
    double midACX = (x2 + x3) / 2;
    double midACY = (y2 + y3) / 2;

    double slopeAB = -1 * ((x2 - x1) / (y2 - y1));
    double slopeAC = -1 * ((x3 - x2) / (y3 - y2));

    double interAB = intercept(slopeAB, midABX, midABY);
    double interAC = intercept(slopeAC, midACX, midACY);

    double centerx = (interAC - interAB) / (slopeAB - slopeAC);
    double centery = slopeAB * centerx + interAB;

    return centery;
}

double findOrthocenterX(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method to find the x of the orthocenter
{
    double perpSlopeAB = -1 * ((tx2 - tx1) / (ty2 - ty1));
    double perpSlopeBC = -1 * ((tx3 - tx2) / (ty3 - ty2));

    double value1 = (-1*(perpSlopeAB * tx3)) +  (perpSlopeBC * tx1) + ty3 - ty1;
    double value2 = -1*(perpSlopeAB-perpSlopeBC);

    double x = value1/value2;

    return x;

}

double findOrthocenterY(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method to find the y of the orthocenter
{
    double perpSlopeAB = -1 * ((tx2 - tx1) / (ty2 - ty1));
    double perpSlopeBC = -1 * ((tx3 - tx2) / (ty3 - ty2));

    double value1 = (-1*(perpSlopeAB * tx3)) +  (perpSlopeBC * tx1) + ty3 - ty1;
    double value2 = -1*(perpSlopeAB-perpSlopeBC);

    double x = value1/value2;

    double y = (perpSlopeAB*(x-tx3)) + ty3;

    return y;
}

void drawTriangle(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method for drawing a triangle
{
    int x1 = round(tx1 * 800);
    int y1 = round(ty1 * 800);
    int x2 = round(tx2 * 800);
    int y2 = round(ty2 * 800);
    int x3 = round(tx3 * 800);
    int y3 = round(ty3 * 800);

    bresenham(x1, y1, x2, y2);
    bresenham(x2, y2, x3, y3);
    bresenham(x1, y1, x3, y3);
}

void drawInCircle(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method for drawing an incircle
{
    double inRadius = findInRadius(tx1, ty1, tx2, ty2, tx3, ty3);
    double cx = findInCenterx(tx1, ty1, tx2, ty2, tx3, ty3);
    double cy = findInCentery(tx1, ty1, tx2, ty2, tx3, ty3);

    int radiusIn = round(inRadius * 800);
    int cxIn = round(cx * 800);
    int cyIn = round(cy * 800);

    drawCircle(radiusIn, radiusIn, cxIn, cyIn);
}


void drawCircumCircle(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method for drawing a circumcircle
{

    double circumx = findCircumCenterx(tx1, ty1, tx2, ty2, tx3, ty3);
    double circumy = findCircumCentery(tx1, ty1, tx2, ty2, tx3, ty3);
    double circumRadius = distance(circumx, circumy, tx1, ty1);

    int radiusCircum = round(circumRadius * 800);
    int cxC = round(circumx * 800);
    int cyC = round(circumy * 800);

    drawCircle(radiusCircum, radiusCircum, cxC, cyC);
}

void drawEulerLine(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method to draw the eulerline
{

    double centX = (tx1 + tx2 + tx3) / 3;
    double centY = (ty1 + ty2 + ty3) / 3;

    double circumX = findCircumCenterx(tx1, ty1, tx2, ty2, tx3, ty3);
    double circumY = findCircumCentery(tx1, ty1, tx2, ty2, tx3, ty3);

    double slope = (circumY - centY) / (circumX - centX);

    double inter = centY - (slope * centX);

    double eulerD1 = slope * 0 + inter; //These are the y points of the euler line
    double eulerD2 = slope * 0.99875 + inter;

    int eulerI1 = round(eulerD1 * 800);
    int eulerI2 = round(eulerD2 * 800);

    bresenham(0, eulerI1, 799, eulerI2);
}

void drawNinePointCircle(double tx1, double ty1, double tx2, double ty2, double tx3, double ty3) //Method to draw the nine-point circle
{
    double orthoX = findOrthocenterX(tx1, ty1, tx2, ty2, tx3, ty3);
    double orthoY = findOrthocenterY(tx1, ty1, tx2, ty2, tx3, ty3);
    double circumx = findCircumCenterx(tx1, ty1, tx2, ty2, tx3, ty3);
    double circumy = findCircumCentery(tx1, ty1, tx2, ty2, tx3, ty3);

    double nineX = (orthoX + circumx)/2;
    double nineY = (orthoY + circumy)/2;

    double midX = (tx1 + tx2) / 2;
    double midY = (ty1 + ty2) / 2;

    double ninePointRadius = distance(nineX, nineY, midX, midY);

    int radiusNine = round(ninePointRadius * 800);
    int cxO = round(nineX * 800);
    int cyO = round(nineY * 800);

    drawCircle(radiusNine, radiusNine, cxO, cyO);
}

int main()
{
    srand(time(0));

    //DRAW TRIANGLE AND MAKE THE RANDOM POINTS FOR THE TRIANGLE
    double tx1 = rand() / ((double)(RAND_MAX + 1.0));
    double ty1 = rand() / ((double)(RAND_MAX + 1.0));

    double tx2 = rand() / ((double)(RAND_MAX + 1.0));
    double ty2 = rand() / ((double)(RAND_MAX + 1.0));

    double tx3 = rand() / ((double)(RAND_MAX + 1.0));
    double ty3 = rand() / ((double)(RAND_MAX + 1.0));

    //DRAW ALL THE SHAPES
    drawTriangle(tx1, ty1, tx2, ty2, tx3, ty3);
    drawInCircle(tx1, ty1, tx2, ty2, tx3, ty3);
    drawCircumCircle(tx1, ty1, tx2, ty2, tx3, ty3);
    drawEulerLine(tx1, ty1, tx2, ty2, tx3, ty3);
    drawNinePointCircle(tx1, ty1, tx2, ty2, tx3, ty3);

    printMatrix(pic);
    //system("output.ppm");
}