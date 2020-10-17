//
// Created by Adithya Swaminathan on 10/17/20.
//

#include <core/probability_finder.h>
#include <fstream>

namespace naivebayes {

vector<double> ProbabilityFinder::CalculatePriorProbabilities(const string& filename, naivebayes::Model &model) {
  //read through training labels
  vector<double> prior_probabilities;

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

double ProbabilityFinder::FindProbabilityOfShadingAtPoint(const naivebayes::Model &model, size_t image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = model.GetFrequencyMap()[image_class][pair.first][pair.second];
  numerator = model.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*model.kLaplaceSmoothingFactor)+model.GetNumOfImagesInClass(image_class);
  return numerator/denominator;
}

}