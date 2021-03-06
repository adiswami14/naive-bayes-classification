#include <core/model.h>

#include <catch2/catch.hpp>
#include <fstream>

using std::map;
using std::vector;

void ReadTestFiles(std::ifstream &ifstream, naivebayes::Model &model) {
  model.ReadLabels("data/testinglabel");

  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
}

void ReadTrainingFiles(std::ifstream &ifstream, naivebayes::Model &model) {
  model.ReadLabels("mnistdatatraining/traininglabels");

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
    REQUIRE(model.GetNumOfImagesInClass()[0] == 479);
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
    REQUIRE(model.GetNumOfImagesInClass()[0] == 1);
    REQUIRE(model.GetNumOfImagesInClass()[4] == 1);
    REQUIRE(model.GetNumOfImagesInClass()[7] == 1);

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

TEST_CASE("Processing Prior Probabilities") {
  std::ifstream ifstream;
  naivebayes::Model model(4);
  model.ReadLabels("data/testinglabel");
  std::ifstream istream("data/testingimages");
  while(istream.good()) {
    istream >> model;
  }
  vector<double> vector = model.GetPriorProbabilities();

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
  model.ReadLabels("data/testinglabel");
  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
  SECTION("Testing values in testing labels") {
    SECTION("Shaded Pixels") {
      REQUIRE(model.FindProbabilityOfShadingAtPoint(0, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(model.FindProbabilityOfShadingAtPoint(4, std::make_pair(1, 2)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(model.FindProbabilityOfShadingAtPoint(7, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
    }

    SECTION("Unshaded Pixels") {
      REQUIRE(model.FindProbabilityOfShadingAtPoint(7, std::make_pair(3, 2)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(model.FindProbabilityOfShadingAtPoint(4, std::make_pair(0, 1)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(model.FindProbabilityOfShadingAtPoint(0, std::make_pair(1, 1)) ==
              Approx(0.333333).epsilon(0.01));
    }
  }

  SECTION("Testing values not in testing labels") {
    REQUIRE(model.FindProbabilityOfShadingAtPoint(3, std::make_pair(0,0)) == Approx(0.5).epsilon(0.01));
    REQUIRE(model.FindProbabilityOfShadingAtPoint(9, std::make_pair(3,2)) == Approx(0.5).epsilon(0.01));
  }
}

TEST_CASE("Classifying Images from Small Dataset") {
  naivebayes::Model model(4);
  model.ReadLabels("data/testinglabel");
  std::ifstream ifstream_images;
  ifstream_images.open("data/testingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
  SECTION("Classifying images") {
    naivebayes::Model model1(4, model.GetFeatureProbMap());
    model1.ReadLabels("data/dataset_labels");
    std::ifstream ifstream;
    ifstream.open("data/dataset_images");
    while (ifstream.good()) {
      ifstream >> model1;
    }
    vector<size_t> best_class_list = model1.GetBestClassList();
    REQUIRE(best_class_list[0] == model1.GetTrainingLabelVec()[0]);
    REQUIRE(best_class_list[1] == model1.GetTrainingLabelVec()[1]);
    REQUIRE(best_class_list[2] == model1.GetTrainingLabelVec()[2]);
  }
}

TEST_CASE("Classifying Image Sanity Check") {
  naivebayes::Model model;
  model.ReadLabels("mnistdatatraining/traininglabels");
  std::ifstream ifstream_images;
  ifstream_images.open("mnistdatatraining/trainingimages");
  while (ifstream_images.good()) {
    ifstream_images >> model;
  }
  SECTION("Get Overall Accuracy of Tests") {
    naivebayes::Model model1(model.GetFeatureProbMap());
    model1.ReadLabels("mnistdatavalidation/testlabels");
    std::ifstream ifstream;
    ifstream.open("mnistdatavalidation/testimages");
    while (ifstream.good()) {
      ifstream >> model1;
    }
    size_t matches = 0;
    vector<size_t> v = model1.GetBestClassList();
    for(int x=0;x<1000;x++) {
      if(v.at(x) == model1.GetTrainingLabelVec().at(x))
        matches++;
    }
    REQUIRE(matches>=770);
  }
}

