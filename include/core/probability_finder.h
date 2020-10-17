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
  /**
   * Calculates the prior probability (P(class = c)) portion of Bayes' Theorem
   * @param ifstream The input file stream
   * @param model The current model
   * @return A vector of all prior probabilities corresponding to each class of images
   */
  vector<double> CalculatePriorProbabilities(const string& filename, naivebayes::Model &model);

  /**
   * Calculates the probability of shading at a point (P(class = c | Fi,j=0)) portion of Bayes' Theorem
   * @param model The current model instance
   * @param image_class Current class of the image instance
   * @param pair Pair of coordinates indicating position
   * @return A double representing the probability of shading at a given point for a given class
   */
  double FindProbabilityOfShadingAtPoint(const naivebayes::Model &model, size_t image_class, std::pair<size_t, size_t> pair);

};

}

#endif  // NAIVE_BAYES_PROBABILITY_FINDER_H
