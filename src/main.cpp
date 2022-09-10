#include <iostream>
#include <iomanip>
#include <set>
#include <unordered_map>

#include "star_catalog.hpp"
#include "candidate_source.hpp"

using namespace std;

int main()
{
    std::cout << "Loading star catalog... " << std::flush;
    ifstream infile("../BSC5", ios::in | ios::binary);
    if(!infile.good())
    {
        std::cout << std::endl << "Could not load star catalog file" << std::endl;
        exit(-1);
    }
    std::cout << "done!" << std::endl;
    std::cout << "Building edge list... " << std::flush;
    StarCatalogManager catalog_manager(infile);
    infile.close();
    std::cout << "done!" << std::endl;

    auto a7c = Camera(60., 35.6, 23.8, 6000, 4000);
    auto source_path = "../photos/60mm_test1.JPG";
    std::cout << "Loading source image '" << source_path << "'... " << std::flush;
    auto star_source = CandidateSource(source_path, a7c);
    std::cout << "done! Found " << star_source.size() << " candidates in input image." << std::endl;

    auto total_pairs = pow(star_source.size(), 2);
    auto count = 0;

    std::vector<std::set<int>> possible_matches;
    possible_matches.resize(star_source.size());

    int s1i = 0;
    for (auto& s1 : star_source.get_candidates())
    {
        for (auto& s2 : star_source.get_candidates())
        {
            // print progress
            if (count % 10 == 0)
            {
                std::cout << "\r" << std::fixed << std::setprecision(0) << count / total_pairs * 100<< "%"<< std::flush; 
            }

            auto dist = star_source.angular_distance_between(s1, s2);
            auto matches = catalog_manager.get_possible_stars(dist);
            if (possible_matches[s1i].empty())
            {
                // Since the set is empty, just fill it with this one
                possible_matches[s1i] = matches;
            }
            else
            {
                // Otherwise use the intersection of the two sets to narrow down the matches
                std::set<int> intersection;
                std::set_intersection(possible_matches[s1i].begin(), possible_matches[s1i].end(), matches.begin(), matches.end(), std::inserter(intersection, intersection.begin()));
                possible_matches[s1i] = intersection;
            }
            count++;
        }
        s1i++;
    }

    std::cout << std::endl;

    // print match counts
    auto positive_matches = 0;
    for (auto& matches : possible_matches)
    {
        if (matches.size() == 1)
        {
            positive_matches++;
            std::cout << setprecision(2);
            std::cout << "ID: " << catalog_manager.get_entries()[*matches.begin()].catalog_id() << " MAG: " << catalog_manager.get_entries()[*matches.begin()].magnitude() << std::endl;
        }
    }
    std::cout << "Matched " << positive_matches << " out of " << star_source.size() << " candidates identified from image." << std::endl;
}