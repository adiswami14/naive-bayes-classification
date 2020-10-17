//
// Created by Adithya Swaminathan on 10/17/20.
//

#ifndef NAIVE_BAYES_ARG_HANDLER_H
#define NAIVE_BAYES_ARG_HANDLER_H


#include <vector>
#include "probability_finder.h"

namespace naivebayes {

using std::vector;
using std::string;

class ArgHandler {
 public:
  /**
   * Takes in main's parameters, and processes them to run the correct functions
   * @param argc Argument count
   * @param argv Argument vector
   * @param model The current model instance
   */
  void ProcessCommandLineArgs(int argc, char *argv[], naivebayes::Model &model);
 private:
  /**
   * Writes calculated probabilities to probabilities txt file
   * @param ofstream The output stream to write to a file
   * @param pf The current instance of probability finder
   * @param model Instance of Model containing all data for all images
   * @param prior_probabilities A vector of all prior probabilities corresponding to each class of images
   */
  void WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::ProbabilityFinder &pf, naivebayes::Model &model, vector<double> prior_probabilities);

  /**
   * Parses through list of all commands and calls the according functions
   * @param all_args Vector of strings containing all command line arguments passed in
   * @param pf Current instance of ProbabilityFinder
   * @param model Current instance of Model class
   */
  void RunCommandLineFunctions(const vector<string> &all_args, naivebayes::ProbabilityFinder &pf, naivebayes::Model &model);
};

}

#endif  // NAIVE_BAYES_ARG_HANDLER_H
