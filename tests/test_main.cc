#define CATCH_CONFIG_MAIN
#include <core/image.h>
#include <catch2/catch.hpp>

using std::vector;

TEST_CASE("Training Labels Test") {
  SECTION("Operator Overloading") {
    std::ifstream myFile;
    naivebayes::Image image;
    myFile.open("mnistdatatraining/traininglabels");
    while(myFile.good()) {
      myFile >> image;
    }
    REQUIRE(image.GetTrainingLabelVec().size() == 5000);
  }
}