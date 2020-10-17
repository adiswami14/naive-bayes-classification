//
// Created by Adithya Swaminathan on 10/17/20.
//
#include <core/arg_handler.h>

#include <catch2/catch.hpp>
#include <fstream>

using std::vector;
using std::string;
using std::map;

TEST_CASE("Processing Command Line Arguments") {
  naivebayes::ArgHandler argHandler;
  naivebayes::Model model;
  char** argv;
  SECTION("Invalid arguments") {
    //Passing in empty argument
    const char* n_argv[] = {" "};
    argv = const_cast<char**>(n_argv);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(1, argv, model, 28), std::invalid_argument);

    //Passing in argument that is not accounted for
    const char* n_argv1[] = {"Hey"};
    argv = const_cast<char**>(n_argv1);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(1, argv, model, 28), std::invalid_argument);

    //Passing in empty whitespace argument
    const char* n_argv2[] = {"     "};
    argv = const_cast<char**>(n_argv2);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(1, argv, model, 28), std::invalid_argument);

    //Passing in empty argument vector
    argv = {};
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(1, argv, model, 28), std::invalid_argument);
  }
  SECTION("Training and saving model") {
    //valid training and saving arguments
    const char* n_argv[] = {"filename", "train", "data/testinglabel", "data/testingimages", "save", "data/test_probs"};
    argv = const_cast<char**>(n_argv);
    argHandler.ProcessCommandLineArgs(6, argv, model, 3);
    std::ifstream ifstream("data/test_probs");
    int line_count = 0;
    while(ifstream.good()) {
      string s;
      getline(ifstream, s);
      if(s.size()>0)
        line_count++;
    }
    REQUIRE(line_count == 90);

    //Argument that's too short
    const char* n_argv1[] = {"filename", "train", "training_labels_file_name", "training_image_file_name"};
    argv = const_cast<char**>(n_argv1);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(4, argv, model, 3), std::invalid_argument);

    //Argument that's too long
    const char* n_argv2[] = {"filename", "train", "training_labels_file_name", "training_image_file_name", "save", "data_file_name", "extra", "arguments"};
    argv = const_cast<char**>(n_argv2);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(8, argv, model, 3), std::invalid_argument);
  }
  SECTION("Loading model") {
    //valid loading arguments
    const char* n_argv[] = {"filename","load", "data/test_probs"};
    argv = const_cast<char**>(n_argv);
    argHandler.ProcessCommandLineArgs(3, argv, model, 3);
    vector<double> loaded_prior_probs = argHandler.GetLoadedPriorProbabilities();
    map<size_t, vector<vector<double>>> m = argHandler.GetLoadedFrequencyMap();
    REQUIRE(loaded_prior_probs.at(0) == Approx(0.154).epsilon(0.01));
    REQUIRE(loaded_prior_probs.at(7) == Approx(0.154).epsilon(0.01));
    REQUIRE(loaded_prior_probs.at(4) == Approx(0.154).epsilon(0.01));
    REQUIRE(loaded_prior_probs.at(1) == Approx(0.077).epsilon(0.01));
    REQUIRE(loaded_prior_probs.at(9) == Approx(0.077).epsilon(0.01));
    REQUIRE(m[0][0][0] == Approx(0.66666667).epsilon(0.01));
    REQUIRE(m[0][1][1] == Approx(0.33333333).epsilon(0.01));
    REQUIRE(m[7][0][0] == Approx(0.66666667).epsilon(0.01));
    REQUIRE(m[1][2][1] == Approx(0.5).epsilon(0.01));

    //Argument that's too short
    const char* n_argv1[] = {"filename","load"};
    argv = const_cast<char**>(n_argv1);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(2, argv, model, 3), std::invalid_argument);

    //Argument that's too long
    const char* n_argv2[] = {"filename","load", "extra", "argument"};
    argv = const_cast<char**>(n_argv2);
    REQUIRE_THROWS_AS(argHandler.ProcessCommandLineArgs(2, argv, model, 3), std::invalid_argument);
  }
}