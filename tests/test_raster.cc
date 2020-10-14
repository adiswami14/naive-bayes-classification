#include <core/raster.h>
#include <catch2/catch.hpp>

using std::vector;

TEST_CASE("Accessing Raster member variables"){
  SECTION("Accessing Raster Vector") {
    naivebayes::Raster raster;
    REQUIRE_THROWS_AS(raster.SetRasterVector(vector<vector<char>>()), std::invalid_argument);
    vector<char> vec (3, 0);
    vector<vector<char>> vec_2d;
    vec_2d.push_back(vec);
    raster.SetRasterVector(vec_2d);
    REQUIRE(vec_2d == raster.GetRasterVector());
  }

  SECTION("Accessing Raster Image Class"){
    naivebayes::Raster raster;
    REQUIRE_THROWS_AS(raster.SetRasterImageClass(12), std::invalid_argument);
    raster.SetRasterImageClass(4);
    REQUIRE(raster.GetRasterImageClass() == 4);
  }
}
