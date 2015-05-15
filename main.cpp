#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>



struct Point {
  double x, y, z;

  bool operator<(const Point& other) const {
    if (x != other.x) 
		return (x < other.x);

    if (y != other.y) 
		return (y < other.y);

    return (z < other.z);
  }
};

typedef std::pair<Point, int> point_and_index;

bool compare(const point_and_index& x, const point_and_index& y) {
	return x.second < y.second;
};

struct Triangle {
  int N[3];
};


using namespace std;

int main (int argc, char *argv[]) {
  
	if (1 == argc) {
    cout << "Converts file from ascii STL to OFF format. Output file has "
            "the same name but its extension is changed to OFF." << endl;
    cout << "Usage: " << argv[0] << " <file.stl>" << endl;
    exit(EXIT_FAILURE);
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
	    
	map<Point, int> points;     // Store point indices in a map to eliminate duplicate points
	vector<Triangle> triangles; 

	Point new_point;
	Triangle new_triangle;

	int max_pt_index = 0;
	int curr_pt_index = 0;

    string term;
    fin >> term;

    // Reading loop
    while (term != "endsolid") {
      getline(fin, line);  // omit normal
      getline(fin, line);  // omit outer loop

      for (int i = 0; i < 3; i++) {
        fin >> term >> new_point.x >> new_point.y >>
            new_point.z;     // read vertex coords
        getline(fin, line);  // go to the next line

		std::pair<const Point, const int> pt = { new_point, max_pt_index };
        auto res = points.insert(pt);  // try to insert new point into map

        if (res.second) {  // if it was inserted, increment point index
			curr_pt_index = max_pt_index;
          max_pt_index++;
          
        } else {
          curr_pt_index = res.first->second;  // if it wasn't, get index of the
          // point that had been added before
        }

        new_triangle.N[i] = curr_pt_index;
      }

      // All indices found, now add the triangle
      triangles.push_back(new_triangle);

      getline(fin, line);  // omit endloop
      getline(fin, line);  // omit endfacet
      fin >> term;  // get next term to check whether endsolid is reached
    }

	// Sort points by index because OFF doesn't allow to specify point index explicitly


	vector<point_and_index> points_sorted(points.begin(), points.end());

    sort(points_sorted.begin(), points_sorted.end(), compare);

        // Write OFF
	ofstream fout;
	string out_filename(argv[1]);
	auto ext_pos = out_filename.find_last_of(".");
	out_filename.replace(ext_pos+1, 3, "off");

	fout.open(out_filename);
	if (!fout.is_open())
	{
		cout << "Error: cannot open output file " << out_filename << endl;
		exit(EXIT_FAILURE);
	}

	const int num_points = points.size();
	const int num_triangles = triangles.size();
	const int num_tetrahedra = 0;

	// Header
	fout << "OFF" << endl;
	fout << num_points << ' ' << num_triangles << ' ' << num_tetrahedra << endl;

	// Points
	for (const auto p : points_sorted){
		const auto point = p.first;
		fout << point.x << ' ' << point.y << ' ' << point.z << endl;
	}

	// Triangles
	for (const auto t : triangles){
		fout << "3 " << t.N[0] << ' ' << t.N[1] << ' ' << t.N[2] << endl;
	}

	fin.close();

    return EXIT_SUCCESS;
}