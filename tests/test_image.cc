#include <core/image.h>

#include <catch2/catch.hpp>

using std::vector;

TEST_CASE("Accessing Image member variables"){
  SECTION("Accessing Image Vector") {
    naivebayes::Image image;
    REQUIRE_THROWS_AS(image.SetImageVector(vector<vector<char>>()), std::invalid_argument);
    vector<char> vec (3, 0);
    vector<vector<char>> vec_2d;
    vec_2d.push_back(vec);
    image.SetImageVector(vec_2d);
    REQUIRE(vec_2d == image.GetImageVector());
  }

  SECTION("Accessing Image Class"){
    naivebayes::Image image;
    REQUIRE_THROWS_AS(image.SetImageClass(12), std::invalid_argument);
    image.SetImageClass(4);
    REQUIRE(image.GetImageClass() == 4);
  }
}
