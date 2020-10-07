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

vector<Point> mergeSortX(vector<Point> &arr)
{
    if (arr.size() <= 1)
        return arr;
    int len = arr.size() / 2;
    vector<Point> left(arr.begin(), arr.begin() + len);
    vector<Point> right(arr.begin() + len, arr.end());
    return mergeX(mergeSortX(left), mergeSortX(right));
}

bool sorty(Point p1, Point p2)
{
    return (p1.y() < p2.y());
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

        for (int i = 0; i < size; ++i) //This for loop will only run a max of 6 times(proven in analysis)
        {
            for (int j = i + 1; j < strip.size() && j < i + 7; j++)
            {
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

        return finalMPair;
    }
}

void recursiveM(vector<Point> randV1)
{
    clock_t start_tR = clock(); //CLOCK BEGINS
    vector<Point> vSorted = mergeSortX(randV1);
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
    cout << endl
         << "Time Analysis: " << endl;
    printf("Part 3 run time is: %fs.\n", double(end_tR - start_tR) / double(CLOCKS_PER_SEC));

    ofstream myfile;
    myfile.open("randomized.txt");
    // myfile.open("results.txt", ios_base::app);
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
    myfile << endl
           << "Time Analysis: " << endl;
    double finalT = double(end_tR - start_tR) / double(CLOCKS_PER_SEC);
    myfile << "Part 3 run time is: " << finalT << "s" << endl;
}

struct hash_func //GEEKSFORGEEKS
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

unordered_map<pair<int, int>, Point, struct hash_func> dict;

void makeDict(vector<Point> points, int n, double square_size)
{
    for (int i = 0; i <= n; i++)
    {
        unsigned long x = (unsigned long)(points[i].x() / square_size);
        unsigned long y = (unsigned long)(points[i].y() / square_size);
        dict[make_pair(x, y)] = points[i];
    }
}

PointPair closestRandomized(vector<Point> randomVector, int n)
{
    Point cP1 = randomVector[0];
    Point cP2 = randomVector[1];
    double minD = distance(randomVector[0], randomVector[1]);
    double sSize = minD / 2.0;

    dict.clear();
    makeDict(randomVector, 1, sSize);

    for (int i = 2; i < randomVector.size(); i++)
    {
        unsigned long sX = (unsigned long)(randomVector[i].x() / sSize);
        unsigned long sY = (unsigned long)(randomVector[i].y() / sSize);
        int bound = 2;
        Point minimumPoint;
        bool updated = false;
        double tempM = minD;
        for (int x = -bound; x <= bound; x++)
        {
            for (int y = -bound; y <= bound; y++)
            {
                Point tempP2 = dict[make_pair(sX + x, sY + y)];
                if (tempP2.x() != -1 && tempP2.y() != -1)
                {
                    double tempDist1 = distance(tempP2, randomVector[i]);
                    if (tempDist1 < tempM)
                    {
                        minimumPoint = tempP2;
                        updated = true;
                        tempM = tempDist1;
                    }
                }
            }
        }
        if (updated)
        {
            cP1 = minimumPoint;
            cP2 = randomVector[i];
            minD = distance(minimumPoint, randomVector[i]);
            sSize = minD / 2.0;

            dict.clear();
            makeDict(randomVector, i, sSize);
        }
        else
            dict[make_pair(sX, sY)] = randomVector[i];
    }
    return PointPair(cP1, cP2);
}

void randomizedM(vector<Point> randV2)
{
    cout << "done reading the file" << endl;

    clock_t start_tR = clock(); //CLOCK BEGINS
    random_shuffle(randV2.begin(), randV2.end());
    PointPair closestPair = closestRandomized(randV2, randV2.size());
    clock_t end_tR = clock(); //CLOCK ENDS
    double minRandomD = distance(closestPair.a(), closestPair.b());

    cout << endl
         << "-------------------PART 4-------------------" << endl
         << endl;
    cout << "The Closest Pair of Points are:" << endl;
    closestPair.a().print();
    cout << " and ";
    closestPair.b().print();
    cout << endl;
    cout << "The Distance found with Random Approach(Part 4) is:" << endl;
    cout << "Exact Minimum Distance: " << minRandomD << endl;
    cout << "Scaled for the ppm(800x800): " << minRandomD * 800 << endl;
    cout << endl
         << "Time Analysis: " << endl;
    printf("Part 4 run time is: %fs.\n", double(end_tR - start_tR) / double(CLOCKS_PER_SEC));

    ofstream myfile;
    myfile.open("randomized.txt", ios_base::app);
    myfile.precision(17);

    myfile << endl
           << "-------------------PART 4-------------------" << endl
           << endl;
    myfile << "The Closest Pair of Points are:" << endl;
    myfile << "(";
    myfile << closestPair.a().x();
    myfile << ",";
    myfile << closestPair.a().y();
    myfile << ")";
    myfile << " and ";
    myfile << "(";
    myfile << closestPair.b().x();
    myfile << ",";
    myfile << closestPair.b().y();
    myfile << ")";
    myfile << endl;
    myfile << "The Distance found Recursively(Part 4) is:" << endl;
    myfile << "Exact Minimum Distance: " << minRandomD << endl;
    myfile << "Scaled for the ppm(800x800): " << minRandomD * 800 << endl;
    myfile << endl
           << "Time Analysis: " << endl;
    double finalT = double(end_tR - start_tR) / double(CLOCKS_PER_SEC);
    myfile << "Part 4 run time is: " << finalT << "s" << endl;
}

int main()
{
    fillFile(10000);
    string file = "points1m.txt";
    vector<Point> randV1 = readFileV(file);
    vector<Point> randV2 = readFileV(file);
    recursiveM(randV1);
    randomizedM(randV1);
}