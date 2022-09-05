#include "star_catalog.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

#include <math.h>

#define MAX_DIST 0.6859144

double angular_distance(double s1_ra, double s1_dec, double s2_ra,double s2_dec)
{
    return acos(sin(s1_dec) * sin(s2_dec) + cos(s1_dec) * cos(s2_dec) * cos(s2_ra - s1_ra));
}

StarCatalog::StarCatalog(std::ifstream& infile)
{
    // seek past header
    infile.seekg(28, infile.beg);

    // read star entries
    while(!infile.eof())
    {
        StarCatalogEntry entry;
        infile.read((char *) &entry, 32);
        entries.push_back(entry);
    }
}

StarCatalogManager::StarCatalogManager(std::ifstream& infile)
: catalog(infile)
{
    // Build edge list
    auto s1i = 0;
    for(auto& s1 : catalog.get_entries())
    {
        auto s2i = 0;
        for(auto& s2 : catalog.get_entries())
        {
            auto dist = angular_distance(s1.right_ascension(), s1.declination(), s2.right_ascension(), s2.declination());
            if (s1i != s2i && dist != 0 && dist < MAX_DIST)
            {
                edges.push_back(CatalogPair {s1i, s2i, dist});
            }
            s2i++;
        }
        s1i++;
    }

    // Sort by distance
    std::sort(edges.begin(), edges.end(), [](const CatalogPair& p1, const CatalogPair& p2) {return p1.dist < p2.dist;});
}