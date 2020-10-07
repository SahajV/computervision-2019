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

using namespace std;

const int width = 800, height = 800; //We define the resolution fo the image that we are making
int pic[width][height] = {0};        //Make a Matrix that consists of all 0's
int nMerge = 0;                      //This stores the time that it takes for the merge sort to run
int nRecur = 0;                      //This stores the number of times the recurisve part 2 code ran
int nRecur2 = 0;                     //This stores the number of times the recursive part 3 code ran
int stripN = 0;                      //This stores the number of times the strip sort runs for part 3 recur

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

vector<Point> readFileV(string file)
{
    ifstream infile(file);
    vector<Point> randV;
    double a, b;

    while (infile >> a >> b)
    {
        randV.push_back(Point(a, b));
    }

    return randV;
}

list<Point> readFileL(string file)
{
    ifstream infile(file);
    list<Point> randL;
    double a, b;

    while (infile >> a >> b)
    {
        randL.push_back(Point(a, b));
    }

    return randL;
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

    ofstream myfile;
    myfile.open("results.txt");
    myfile.precision(17);

    myfile << endl;
    myfile << "-------------------PART 1-------------------" << endl;
    myfile << endl;
    myfile << "The Points and Distance Found Using Brute Force(part 1) are: " << endl;
    myfile << "The Closest Pair of Points are:" << endl;
    myfile << "(";
    myfile << minP1.x();
    myfile << ",";
    myfile << minP1.y();
    myfile << ")";
    myfile << " and ";
    myfile << "(";
    myfile << minP2.x();
    myfile << ",";
    myfile << minP2.y();
    myfile << ")";
    myfile << endl;

    myfile << "The distance between these two points is ";
    myfile << minD;
    myfile << " or ";
    myfile << minD * 800;
    myfile << " when scaled" << endl;

    myfile << "The Brute Force ran " << n << " times." << endl;

    return PointPair(minP1, minP2);
}

void drawPoints(vector<Point> randL, PointPair closestPair)
{
    vector<Point>::iterator it;
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
    list<Point> randL = readFileL("points.txt");

    clock_t start_tB = clock();
    PointPair closestPair = closestPoints(randL);
    clock_t end_tB = clock();

    cout << endl
         << "Time Analysis(not including filling list and making file): " << endl;
    printf("Part 1 run time is: %fs.\n", double(end_tB - start_tB) / double(CLOCKS_PER_SEC));

    ofstream myfile;
    myfile.open("results.txt");
    myfile.precision(17);
    myfile << endl
           << "Time Analysis(not including filling list and making file): " << endl;
    double finalT = double(end_tB - start_tB) / double(CLOCKS_PER_SEC);
    myfile << "Part 1 run time is: " << endl
           << finalT << "s" << endl;

    //drawPoints(randL, closestPair);
}

vector<Point> mergeX(vector<Point> left, vector<Point> right)
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

vector<Point> mergeSortX(vector<Point> &arr)
{
    if (arr.size() <= 1)
        return arr;
    int len = arr.size() / 2;
    vector<Point> left(arr.begin(), arr.begin() + len);
    vector<Point> right(arr.begin() + len, arr.end());
    return mergeX(mergeSortX(left), mergeSortX(right));
}

vector<Point> mergeY(vector<Point> left, vector<Point> right)
{
    size_t ileft = 0, iright = 0;
    vector<Point> results;
    while (ileft < left.size() && iright < right.size())
    {
        if (left[ileft].y() < right[iright].y())
            results.push_back(left[ileft++]);
        else
            results.push_back(right[iright++]);
    }
    while (ileft < left.size())
    {
        results.push_back(left[ileft++]);
    }
    while (iright < right.size())
    {
        results.push_back(right[iright++]);
    }
    return results;
}

vector<Point> mergeSortY(vector<Point> &arr)
{
    if (arr.size() <= 1)
        return arr;
    int len = arr.size() / 2;
    vector<Point> left(arr.begin(), arr.begin() + len);
    vector<Point> right(arr.begin() + len, arr.end());
    return mergeX(mergeSortY(left), mergeSortY(right));
}

