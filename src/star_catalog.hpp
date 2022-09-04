#ifndef STAR_CATALOG_H
#define STAR_CATALOG_H

#include <vector>
#include <fstream>

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
} __attribute__((packed));

class StarCatalog
{
    std::vector<StarCatalogEntry> entries;

public:
    StarCatalog(std::ifstream& infile);

    constexpr std::vector<StarCatalogEntry>& get_entries() {return entries;}
};

#endif /* STAR_CATALOG_H */
