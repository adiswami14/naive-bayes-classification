//
// Created by Adithya Swaminathan on 10/17/20.
//

#ifndef NAIVE_BAYES_ARG_HANDLER_H
#define NAIVE_BAYES_ARG_HANDLER_H

#include <vector>

#include <core/model.h>

namespace naivebayes {

using std::vector;
using std::string;

/**
 * Breaks down and handles command line arguments
 */
class ArgHandler {
 public:
  /**
   * Default constructor for ArgHandler class
   */
  ArgHandler();

  /**
   * Overloaded constructor
   * @param image_size The size of each Image
   */
  ArgHandler(size_t image_size);

  /**
   * Takes in main's parameters, and processes them to run the correct functions
   * @param argc Argument count
   * @param argv Argument vector
   * @param model The current model instance
   */
  void ProcessCommandLineArgs(int argc, char *argv[], naivebayes::Model &model);

  /**
   * Gets loaded_feature_prob_map_ variable
   * @return Map of loaded feature probabilities
   */
  map<size_t, vector<vector<double>>> GetLoadedFeatureProbMap();

  /**
   * Gets loaded_prior_probabilities_ variable
   * @return Vector of loaded prior probabilities
   */
  vector<double> GetLoadedPriorProbabilities();
 private:
  /**
   * Writes calculated probabilities to probabilities txt file
   * @param ofstream The output stream to write to a file
   * @param model Instance of Model containing all data for all images
   * @param prior_probabilities A vector of all prior probabilities corresponding to each class of images
   */
  void WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::Model &model, const vector<double> &prior_probabilities);

  /**
   * Parses through list of all commands and calls the according functions
   * @param all_args Vector of strings containing all command line arguments passed in
   * @param model Current instance of Model class
   */
  void RunCommandLineFunctions(const vector<string> &all_args, naivebayes::Model &model);

  /**
   * Initializes loaded_feature_prob_map_ to all 0s
   * @param feature_prob_map_size The desired size of each Image in map
   */
  void InitializeLoadedFeatureProbMap(size_t feature_prob_map_size);

  /**
   * Loads a file and saves values of file to member variables
   * @param filename The name of the file to load
   */
  void LoadFile(const string& filename);

  /**
   * Tests a new model and calculates accuracy of the model
   * @param test_label_file Name of file of test labels
   * @param test_image_file Name of file of test images
   * @param map Feature probability map of trained model
   */
  void TestModel(const string& test_label_file, const string& test_image_file, const std::map<size_t, vector<vector<double>>> &map) const;

  /**
   * Writes probabilities to file after training
   * @param ofstream Output stream to write to file
   * @param filename Name of file to write to
   * @param model Instance of trained model
   * @param prior_probabilities List of prior probabilities
   */
  void SaveToFile(std::ofstream &ofstream, const string &filename, Model &model, const vector<double> &prior_probabilities);

  vector<double> loaded_prior_probabilities_; //vector of prior probabilities for each class loaded in

  map<size_t, vector<vector<double>>> loaded_feature_prob_map_; //map of feature probabilities at each pixel;

  size_t image_size_; //size of each image to handle
};

}

#endif  // NAIVE_BAYES_ARG_HANDLER_H
