#include <catch2/catch_test_macros.hpp>
#include "../src/star_catalog.hpp"

#include <vector>
#include <set>

TEST_CASE("Candidate stars are correctly found")
{
    SECTION("Catalog can find stars within distance")
    {
        std::vector<StarCatalogEntry> mock_entries = {
            StarCatalogEntry(0, 1),
            StarCatalogEntry(0, 1.3)
        };
        StarCatalogManager catalog_manager = StarCatalogManager(StarCatalog(mock_entries));
        std::set<int> correct = {0, 1};
        REQUIRE(catalog_manager.get_possible_stars(0.3) == correct);
    }

    SECTION("Catalog rejects stars that are too close")
    {
        std::vector<StarCatalogEntry> mock_entries = {
            StarCatalogEntry(0, 1),
            StarCatalogEntry(0, 1.1)
        };
        StarCatalogManager catalog_manager = StarCatalogManager(StarCatalog(mock_entries));
        std::set<int> correct = {};
        REQUIRE(catalog_manager.get_possible_stars(0.3) == correct);
    }

    SECTION("Catalog rejects stars that are too far")
    {
        std::vector<StarCatalogEntry> mock_entries = {
            StarCatalogEntry(0, 1),
            StarCatalogEntry(0, 1.5)
        };
        StarCatalogManager catalog_manager = StarCatalogManager(StarCatalog(mock_entries));
        std::set<int> correct = {};
        REQUIRE(catalog_manager.get_possible_stars(0.3) == correct);
    }

    SECTION("Catalog rejects stars that have no possible edges")
    {
        std::vector<StarCatalogEntry> mock_entries = {
            StarCatalogEntry(0, 1),
            StarCatalogEntry(0, 2.5)
        };
        StarCatalogManager catalog_manager = StarCatalogManager(StarCatalog(mock_entries));
        std::set<int> correct = {};
        REQUIRE(catalog_manager.get_possible_stars(0.3) == correct);
    }
}