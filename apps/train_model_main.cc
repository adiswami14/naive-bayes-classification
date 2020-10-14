#include <core/image.h>
#include <core/model.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <vector>

using std::vector;
using std::string;

/**
 * Calculates the prior probability (P(class = c)) portion of Bayes' Theorem
 * @param ifstream The input file stream
 * @param image Instance of Image containing all data for all images
 * @return A vector of all prior probabilities corresponding to each class of images
 */
vector<double> CalculatePriorProbabilities(std::ifstream &ifstream, naivebayes::Model& model) {
  //read through training labels
  vector<double> prior_probabilities;
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

/**
 * Calculates the probability of shading at a point (P(class = c | Fi,j=0)) portion of Bayes' Theorem
 * @param image Instance of Image containing all data for all images
 * @param raster_image_class Current class of the Raster image instance
 * @param pair Pair of coordinates indicating position
 * @return A double representing the probability of shading at a given point for a given class
 */
double FindProbabilityOfShadingAtPoint(const naivebayes::Model &model, size_t raster_image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = model.GetFrequencyMap()[raster_image_class][pair.first][pair.second];
  numerator = model.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*model.kLaplaceSmoothingFactor)+model.GetNumOfImagesInClass(raster_image_class);
  return numerator/denominator;
}

/**
 * Writes calculated probabilities to probabilities txt file
 * @param image Instance of Image containing all data for all images
 * @param prior_probabilities A vector of all prior probabilities corresponding to each class of images
 */
void WriteProbabilitiesToFile(std::ofstream &ofstream, naivebayes::Model &model, vector<double> prior_probabilities) {
  for(size_t cl = 0; cl <= 9; cl++) { //class number
    for(size_t x=0;x<28;x++) {
      for (size_t y = 0; y < 28; y++) {
        ofstream <<cl << " "<<x<< " "<<y<<" "<< prior_probabilities[cl] << " "
                 << FindProbabilityOfShadingAtPoint(model, cl, std::make_pair(x, y)) << std::endl;
      }
    }
  }
}

/**
 * Parses through list of all commands and calls the according functions
 * @param all_args Vector of strings containing all command line arguments passed in
 * @param image Current instance of Image class
 */
void RunCommandLineFunctions(const vector<string> &all_args, naivebayes::Model &model) {
  if(all_args.empty()) {
    throw std::invalid_argument("Empty argument vector!");
  }
  //input and output streams
  std::ifstream ifstream;
  std::ofstream ofstream;

  //train TrainingLabelsName TrainingImagesName save FileToWriteTo
  if(all_args[0] == "train") {
    if(all_args.size() == 5) {
      ifstream.open(all_args[1]); // mnistdatatraining/traininglabels
      vector<double> prior_probabilities = CalculatePriorProbabilities(ifstream, model);

      //read through training images
      ifstream.clear();
      ifstream.open(all_args[2]); // mnistdatatraining/trainingimages
      while(ifstream.good()) {
        ifstream>> model;
      }

      if (all_args[3] == "save") {
        //saves model to file
        ofstream.open(all_args[4]); // data/probabilities
        WriteProbabilitiesToFile(ofstream, model, prior_probabilities);
      }

    } else std::cout<<"Invalid amount of command line args. Please try again."<<std::endl;
  } else if(all_args[0] == "load") {  //load FileToLoadUp
    if(all_args.size() == 2) {
      ifstream.clear();
      ifstream.open(all_args[1]);

      while(ifstream.good()) {
        string line;
        getline(ifstream, line);
        std::cout<<line<<std::endl;
      }

    } else std::cout<<"Invalid amount of command line args. Please try again."<<std::endl;
  } else {
    std::cout<<"Invalid command line arg. Please try again."<<std::endl;
  }
}

//https://stackoverflow.com/questions/15344714/convert-command-line-argument-to-string
/**
 * Takes in main's parameters, and processes them to run the correct functions
 * @param argc Argument count
 * @param argv Argument vector
 */
void ProcessCommandLineArgs(int argc, char *argv[]) {
  //vector of strings consisting of all args passed in (not counting file name)
  vector<string> all_args;
  naivebayes::Model model;
  if (argc > 1) {
    all_args.assign(argv + 1, argv + argc);
  }
  RunCommandLineFunctions(all_args, model);
}

int main(int argc, char* argv[]) {
  ProcessCommandLineArgs(argc, argv);
  return 0;
}
