#include <iostream>
#include <iomanip>
#include <set>
#include <unordered_map>
#include <algorithm>


#include "star_catalog.hpp"
#include "candidate_source.hpp"
#include "fmt/core.h"

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

    for (auto& star : catalog_manager.get_entries())
    {
        fmt::println("DEC: {}", star.declination());
    }

    auto a7c = Camera(60., 35.6, 23.8, 6000, 4000);
    auto source_path = "../photos/60mm_test1.JPG";
    std::cout << "Loading source image '" << source_path << "'... " << std::flush;
    auto star_source = CandidateSource(source_path, a7c);
    std::cout << "done! Found " << star_source.size() << " candidates in input image." << std::endl;

    auto total_pairs = pow(star_source.size(), 2);
    auto count = 0;

    std::vector<std::unordered_map<int,int>> possible_matches;
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

            for (const auto match_index : matches)
            {
                possible_matches[s1i][match_index]++;
            }
  
            count++;
        }
        s1i++;
    }

    std::cout << std::endl;

    // Sort our matches by number of matches, then magnitude to break any ties


    // print match counts
    auto positive_matches = 0;
    for (auto& matches : possible_matches)
    {
        // Sort the matches
        auto best_match_index = std::max_element(std::begin(matches), std::end(matches), [](const std::pair<int, int> &p1, const std::pair<int, int> &p2)
                         { return p1.second < p2.second; });
        auto best_star = catalog_manager.get_entries()[best_match_index->first];
        // Print the star
        fmt::println("ID: {}, DEC: {}, RA: {}, MATCHES: {}", best_star.catalog_id(), best_star.declination(), best_star.right_ascension(), best_match_index->second);
    }
}