PointPair minDistance(vector<Point> sortedV) //only runs when n <= 3
{
    double minD = INFINITY;

    vector<Point>::iterator it1;
    vector<Point>::iterator it2;
    Point minP1;
    Point minP2;

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
                    minP1 = temp1;
                    minP2 = temp2;
                }
            }
        }
    }

    return PointPair(minP1, minP2);
}

PointPair closestRecursiveR1(vector<Point> sortedVector, int n)
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

        PointPair p1 = closestRecursiveR1(left, mid);
        PointPair p2 = closestRecursiveR1(right, n - mid);

        double d1 = distance(p1.a(), p1.b());
        double d2 = distance(p2.a(), p2.b());

        double d = min(d1, d2);

        PointPair minPP = p1;
        if (d1 < d2)
        {
            minPP = p1;
        }
        else if (d2 < d1)
        {
            minPP = p2;
        }

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
                    minPP = PointPair(sortedVector[x], sortedVector[y]);
                }
            }
        }

        nRecur++;
        return minPP;
    }
}

void part2()
{
    vector<Point> randV1 = readFileV("points.txt");

    clock_t start_tR = clock(); //CLOCK BEGINS
    vector<Point> vSorted = mergeSortX(randV1);
    PointPair minRecurPP = closestRecursiveR1(vSorted, vSorted.size());
    double minRecurD = distance(minRecurPP.a(), minRecurPP.b());
    clock_t end_tR = clock(); //CLOCK ENDS

    cout << endl
         << "-------------------PART 2-------------------" << endl
         << endl;
    cout << "The Closest Pair of Points are:" << endl;
    minRecurPP.a().print();
    cout << " and ";
    minRecurPP.b().print();
    cout << endl;
    cout << "The Distance found Recursively(Part 2) is:" << endl;
    cout << "Exact Minimum Distance: " << minRecurD << endl;
    cout << "Scaled for the ppm(800x800): " << minRecurD * 800 << endl;
    cout << "Merge Sort Complexity is o(nlogn) and it ran: " << nMerge << " times." << endl;
    cout << "Recursive method recurred " << nRecur << " times." << endl;
    cout << endl
         << "Time Analysis: " << endl;
    printf("Part 2 run time is: %fs.\n", double(end_tR - start_tR) / double(CLOCKS_PER_SEC));

    ofstream myfile;
    myfile.open("results.txt");
    myfile.precision(17);

    myfile << endl
           << "-------------------PART 2-------------------" << endl
           << endl;
    myfile << "The Closest Pair of Points are:" << endl;
    myfile << "(";
    myfile << minRecurPP.a().x();
    myfile << ",";
    myfile << minRecurPP.a().y();
    myfile << ")";
    myfile << " and ";
    myfile << "(";
    myfile << minRecurPP.b().x();
    myfile << ",";
    myfile << minRecurPP.b().y();
    myfile << ")";
    myfile << endl;
    myfile << "The Distance found Recursively(Part 2) is:" << endl;
    myfile << "Exact Minimum Distance: " << minRecurD << endl;
    myfile << "Scaled for the ppm(800x800): " << minRecurD * 800 << endl;
    myfile << "Merge Sort Complexity is o(nlogn) and it ran: " << nMerge << " times." << endl;
    myfile << "Recursive method recurred " << nRecur << " times." << endl;
    myfile << endl
           << "Time Analysis: " << endl;
    double finalT = double(end_tR - start_tR) / double(CLOCKS_PER_SEC);
    myfile << "Part 2 run time is: " << finalT << "s" << endl;
}

bool sorty(Point p1, Point p2)
{
    return(p1.y() < p2.y());
}

