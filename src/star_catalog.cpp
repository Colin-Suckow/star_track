#include "star_catalog.hpp"

#include <fstream>
#include <iostream>

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