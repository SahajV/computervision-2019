#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>

using namespace std;
int w, h;

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
            img << imageM[x][y];
            img << " ";
            img << imageM[x][y];
            img << " ";
            img << imageM[x][y];
            img << " ";
        }
        img << endl;
    }
}

int **sobelOperator(int **imageM)
{
    int gX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gY[3][3] = {{-1, 2, -1}, {0, 0, 0}, {1, 2, 1}};

    int rows = w;
    int columns = h;

    int **mag = new int *[w];
    for (int i = 0; i < w; ++i)
        mag[i] = new int[h];

    for (int x = 1; x < rows - 2; x++)
    {
        for (int y = 1; y < columns - 2; y++)
        {
            double s1 = 0;
            double s2 = 0;

            s1 = ((2 * imageM[x + 2][y + 1] + imageM[x + 2][y] + imageM[x + 2][y + 2]) - (2 * imageM[x][y + 1] + imageM[x][y] + imageM[x][y + 2]));
            s2 = ((2 * imageM[x + 1][y + 2] + imageM[x][y + 2] + imageM[x + 2][y + 2]) - (2 * imageM[x + 1][y] + imageM[x][y] + imageM[x + 2][y]));

            double magn = sqrt(pow(s1, 2) + pow(s2, 2));
            double finalP = 255;
            if (magn < 80)
                finalP = 0;

            mag[x][y] = finalP;
        }
    }

    return mag;
}

int main()
{
    cout << "Input file name with .ppm" << endl;
    string file;
    cin >> file;

    int **imageMG = readFile(file);
    makeImage(imageMG, "imageg.ppm");

    int **imageMS = sobelOperator(imageMG);
    makeImage(imageMS, "imagem.ppm");

    return 0;
}