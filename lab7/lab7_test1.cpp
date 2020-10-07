//SAHAJ VEDEREY

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;
int w, h;

int **downScale(int **imageM, int scale)
{
    int nW = w / scale;
    int nH = h / scale;

    int **newM = new int *[nW];
    for (int i = 0; i < nW; ++i)
    {
        newM[i] = new int[nH];
    }

    for (int i = 0; i < nW * scale; i += scale)
        for (int j = 0; j < nH * scale; j += scale)
        {

            double newP = 0;
            for (int di = 0; di < scale; di++)
                for (int dj = 0; dj < scale; dj++)
                    newP += imageM[i + di][j + dj];

            newP = newP / (double)(scale * scale);
            int finalNP = newP;

            newM[i / scale][j / scale] = finalNP;
        }
    w = nW;
    h = nH;

    return newM;
}

int **readFile(string file)
{
    ifstream infile;
    infile.open(file);
    infile.ignore(10, 'P');
    infile.ignore(1, '3');
    int width, height, intensity;
    infile >> width >> height >> intensity;
    w = width;
    h = height;
    // int imageM[width][height];

    int **imageM;
    imageM = (int **)malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++)
        imageM[i] = (int *)malloc(height * sizeof(int));

    int r, g, b;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            infile >> r >> g >> b;
            imageM[x][y] = (r + g + b) / 3;
        }
    }

    imageM = downScale(imageM, 4);
    return imageM;
}

void makeImage(int **imageM, string fileN)
{
    ofstream img(fileN);
    img << "P3" << endl;
    img << w << " " << h << endl;
    img << "255" << endl;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (imageM[x][y] == 777)
            {
                img << 255;
                img << " ";
                img << 0;
                img << " ";
                img << 0;
                img << " ";
            }
            else if (imageM[x][y] == 666)
            {
                img << 0;
                img << " ";
                img << 0;
                img << " ";
                img << 255;
                img << " ";
            }
            else
            {
                img << imageM[x][y];
                img << " ";
                img << imageM[x][y];
                img << " ";
                img << imageM[x][y];
                img << " ";
            }
        }
        img << endl;
    }
}

int **basicBlur(int **imageM)
{
    int rows = w;
    int columns = h;

    int kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int **image = new int *[w];

    for (int i = 0; i < w; ++i)
    {
        image[i] = new int[h];
    }

    for (int x = 1; x < rows - 2; x++)
    {
        for (int y = 1; y < columns - 2; y++)
        {
            image[x][y] = ((imageM[x - 1][y - 1] * kernel[0][0]) +
                           (imageM[x - 1][y] * kernel[0][1]) +
                           (imageM[x - 1][y + 1] * kernel[0][2]) +
                           (imageM[x][y - 1] * kernel[1][0]) +
                           (imageM[x][y] * kernel[1][1]) +
                           (imageM[x][y + 1] * kernel[1][2]) +
                           (imageM[x + 1][y - 1] * kernel[2][0]) +
                           (imageM[x + 1][y] * kernel[2][1]) +
                           (imageM[x + 1][y + 1] * kernel[2][2])) /
                          16;
        }
    }

    return image;
}

struct Foo
{
    int **value1;
    double **value2;
};

