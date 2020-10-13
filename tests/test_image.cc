#include <core/image.h>

#include <catch2/catch.hpp>
#include <fstream>

using std::map;
using std::vector;

void ReadTestFiles(std::ifstream &ifstream, naivebayes::Image &image) {
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

void ReadTrainingFiles(std::ifstream &ifstream, naivebayes::Image &image) {
  ifstream.open("mnistdatatraining/traininglabels");
  while(ifstream.good()) {
    ifstream >> image;
  }

  std::ifstream ifstream_images;
  ifstream_images.open("mnistdatatraining/trainingimages");
  while (ifstream_images.good()) {
    ifstream_images >> image;
  }
}

TEST_CASE("File Reading Tests") {
  SECTION("Reading Training Dataset") {
    std::ifstream ifstream;
    naivebayes::Image image;
    ReadTrainingFiles(ifstream, image);
    REQUIRE(image.GetTrainingLabelVec().size() == 5000);
    REQUIRE(image.GetNumOfImagesInClass(0) == 479);
    REQUIRE(image.GetRasterList().size() == 5000);

    //make sure initialization of maps makes all value 0
    vector<vector<size_t>> vec_big;
    vector<size_t> vec_small;
    map<size_t, vector<vector<size_t>>> f_map;
    for(size_t cl = 0; cl <= 9; cl++) {
      for (size_t x = 0; x < 28; x++) {
        for (size_t y = 0; y < 28; y++) {
          vec_small = vector<size_t>(28, 0);
        }
        vec_big.push_back(vec_small);
      }
      f_map[cl] = vec_big;
      vec_big.clear();
    }

    naivebayes::Image image2;
    REQUIRE(f_map == image2.GetFrequencyMap());
  }

  SECTION("Testing Personal Dataset") {
    std::ifstream ifstream;
    naivebayes::Image image(4);
    ReadTestFiles(ifstream, image);
    REQUIRE(image.GetTrainingLabelVec().size()==3);
    REQUIRE(image.GetRasterList().size()==3);
    REQUIRE(image.GetNumOfImagesInClass(0) ==1);
    REQUIRE(image.GetNumOfImagesInClass(7) ==1);
    REQUIRE(image.GetNumOfImagesInClass(4) ==1);

    SECTION("Check Training Label Vector") {
      vector<int> training_vec;
      training_vec.push_back(0);
      training_vec.push_back(7);
      training_vec.push_back(4);
      REQUIRE(training_vec == image.GetTrainingLabelVec());
    }

    SECTION("Check Raster List") {
      //goes through first element of raster list from testing data
      naivebayes::Raster raster = image.GetRasterList().at(0);
      vector<vector<char>> big_vec;
      vector<char> row_1 = {'*', '*', '*', '*'};
      vector<char> row_2 = {'*', ' ', ' ', '*'};
      vector<char> row_3 = {'*', '*', '*', '*'};
      vector<char> row_4 = {' ', ' ', ' ', ' '};
      big_vec.push_back(row_1);
      big_vec.push_back(row_2);
      big_vec.push_back(row_3);
      big_vec.push_back(row_4);
      REQUIRE(raster.GetRasterVector() == big_vec);
    }
  }
}

TEST_CASE("Setter & Getter Methods") {
  naivebayes::Image image;
  SECTION("Training Label Vector") {
    REQUIRE_THROWS_AS(image.SetTrainingLabelVec(vector<int>()),
                      std::invalid_argument);
    vector<int> v;
    v.push_back(2);
    image.SetTrainingLabelVec(v);
    REQUIRE(v == image.GetTrainingLabelVec());
  }

  SECTION("Frequency Map") {
    REQUIRE_THROWS_AS(
        image.SetFrequencyMap(map<size_t, vector<vector<size_t>>>()),
        std::invalid_argument);
    map<size_t, vector<vector<size_t>>> v;
    v[0] = vector<vector<size_t>>();
    image.SetFrequencyMap(v);
    REQUIRE(v == image.GetFrequencyMap());
  }

  SECTION("Raster List") {
    REQUIRE_THROWS_AS(image.SetRasterList(vector<naivebayes::Raster>()),
                      std::invalid_argument);
    vector<naivebayes::Raster> v;
    naivebayes::Raster curr_raster;
    v.push_back(curr_raster);
    image.SetRasterList(v);
    REQUIRE(v.at(0).GetRasterVector() ==
            image.GetRasterList().at(0).GetRasterVector());
    REQUIRE(v.at(0).GetRasterImageClass() ==
            image.GetRasterList().at(0).GetRasterImageClass());
  }
}

