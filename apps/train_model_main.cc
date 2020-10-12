#include <core/image.h>
#include <core/raster.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

// TODO: You may want to change main's signature to take in argc and argv
using std::vector;

vector<double> CalculatePriorProbabilities(std::ifstream &ifstream, naivebayes::Image& image) {
  vector<double> prior_probabilities;
  ifstream.open("mnistdatatraining/traininglabels");
  while(ifstream.good()) {
    ifstream>> image;
  }

  vector<int> training_vec = image.GetTrainingLabelVec();
  for(size_t num = 0; num <= 9; num++) {
    //the number of times a certain number shows up in training labels over total number of training labels
    double probability_of_num =
        static_cast<double>(std::count(training_vec.begin(), training_vec.end(), num))/training_vec.size();

    //std::cout<<num<<": "<<probability_of_num<<std::endl;
    prior_probabilities.push_back(probability_of_num);
  }
  return prior_probabilities;
}

int main() {
  // TODO: Replace this with code that reads the training data, trains a model,
  // and saves the trained model to a file.

  std::ifstream ifstream_labels;
  naivebayes::Image image;
  vector<double> prior_probabilities = CalculatePriorProbabilities(ifstream_labels, image);
  std::ifstream ifstream_images;
  ifstream_images.open("mnistdatatraining/trainingimages");
  while(ifstream_images.good()) {
    ifstream_images>> image;
  }

  return 0;
}
