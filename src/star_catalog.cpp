#include "star_catalog.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>

#include <math.h>

#define MAX_DIST 0.6859144
#define MIN_RESOLVABLE_DIST 9.902777745407246e-5

double angular_distance(double s1_ra, double s1_dec, double s2_ra,double s2_dec)
{
    return acos(sin(s1_dec) * sin(s2_dec) + cos(s1_dec) * cos(s2_dec) * cos(s2_ra - s1_ra));
}

StarCatalog::StarCatalog(std::ifstream& infile)
{
    // seek past header
    CatalogHeader header;
    infile.read((char *) &header, 28);



    // read star entries
    while(!infile.eof())
    {
        StarCatalogEntry entry = StarCatalogEntry(0., 0.);
        infile.read((char *) &entry, 32);
        entries.push_back(entry);
    }
}

StarCatalog::StarCatalog(std::vector<StarCatalogEntry>& entries) : entries(entries) {}

StarCatalogManager::StarCatalogManager(std::ifstream& infile)
: catalog(infile)
{
    calculate_edges();
}

StarCatalogManager::StarCatalogManager(StarCatalog catalog) : catalog(catalog)
{
    calculate_edges();
}

void StarCatalogManager::calculate_edges()
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
    std::sort(edges.begin(), edges.end(), [](CatalogPair const& p1, CatalogPair const& p2) {return p1.dist < p2.dist;});
}

// Returns a set of all possible stars indicies given an angular distance between the target star and another star.
std::set<int> StarCatalogManager::get_possible_stars(double dist)
{

    std::set<int> result;

    auto within_distance = [dist](CatalogPair pair)
    {
        return abs(pair.dist - dist) <= MIN_RESOLVABLE_DIST;
    };

    std::vector<CatalogPair>::iterator iter = edges.begin();
    while ((iter = std::find_if(iter, edges.end(), within_distance)) != edges.end())
    {
        result.insert(iter->s1);
        iter++;
    }

    return result;
}