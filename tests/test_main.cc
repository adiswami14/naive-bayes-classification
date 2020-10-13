#define CATCH_CONFIG_MAIN
#include <core/image.h>
#include <catch2/catch.hpp>

using std::vector;

TEST_CASE("File Reading Tests") {
  SECTION("Reading over Training Labels") {
    std::ifstream ifstream;
    naivebayes::Image image;
    ifstream.open("mnistdatatraining/traininglabels");
    while(ifstream.good()) {
      ifstream >> image;
    }
    REQUIRE(image.GetTrainingLabelVec().size() == 5000);
    REQUIRE(image.GetNumOfImagesInClass(0) == 479);
  }
}