#define CATCH_CONFIG_MAIN
#include <core/image.h>
#include <catch2/catch.hpp>

using std::vector;

void ReadTestFilesMain(std::ifstream &ifstream, naivebayes::Image &image) {
  ifstream.open("data/testinglabel.txt");
  while(ifstream.good()) {
    ifstream >> image;
  }
  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> image;
  }
}

vector<double> CalculatePriorProbabilities(std::ifstream &ifstream, naivebayes::Image& image) {
  vector<double> prior_probabilities;
  ifstream.open("data/testinglabel.txt");
  while(ifstream.good()) {
    ifstream>> image;
  }

  vector<int> training_vec = image.GetTrainingLabelVec();
  for(size_t num = 0; num <= 9; num++) {
    //the number of times a certain number shows up in training labels over total number of training labels
    double probability_of_num =
        (image.kLaplaceSmoothingFactor+ static_cast<double>(std::count(training_vec.begin(), training_vec.end(), num)))
        /((10*image.kLaplaceSmoothingFactor)+training_vec.size());

    prior_probabilities.push_back(probability_of_num);
  }
  return prior_probabilities;
}

double FindProbabilityOfShadingAtPoint(const naivebayes::Image &image, size_t raster_image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = image.GetFrequencyMap()[raster_image_class][pair.first][pair.second];
  numerator = image.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*image.kLaplaceSmoothingFactor)+image.GetNumOfImagesInClass(raster_image_class);
  return numerator/denominator;
}

TEST_CASE("Processing Prior Probabilities") {
  std::ifstream ifstream;
  naivebayes::Image image(4);
  ReadTestFilesMain(ifstream, image);
  vector<double> vector = CalculatePriorProbabilities(ifstream, image);

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
  naivebayes::Image image(4);
  ReadTestFilesMain(ifstream, image);

  SECTION("Testing values in testing labels") {
    SECTION("Shaded Pixels") {
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 0, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 4, std::make_pair(1, 2)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 7, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
    }

    SECTION("Unshaded Pixels") {
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 7, std::make_pair(3, 2)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 4, std::make_pair(0, 1)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(image, 0, std::make_pair(1, 1)) ==
              Approx(0.333333).epsilon(0.01));
    }
  }

  SECTION("Testing values not in testing labels") {
    REQUIRE(FindProbabilityOfShadingAtPoint(image, 3, std::make_pair(0,0)) == Approx(0.5).epsilon(0.01));
    REQUIRE(FindProbabilityOfShadingAtPoint(image, 9, std::make_pair(3,2)) == Approx(0.5).epsilon(0.01));
  }
}