Foo sobelAndNonmaxSup(int **imageM)
{
    int gX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gY[3][3] = {{-1, 2, -1}, {0, 0, 0}, {1, 2, 1}};

    int rows = w;
    int columns = h;

    int **mag = new int *[w];
    double **angles = new double *[w];
    int **sup = new int *[w];
    for (int i = 0; i < w; ++i)
    {
        mag[i] = new int[h];
        angles[i] = new double[h];
        sup[i] = new int[h];
    }

    for (int x = 1; x < rows - 2; x++)
    {
        for (int y = 1; y < columns - 2; y++)
        {
            double s1 = 0;
            double s2 = 0;

            s1 = ((2 * imageM[x + 2][y + 1] + imageM[x + 2][y] + imageM[x + 2][y + 2]) - (2 * imageM[x][y + 1] + imageM[x][y] + imageM[x][y + 2]));
            s2 = ((2 * imageM[x + 1][y + 2] + imageM[x][y + 2] + imageM[x + 2][y + 2]) - (2 * imageM[x + 1][y] + imageM[x][y] + imageM[x + 2][y]));

            int angle = atan2(s2, s1) * 180 / 3.141592;
            double magn = sqrt(pow(s1, 2) + pow(s2, 2));
            // double finalP = 255;
            // if (magn < 80)
            //     finalP = 0;

            mag[x][y] = magn;
            angles[x][y] = angle;
        }
    }

    double theta;
    int pixel;
    sup = mag;

    for (int x = 1; x < rows - 1; x++)
    {
        for (int y = 1; y < columns - 1; y++)
        {
            pixel = mag[x][y];
            theta = angles[x][y];

            int q = 255;
            int r = 255;

            if (theta < 180)
                theta += 180;

            if (theta < 22.5 || theta > 157.5)
            {
                if ((mag[x - 1][y] > pixel) || (mag[x + 1][y] > pixel))
                    sup[x][y] = 0;
            }
            else if (theta >= 22.5 && theta <= 67.5)
            {
                if ((mag[x + 1][y + 1] > pixel) || (mag[x - 1][y - 1]) > pixel)
                    sup[x][y] = 0;
            }
            else if (theta > 67.5 && theta <= 112.5)
            {
                if ((mag[x][y - 1] > pixel) || (mag[x][y + 1] > pixel))
                    sup[x][y] = 0;
            }
            else
            {
                if ((mag[x + 1][y - 1] > pixel) || (mag[x - 1][y + 1] > pixel))
                    sup[x][y] = 0;
            }
        }
    }

    Foo result = {sup, angles};
    return result;
}

int **dblThreshAndHyster(int **imageM, int lower, int upper)
{
    int l = lower;
    int u = upper;

    int **fin = new int *[w];
    for (int i = 0; i < w; ++i)
    {
        fin[i] = new int[h];
    }

    for (int x = 1; x < w - 1; x++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            if (imageM[x][y] >= u)
            {
                fin[x][y] = 255;
            }
            else if (imageM[x][y] > l)
            {
                fin[x][y] = 0;
            }
            else if (imageM[x][y] <= u && imageM[x][y] >= l)
            {

                if (imageM[x + 1][y - 1] >= u || imageM[x + 1][y] >= u || imageM[x + 1][y + 1] >= u || imageM[x][y - 1] >= u || imageM[x][y + 1] >= u ||
                    imageM[x - 1][y - 1] >= u || imageM[x - 1][y] >= u || imageM[x - 1][y + 1] >= u)
                {
                    fin[x][y] = 255;
                }
                else
                {
                    fin[x][y] = 0;
                }
            }
        }
    }

    return fin;
}

//START CIRCLE DETECTION HERE

int **drawCircle(int **imageM, int a, int b, double r)
{
    for (int theta = 0; theta <= 360; theta += 1)
    {
        int x = a + (r * cos(theta * 3.14159 / 180));
        int y = b + (r * sin(theta * 3.14159 / 180));

        if (x > 0 && x < w && y > 0 && y < h)
            imageM[x][y] = 777;
    }

    imageM[a][b] = 666;
    return imageM;
}

struct Circle
{
    int x, y;
    double r;
    int v;
};

