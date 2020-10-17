//
// Created by Adithya Swaminathan on 10/17/20.
//
#include <core/probability_finder.h>

#include <catch2/catch.hpp>
#include <fstream>

using std::string;
using std::vector;

TEST_CASE("Processing Prior Probabilities") {
  std::ifstream ifstream;
  naivebayes::Model model(4);
  model.ReadLabels("data/testinglabel");
  std::ifstream istream("data/testingimages");
  while(istream.good()) {
    istream >> model;
  }
  naivebayes::ProbabilityFinder p_finder;
  vector<double> vector = p_finder.CalculatePriorProbabilities("data/testinglabel", model);

  SECTION("Testing values in testing labels") {
    REQUIRE(vector.at(0) == Approx(0.154).epsilon(0.01));
    REQUIRE(vector.at(7) == Approx(0.154).epsilon(0.01));
    REQUIRE(vector.at(4) == Approx(0.154).epsilon(0.01));
  }

  SECTION("Testing values not in testing labels") {
    REQUIRE(vector.at(1) == Approx(0.077).epsilon(0.01));
    REQUIRE(vector.at(3) == Approx(0.077).epsilon(0.01));
    REQUIRE(vector.at(8) == Approx(0.077).epsilon(0.01));
  }
}

TEST_CASE("Processing Pixel Probabilities") {
  std::ifstream ifstream;
  naivebayes::Model model(4);
  naivebayes::ProbabilityFinder p_finder;
  model.ReadLabels("data/testinglabel");
  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }

  SECTION("Testing values in testing labels") {
    SECTION("Shaded Pixels") {
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 0, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 4, std::make_pair(1, 2)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 7, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
    }

    SECTION("Unshaded Pixels") {
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 7, std::make_pair(3, 2)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 4, std::make_pair(0, 1)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 0, std::make_pair(1, 1)) ==
              Approx(0.333333).epsilon(0.01));
    }
  }

  SECTION("Testing values not in testing labels") {
    REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 3, std::make_pair(0,0)) == Approx(0.5).epsilon(0.01));
    REQUIRE(p_finder.FindProbabilityOfShadingAtPoint(model, 9, std::make_pair(3,2)) == Approx(0.5).epsilon(0.01));
  }
}