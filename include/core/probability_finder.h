//
// Created by Adithya Swaminathan on 10/17/20.
//

#ifndef NAIVE_BAYES_PROBABILITY_FINDER_H
#define NAIVE_BAYES_PROBABILITY_FINDER_H
#include <core/model.h>

namespace naivebayes {

using std::vector;

class ProbabilityFinder {
 public:
  ProbabilityFinder();

  ProbabilityFinder(naivebayes::Model &model);
  /**
   * Calculates the prior probability (P(class = c)) portion of Bayes' Theorem
   * @param model The current model
   * @return A vector of all prior probabilities corresponding to each class of images
   */
  vector<double> CalculatePriorProbabilities();

  /**
   * Calculates the probability of shading at a point (P(class = c | Fi,j=0)) portion of Bayes' Theorem
   * @param model The current model instance
   * @param image_class Current class of the image instance
   * @param pair Pair of coordinates indicating position
   * @return A double representing the probability of shading at a given point for a given class
   */
  double FindProbabilityOfShadingAtPoint(size_t image_class, std::pair<size_t, size_t> pair);

  size_t ClassifyImage(naivebayes::Image &image);

  void SetModel(naivebayes::Model &model);
 private:
  naivebayes::Model model_;
};
} //namespace naivebayes

#endif  // NAIVE_BAYES_PROBABILITY_FINDER_H