PointPair closestRecursiveR2(vector<Point> sortedVector, int n)
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

        PointPair p1 = closestRecursiveR2(left, mid);
        PointPair p2 = closestRecursiveR2(right, n - mid);

        double d1 = distance(p1.a(), p1.b());
        double d2 = distance(p2.a(), p2.b());

        double d = min(d1, d2);

        PointPair minPP = p1;
        if (d1 < d2)
        {
            minPP = p1;
        }
        else if (d2 < d1)
        {
            minPP = p2;
        }

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
        vector<Point> strip;
        for (int i = 0; i < n; i++)
        {
            if (abs(sortedVector[i].x() - midLine) < d)
                strip.push_back(sortedVector[i]);
        }

        sort(strip.begin(), strip.end(), sorty);
        int size = strip.size();
        double tempMin = d;
        PointPair tempMPair = minPP;

        stripN = 0;
        for (int i = 0; i < size; ++i) //This for loop will only run a max of 6 times(proven in analysis)
        {
            // for (int j = i + 1; j < size && (strip[j].y() - strip[i].y()) < tempMin; ++j)
            for (int j = i + 1; j < strip.size() && j < i + 7; j++)
            {
                stripN++;
                if (distance(strip[i], strip[j]) < tempMin)
                {
                    tempMin = distance(strip[i], strip[j]);
                    tempMPair = PointPair(strip[i], strip[j]);
                }
            }
        }

        PointPair finalMPair = minPP;
        if (tempMin < d)
        {
            finalMPair = tempMPair;
        }

        nRecur2++;
        return finalMPair;
    }
}

void part3()
{
    vector<Point> randV2 = readFileV("points.txt");

    clock_t start_tR = clock(); //CLOCK BEGINS
    vector<Point> vSorted = mergeSortX(randV2);
    PointPair minRecurPP = closestRecursiveR2(vSorted, vSorted.size());
    double minRecurD = distance(minRecurPP.a(), minRecurPP.b());
    clock_t end_tR = clock(); //CLOCK ENDS

    cout << endl
         << "-------------------PART 3-------------------" << endl
         << endl;
    cout << "The Closest Pair of Points are:" << endl;
    minRecurPP.a().print();
    cout << " and ";
    minRecurPP.b().print();
    cout << endl;
    cout << "The Distance found Recursively(Part 3) is:" << endl;
    cout << "Exact Minimum Distance: " << minRecurD << endl;
    cout << "Scaled for the ppm(800x800): " << minRecurD * 800 << endl;
    cout << "Recursive method recurred " << nRecur2 << " times." << endl;
    cout << "Strip Search(Y sorted) ran " << stripN << " times." << endl;
    cout << endl
         << "Time Analysis: " << endl;
    printf("Part 3 run time is: %fs.\n", double(end_tR - start_tR) / double(CLOCKS_PER_SEC));

    ofstream myfile;
    myfile.open("results.txt", ios_base::app);
    myfile.precision(17);

    myfile << endl
           << "-------------------PART 3-------------------" << endl
           << endl;
    myfile << "The Closest Pair of Points are:" << endl;
    myfile << "(";
    myfile << minRecurPP.a().x();
    myfile << ",";
    myfile << minRecurPP.a().y();
    myfile << ")";
    myfile << " and ";
    myfile << "(";
    myfile << minRecurPP.b().x();
    myfile << ",";
    myfile << minRecurPP.b().y();
    myfile << ")";
    myfile << endl;
    myfile << "The Distance found Recursively(Part 3) is:" << endl;
    myfile << "Exact Minimum Distance: " << minRecurD << endl;
    myfile << "Scaled for the ppm(800x800): " << minRecurD * 800 << endl;
    myfile << "Recursive method recurred " << nRecur << " times." << endl;
    myfile << endl
           << "Time Analysis: " << endl;
    double finalT = double(end_tR - start_tR) / double(CLOCKS_PER_SEC);
    myfile << "Part 3 run time is: " << finalT << "s" << endl;
}

int main()
{
    fillFile(100000);
    //part1();
    part2();
    part3();
}