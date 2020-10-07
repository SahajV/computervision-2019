#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <list>
#include <iterator>
#include <vector>

using namespace std;

const int width = 800, height = 800; //We define the resolution fo the image that we are making
int pic[width][height] = {0};        //Make a Matrix that consists of all 0's
int nMerge = 0;                      //This stores the time that it takes for the merge sort to run
int nRecur = 0;                      //This stores the number of times the recurisve part 2 code ran

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
    cout << endl << randL.size() << " points are being tested in this code." << endl;
    cout << "Change fillList() if you want to change the number of elements" << endl;
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

    cout << endl;
    cout << "-------------------PART 1-------------------" << endl;
    cout << endl;
    cout << "The Points and Distance Found Using Brute Force(part 1) are: " << endl;
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

    cout << "The Brute Force ran " << n << " times." << endl;

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

list<Point> part1()
{
    list<Point> rand50List;
    rand50List = fillList(rand50List);

    makePointFile(rand50List);

    clock_t start_tB = clock();
    PointPair closestPair = closestPoints(rand50List);
    clock_t end_tB = clock();

    cout << endl << "Time Analysis(not including filling list and making file): " << endl;
    printf("Part 1 run time is: %fs.\n", double(end_tB - start_tB) / double(CLOCKS_PER_SEC));

    drawPoints(rand50List, closestPair);

    return rand50List;
}

vector<Point> listToVector(list<Point> randL)
{

    // vector<int> vPointsRand;
    // list<Point>::iterator it;

    // for (it = randL.begin(); it != randL.end(); it++)
    // {
    //     vPointsRand.push_back(it);
    // }

    // std::list<char> list = { 'a', 'b', 'c' };

    vector<Point> result;
    for (Point const &c : randL)
    {
        result.push_back(c);
    }

    return result;
}

vector<Point> merge(vector<Point> left, vector<Point> right)
{
    size_t ileft = 0, iright = 0;
    vector<Point> results;
    while (ileft < left.size() && iright < right.size())
    {
        if (left[ileft].x() < right[iright].x())
            results.push_back(left[ileft++]);
        else
            results.push_back(right[iright++]);
        nMerge++;
    }
    while (ileft < left.size())
    {
        results.push_back(left[ileft++]);
        nMerge++;
    }
    while (iright < right.size())
    {
        results.push_back(right[iright++]);
        nMerge++;
    }
    return results;
}

vector<Point> mergeSort(vector<Point> &arr)
{
    if (arr.size() <= 1)
        return arr;
    int len = arr.size() / 2;
    vector<Point> left(arr.begin(), arr.begin() + len);
    vector<Point> right(arr.begin() + len, arr.end());
    return merge(mergeSort(left), mergeSort(right));
}

double minDistance(vector<Point> sortedV) //only runs when n <= 3
{
    double minD = INFINITY;

    vector<Point>::iterator it1;
    vector<Point>::iterator it2;
    for (it1 = sortedV.begin(); it1 != sortedV.end(); it1++)
    {
        for (it2 = it1; it2 != sortedV.end(); it2++)
        {

            Point temp1 = *it1;
            Point temp2 = *it2;

            if (!(temp1.equal(temp2)))
            {
                double dist = distance(temp1, temp2);
                if (dist < minD)
                {
                    minD = dist;
                }
            }
        }
    }

    return minD;
}

double closestRecursive(vector<Point> sortedVector, int n)
{
    if (n <= 3)
    {
        return minDistance(sortedVector);
    }
    else
    {
        int mid = n / 2;
        vector<Point> left(sortedVector.begin(), sortedVector.begin() + sortedVector.size() / 2);
        vector<Point> right(sortedVector.begin() + sortedVector.size() / 2, sortedVector.end());

        // cout <<  "this is the left half of the vector" << endl;
        // for (auto it = left.begin(); it != left.end(); it++)
        // {
        //     double x = it->x();
        //     double y = it->y();
        //     cout << x << " " << y << endl;
        // }

        double d1 = closestRecursive(left, mid);
        double d2 = closestRecursive(right, n - mid);

        double d = min(d1, d2);

        // vector<Point> strip;
        // Point midP = sortedVector[n / 2];

        // for (int i = 0; i < n; i++)
        // {
        //     if (abs(sortedVector[i].x() - midP.x()) < d)
        //     {
        //         strip.push_back(sortedVector[i]);
        //     }
        // }

        double midLine = sortedVector[mid].x();
        double leftS = midLine - d;
        double rightS = midLine + d;

        for (int x = mid - 1; x >= 0 && leftS <= sortedVector[x].x(); x--) //followed algorithm given for this part(worst case is o(n^2))
        {
            for (int y = mid; y <= n - 1 && rightS >= sortedVector[y].x(); y++)
            {
                double tempD = distance(sortedVector[x], sortedVector[y]);
                if (tempD < d)
                {
                    d = tempD;
                }
            }
        }

        nRecur++;
        return d;
    }
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

void part2(list<Point> randL)
{
    vector<Point> Vrand = listToVector(randL);
    //printVector(Vrand);

    clock_t start_tR = clock();
    vector<Point> vSorted = mergeSort(Vrand);
    //printVector(vSorted);

    double minRecurD = closestRecursive(vSorted, vSorted.size());
    clock_t end_tR = clock();

    cout << endl;
    cout << "-------------------PART 2-------------------" << endl;
    cout << endl;
    cout << "The Distance found Recursively(Part 2) is:" << endl;
    cout << "Exact Minimum Distance: " << minRecurD << endl;
    cout << "Scaled for the ppm(800x800): " << minRecurD * 800 << endl;
    cout << "Merge Sort Complexity is o(nlogn) and it ran : : " << nMerge << " times." << endl;
    cout << "Recursive method recurred " << nRecur << " times." << endl;
    cout << endl << "Time Analysis: " << endl;
    printf("Part 2 run time is: %fs.\n", double(end_tR - start_tR) / double(CLOCKS_PER_SEC));
}

int main()
{
    list<Point> randL = part1();

    part2(randL);

    printMatrix(pic);
    //system("output.ppm");

    return (0);
}