vector<Circle> hough(int **imageM, double **angles, int R)
{

    cout << "Dimensions: " << w << " " << h << endl;

    int **accum = new int *[w];
    for (int i = 0; i < w; ++i)
    {
        accum[i] = new int[h];
    }

    int max = 0;
    vector<Circle> circles;

    for (double r = 15; r < 70; r += 0.17)
    {
        for (int x = 1; x < w - 1; x++)
        {
            for (int y = 1; y < h - 1; y++)
            {
                if (imageM[x][y] == 255)
                {
                    int xC = x - (r * cos(angles[x][y] * 3.14159 / 180));
                    int yC = y - (r * sin(angles[x][y] * 3.14159 / 180));
                    if (xC > 0 && xC < w && yC > 0 && yC < h)
                    {
                        if (accum[xC][yC] > max)
                        {
                            max = accum[xC][yC];
                        }
                        accum[xC][yC] += 1;
                    }
                }
            }
        }

        // cout << "max: " << max << " rad: " << r << endl;

        for (int x = 5; x < w; x++)
        {
            for (int y = 5; y < h; y++)
            {
                struct Circle c1 = {x, y, r, accum[x][y]};
                if (accum[x][y] > 13)
                {
                    // imageM[x][y] = 666;
                    // imageM = drawCircle(imageM, x, y, r);
                    bool inRange = false;
                    for (int i = 0; i < circles.size(); i++)
                    {
                        if (x < circles[i].x + 10 && x > circles[i].x - 10 &&
                            y < circles[i].y + 10 && y > circles[i].y - 10)
                        {
                            inRange = true;
                            if (accum[x][y] > circles[i].v)
                                circles[i] = c1;
                            // cout << x << " was in range of " << circles[i].x << endl;
                        }
                        else if (x == circles[i].x || y == circles[i].y)
                            inRange = true;
                    }
                    if (!inRange)
                        circles.push_back(c1);
                }
                // if (r < 50 && r > 40 && accum[x][y] > 2)
                // {
                //     cout << "votes: " << accum[x][y] << " xy " << x << " " << y << endl;
                // }
                accum[x][y] = 0;
            }
        }
    }

    vector<double> coins;
    int coinC[3] = {0};

    for (int i = 0; i < circles.size(); i++)
    {
        coins.push_back(circles[i].r);
    }
    sort(coins.begin(), coins.end());
    for (int i = 0; i < coins.size(); i++)
    {
        // cout << coins[i] << endl;

        if (coins[i] < 23.84)
            coinC[0]++;
        else if (coins[i] < 27.75)
            coinC[1]++;
        else
            coinC[2]++;
    }

    cout << "Total Coins : " << circles.size() << endl;
    cout << "Total Pennies: " << coinC[0] << endl;
    cout << "Total Nickels: " << coinC[1] << endl;
    cout << "Total Quarters: " << coinC[2] << endl;
    double totalM = coinC[0] * 0.01 + coinC[1] * 0.05 + coinC[2] * 0.25;
    cout << "Total Money: $" << totalM << endl;

    ofstream myfile;
    myfile.open ("results.txt");
    myfile << "Total Coins : " << circles.size() << endl;
    myfile << "Total Pennies: " << coinC[0] << endl;
    myfile << "Total Nickels: " << coinC[1] << endl;
    myfile << "Total Quarters: " << coinC[2] << endl;
    myfile << "Total Money: $" << totalM << endl;

    return circles;
}

int main()
{
    cout << "Downscaling will occur by a scale of 4 on line 73" << endl;
    int **imageMG = readFile("image.ppm");

    int **imageMB = basicBlur(imageMG);
    Foo result = sobelAndNonmaxSup(imageMB);
    int **imageMS = result.value1;
    double **anglesR = result.value2;

    int **imageMF = dblThreshAndHyster(imageMS, 70, 140);
    makeImage(imageMF, "imagef.ppm");

    cout << "Canny edge completed" << endl;

    vector<Circle> houghRe = hough(imageMF, anglesR, 188);
    for (int i = 0; i < houghRe.size(); i++)
        imageMG = drawCircle(imageMG, houghRe[i].x, houghRe[i].y, houghRe[i].r);
    makeImage(imageMG, "imagec.ppm");

    return 0;
}