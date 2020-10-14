#define CATCH_CONFIG_MAIN
#include <core/model.h>

#include <catch2/catch.hpp>

using std::vector;
using std::string;


void ReadTestFilesMain(std::ifstream &ifstream, naivebayes::Model &model) {
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

vector<double> CalculatePriorProbabilities(std::ifstream &ifstream, naivebayes::Model& model) {
  vector<double> prior_probabilities;
  ifstream.open("data/testinglabel.txt");
  while(ifstream.good()) {
    ifstream>> model;
  }

  vector<int> training_vec = model.GetTrainingLabelVec();
  for(size_t num = 0; num <= 9; num++) {
    //the number of times a certain number shows up in training labels over total number of training labels
    double probability_of_num =
        (model.kLaplaceSmoothingFactor+ static_cast<double>(std::count(training_vec.begin(), training_vec.end(), num)))
        /((10*model.kLaplaceSmoothingFactor)+training_vec.size());

    prior_probabilities.push_back(probability_of_num);
  }
  return prior_probabilities;
}

double FindProbabilityOfShadingAtPoint(const naivebayes::Model &model, size_t image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = model.GetFrequencyMap()[image_class][pair.first][pair.second];
  numerator = model.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*model.kLaplaceSmoothingFactor)+model.GetNumOfImagesInClass(image_class);
  return numerator/denominator;
}

vector<string> RunCommandLineFunctions(const vector<string> &all_args) {
  if(all_args.empty()) {
    throw std::invalid_argument("Empty argument vector!");
  }
  vector<string> command_args;
  //train TrainingLabelsName TrainingImagesName save FileToWriteTo
  if(all_args[0] == "train") {
    if(all_args.size() == 5) {
      command_args.push_back(all_args[0]);
      command_args.push_back(all_args[1]); //equivalent of reading training labels
      command_args.push_back(all_args[2]); //equivalent of reading training images
      if (all_args[3] == "save") {
        command_args.push_back(all_args[3]);
        command_args.push_back(all_args[4]); //equivalent of saving data to file
      }
    } else command_args.push_back("Invalid amount of command line args. Please try again.");
  } else if(all_args[0] == "load") {  //load FileToLoadUp
    if(all_args.size() == 2) {
      command_args.push_back(all_args[0]);
      command_args.push_back(all_args[1]); //equivalent of loading file
    } else command_args.push_back("Invalid amount of command line args. Please try again.");
  } else {
    command_args.push_back("Invalid command line arg. Please try again.");
  }
  return command_args;
}

TEST_CASE("Processing Prior Probabilities") {
  std::ifstream ifstream;
  naivebayes::Model model(4);
  ReadTestFilesMain(ifstream, model);
  vector<double> vector = CalculatePriorProbabilities(ifstream, model);

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
  ReadTestFilesMain(ifstream, model);

  SECTION("Testing values in testing labels") {
    SECTION("Shaded Pixels") {
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 0, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 4, std::make_pair(1, 2)) ==
              Approx(0.666667).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 7, std::make_pair(0, 0)) ==
              Approx(0.666667).epsilon(0.01));
    }

    SECTION("Unshaded Pixels") {
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 7, std::make_pair(3, 2)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 4, std::make_pair(0, 1)) ==
              Approx(0.333333).epsilon(0.01));
      REQUIRE(FindProbabilityOfShadingAtPoint(model, 0, std::make_pair(1, 1)) ==
              Approx(0.333333).epsilon(0.01));
    }
  }

  SECTION("Testing values not in testing labels") {
    REQUIRE(FindProbabilityOfShadingAtPoint(model, 3, std::make_pair(0,0)) == Approx(0.5).epsilon(0.01));
    REQUIRE(FindProbabilityOfShadingAtPoint(model, 9, std::make_pair(3,2)) == Approx(0.5).epsilon(0.01));
  }
}

TEST_CASE("Processing Command Line Arguments") {
  SECTION("Invalid arguments") {
    //Passing in empty argument
    vector<string> command_args;
    command_args.push_back("");
    vector<string> check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid command line arg. Please try again.");

    //Passing in argument that is not accounted for
    command_args.clear();
    command_args.push_back("Hey");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid command line arg. Please try again.");

    //Passing in empty whitespace argument
    command_args.clear();
    command_args.push_back("      ");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid command line arg. Please try again.");

    //Passing in empty vector
    command_args.clear();
    REQUIRE_THROWS_AS(RunCommandLineFunctions(command_args), std::invalid_argument);
  }
  SECTION("Training and saving model") {
    //valid training and saving arguments
    vector<string> command_args;
    command_args.push_back("train");
    command_args.push_back("training_labels_file_name");
    command_args.push_back("training_image_file_name");
    command_args.push_back("save");
    command_args.push_back("data_file_name");
    vector<string> check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec == command_args);

    //Argument that's too short
    command_args.clear();
    command_args.push_back("train");
    command_args.push_back("training_labels_file_name");
    command_args.push_back("training_image_file_name");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid amount of command line args. Please try again.");

    //Argument that's too long
    command_args.clear();
    command_args.push_back("train");
    command_args.push_back("training_labels_file_name");
    command_args.push_back("training_image_file_name");
    command_args.push_back("save");
    command_args.push_back("data_file_name");
    command_args.push_back("extra");
    command_args.push_back("arguments");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid amount of command line args. Please try again.");
  }
  SECTION("Loading model") {
    //valid loading arguments
    vector<string> command_args;
    command_args.push_back("load");
    command_args.push_back("data_file_name");
    vector<string> check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec == command_args);

    //Argument that's too short
    command_args.clear();
    command_args.push_back("load");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid amount of command line args. Please try again.");

    //Argument that's too long
    command_args.clear();
    command_args.push_back("load");
    command_args.push_back("extra");
    command_args.push_back("argument");
    check_vec = RunCommandLineFunctions(command_args);
    REQUIRE(check_vec[0] == "Invalid amount of command line args. Please try again.");
  }
}