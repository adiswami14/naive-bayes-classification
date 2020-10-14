#include <core/model.h>

#include <catch2/catch.hpp>
#include <fstream>

using std::map;
using std::vector;

void ReadTestFiles(std::ifstream &ifstream, naivebayes::Model &model) {
  ifstream.open("data/testinglabel.txt");
  while(ifstream.good()) {
    ifstream >> model;
  }
  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
}

void ReadTrainingFiles(std::ifstream &ifstream, naivebayes::Model &model) {
  ifstream.open("mnistdatatraining/traininglabels");
  while(ifstream.good()) {
    ifstream >> model;
  }

  std::ifstream ifstream_images;
  ifstream_images.open("mnistdatatraining/trainingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
}

TEST_CASE("Reading through datasets") {
  SECTION("Reading Training Dataset") {
    std::ifstream ifstream;
    naivebayes::Model model;
    ReadTrainingFiles(ifstream, model);
    REQUIRE(model.GetTrainingLabelVec().size() == 5000);
    REQUIRE(model.GetNumOfImagesInClass(0) == 479);
    REQUIRE(model.GetImageList().size() == 5000);

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

    naivebayes::Model model2;
    REQUIRE(f_map == model2.GetFrequencyMap());
  }

  SECTION("Testing Personal Dataset") {
    std::ifstream ifstream;
    naivebayes::Model model(4);
    ReadTestFiles(ifstream, model);
    REQUIRE(model.GetTrainingLabelVec().size()==3);
    REQUIRE(model.GetImageList().size()==3);
    REQUIRE(model.GetNumOfImagesInClass(0) ==1);
    REQUIRE(model.GetNumOfImagesInClass(7) ==1);
    REQUIRE(model.GetNumOfImagesInClass(4) ==1);

    SECTION("Check Training Label Vector") {
      vector<int> training_vec;
      training_vec.push_back(0);
      training_vec.push_back(7);
      training_vec.push_back(4);
      REQUIRE(training_vec == model.GetTrainingLabelVec());
    }

    SECTION("Check Raster List") {
      //goes through first element of raster list from testing data
      naivebayes::Image image = model.GetImageList().at(0);
      vector<vector<char>> big_vec;
      vector<char> row_1 = {'*', '*', '*', '*'};
      vector<char> row_2 = {'*', ' ', ' ', '*'};
      vector<char> row_3 = {'*', '*', '*', '*'};
      vector<char> row_4 = {' ', ' ', ' ', ' '};
      big_vec.push_back(row_1);
      big_vec.push_back(row_2);
      big_vec.push_back(row_3);
      big_vec.push_back(row_4);
      REQUIRE(image.GetImageVector() == big_vec);
    }
  }
}

TEST_CASE("Accessing Model Member variables") {
  naivebayes::Model model;
  SECTION("Accessing Training Label Vector") {
    REQUIRE_THROWS_AS(model.SetTrainingLabelVec(vector<int>()),
                      std::invalid_argument);
    vector<int> v;
    v.push_back(2);
    model.SetTrainingLabelVec(v);
    REQUIRE(v == model.GetTrainingLabelVec());
  }

  SECTION("Accessing Frequency Map") {
    REQUIRE_THROWS_AS(
        model.SetFrequencyMap(map<size_t, vector<vector<size_t>>>()),
        std::invalid_argument);
    map<size_t, vector<vector<size_t>>> v;
    v[0] = vector<vector<size_t>>();
    model.SetFrequencyMap(v);
    REQUIRE(v == model.GetFrequencyMap());
  }

  SECTION("Accessing Image List") {
    REQUIRE_THROWS_AS(model.SetImageList(vector<naivebayes::Image>()),
                      std::invalid_argument);
    vector<naivebayes::Image> v;
    naivebayes::Image curr_image;
    v.push_back(curr_image);
    model.SetImageList(v);
    REQUIRE(v.at(0).GetImageVector() ==
            model.GetImageList().at(0).GetImageVector());
    REQUIRE(v.at(0).GetImageClass() ==
            model.GetImageList().at(0).GetImageClass());
  }
}

