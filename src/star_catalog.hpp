#ifndef STAR_CATALOG_H
#define STAR_CATALOG_H

#include <vector>
#include <fstream>
#include <tuple>
#include <set>
#include <cstdint>

struct CatalogHeader
{
    int32_t star0;
    int32_t star1;
    int32_t starn;
    int32_t stnum;
    int32_t mprop;
    int32_t nmag;
    int32_t nbent;
};

class StarCatalogEntry
{
    float cat_num;
    double ra, dec;
    char spectral_type[2];
    uint16_t mag100;
    float ra_pm, dec_pm;
public:
    constexpr float catalog_id() {return cat_num;}
    constexpr float magnitude() {return mag100 / 100.;}
    constexpr double right_ascension() {return ra;}
    constexpr double declination() {return dec;}

    // This constructor is just here for unit testing
    StarCatalogEntry(double ra, double dec) :
        cat_num(1.), ra(ra), dec(dec), spectral_type{0, 0},
        mag100(0), ra_pm(0.), dec_pm(0.) {}
} __attribute__((packed));

class StarCatalog
{
    std::vector<StarCatalogEntry> entries;

public:
    StarCatalog(std::ifstream& infile);
    StarCatalog(std::vector<StarCatalogEntry>& entries);

    constexpr std::vector<StarCatalogEntry>& get_entries() {return entries;}
};

struct CatalogPair
{
    int s1, s2;
    double dist;
};

class StarCatalogManager
{
    StarCatalog catalog;
    std::vector<CatalogPair> edges;
public:
    StarCatalogManager(std::ifstream& infile);
    StarCatalogManager(StarCatalog catalog);
    std::set<int> get_possible_stars(double dist);
    constexpr std::vector<StarCatalogEntry>& get_entries() {return catalog.get_entries();}
private:
    void calculate_edges();
};

#endif /* STAR_CATALOG_H */
