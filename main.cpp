#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

struct Point {
float x, y, z;
};

using namespace std;

int main (int argc, char *argv[]) {

    if (1 == argc)
    {
        cout << "Usage: " << argv[0] << " <file.stl> [<file2.stl> <file2.stl> ...]" << endl;
        exit (EXIT_FAILURE);
    }

    ifstream fin;
    fin.open(argv[1]);
    if (!fin.is_open())
    {
        cout << "Error: cannot open file " << argv[1] << endl;
        exit (EXIT_FAILURE);
    }

    string line;
    getline(fin, line); //omit first line of file

    Point new_point;
    vector<Point> points;

    string term;
    fin >> term;
    int term_count = 0;

    while (term != "endsolid") {
        getline(fin, line); //omit normal

        getline(fin, line); //omit outer loop

        for (int i = 0; i<3; i++){
            fin >> term >> new_point.x >> new_point.y >> new_point.z; // read vertex coords
            points.push_back(new_point);
            getline(fin, line); //go to the next line
        }
        getline(fin, line); //omit endloop
        getline(fin, line); //omit endfacet
        fin >> term; //get next term to check whether endsolid is reached
    }

    return 0;
}