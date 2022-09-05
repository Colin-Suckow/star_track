#include <iostream>
#include "star_catalog.hpp"

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
}