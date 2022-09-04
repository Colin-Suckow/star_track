#include <iostream>
#include "star_catalog.hpp"
#include "adjacency_list.hpp"

#include <math.h>

#define MAX_DIST 0.6859144

using namespace std;

double angular_distance(double s1_ra, double s1_dec, double s2_ra,double s2_dec)
{
    return acos(sin(s1_dec) * sin(s2_dec) + cos(s1_dec) * cos(s2_dec) * cos(s2_ra - s1_ra));
}

int main()
{
    std::cout << "Loading star catalog..." << std::endl;
    ifstream infile("../BSC5", ios::in | ios::binary);
    if(!infile.good())
    {
        std::cout << "Failed to star catalog file" << std::endl;
        exit(-1);
    }
    StarCatalog catalog(infile);
    infile.close();

    auto star_adjacency_list = AdjacencyList(9111);
    auto s1i = 0;

    std::cout << "Calculating star pairs..." << std::endl;
    for(auto& s1 : catalog.get_entries())
    {
        auto s2i = 0;
        for(auto& s2 : catalog.get_entries())
        {
            auto dist = angular_distance(s1.right_ascension(), s1.declination(), s2.right_ascension(), s2.declination());
            if (s1i != s2i && dist != 0 && dist < MAX_DIST)
            {
                star_adjacency_list.add_edge(s1i, s2i, dist);
            }
            s2i++;
        }
        s1i++;
    }
}