#include <core/image.h>
#include <core/raster.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

using std::vector;

/**
 * Calculates the prior probability (P(class = c)) portion of Bayes' Theorem
 * @param ifstream The input file stream
 * @param image Instance of Image containing all data for all images
 * @return A vector of all prior probabilities corresponding to each class of images
 */
vector<double> CalculatePriorProbabilities(std::ifstream &ifstream, naivebayes::Image& image) {
  //read through training labels
  vector<double> prior_probabilities;
  ifstream.open("mnistdatatraining/traininglabels");
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

/**
 * Calculates the probability of shading at a point (P(class = c | Fi,j=0)) portion of Bayes' Theorem
 * @param image Instance of Image containing all data for all images
 * @param raster_image_class Current class of the Raster image instance
 * @param pair Pair of coordinates indicating position
 * @return A double representing the probability of shading at a given point for a given class
 */
double FindProbabilityOfShadingAtPoint(const naivebayes::Image &image, size_t raster_image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = image.GetFrequencyMap()[raster_image_class][pair.first][pair.second];
  numerator = image.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (10*image.kLaplaceSmoothingFactor)+image.GetNumOfImagesInClass(raster_image_class);
  return numerator/denominator;
}

/**
 * Writes calculated probabilities to probabilities txt file
 * @param image Instance of Image containing all data for all images
 * @param prior_probabilities A vector of all prior probabilities corresponding to each class of images
 */
void WriteProbabilitiesToFile(naivebayes::Image &image, vector<double> prior_probabilities) {
  std::ofstream ofstream;
  ofstream.open("probabilities");

  for(size_t cl = 0; cl <= 9; cl++) { //class number
    for(size_t x=0;x<28;x++) {
      for (size_t y = 0; y < 28; y++) {
        ofstream <<cl << " "<<x<< " "<<y<<" "<< prior_probabilities[cl] << " "
                 << FindProbabilityOfShadingAtPoint(image, cl, std::make_pair(x, y)) << std::endl;
      }
    }
  }

}

int main() {
  std::ifstream ifstream_labels;
  naivebayes::Image image;
  vector<double> prior_probabilities = CalculatePriorProbabilities(ifstream_labels, image);

  //read through training images
  std::ifstream ifstream_images;
  ifstream_images.open("mnistdatatraining/trainingimages");
  while(ifstream_images.good()) {
    ifstream_images>> image;
  }

  //saves model to file
  WriteProbabilitiesToFile(image, prior_probabilities);
  return 0;
}
