//
// Created by Adithya Swaminathan on 10/17/20.
//

#include <core/probability_finder.h>
#include <fstream>
#include <cmath>

namespace naivebayes {

ProbabilityFinder::ProbabilityFinder() {
}

ProbabilityFinder::ProbabilityFinder(naivebayes::Model &model) {
  SetModel(model);
}

vector<double> ProbabilityFinder::CalculatePriorProbabilities() {
  //read through training labels
  vector<double> prior_probabilities;

  vector<int> training_vec = model_.GetTrainingLabelVec();
  for(size_t num = 0; num <= 9; num++) {
    //the number of times a certain number shows up in training labels over total number of training labels
    double probability_of_num =
        (model_.kLaplaceSmoothingFactor+ static_cast<double>(std::count(training_vec.begin(), training_vec.end(), num)))
        /((10*model_.kLaplaceSmoothingFactor)+training_vec.size());

    prior_probabilities.push_back(probability_of_num);
  }
  return prior_probabilities;
}

double ProbabilityFinder::FindProbabilityOfShadingAtPoint(size_t image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = model_.GetFrequencyMap()[image_class][pair.first][pair.second];
  numerator = model_.kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*model_.kLaplaceSmoothingFactor)+model_.GetNumOfImagesInClass(image_class);
  return numerator/denominator;
}

size_t ProbabilityFinder::ClassifyImage(naivebayes::Image &image) {
  double max = -5000.0;
  size_t best_class = 0;
  for(size_t cl = 0; cl <= 9; cl++) {
    double likelihood_score = 0.0;
    likelihood_score+=log(CalculatePriorProbabilities().at(cl));
    for (size_t r = 0; r < image.GetImageVector().size(); r++) {
      for (size_t c = 0; c < image.GetImageVector()[r].size(); c++) {
        if(isspace(image.GetImageVector()[r][c])) {
          likelihood_score += log(1 - FindProbabilityOfShadingAtPoint(cl, std::make_pair(r, c)));
        }
        else likelihood_score+=log(FindProbabilityOfShadingAtPoint(cl, std::make_pair(r, c)));
      }
    }
    if(likelihood_score>max) {
      best_class = cl;
      max = likelihood_score;
    }
  }
  return best_class;
}

void ProbabilityFinder::SetModel(naivebayes::Model &model) {
  model_ = model;
}

}