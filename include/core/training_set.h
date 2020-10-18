//
// Created by Adithya Swaminathan on 10/18/20.
//

#ifndef NAIVE_BAYES_TRAINING_SET_H
#define NAIVE_BAYES_TRAINING_SET_H

#include "probability_finder.h"
namespace naivebayes {

class TrainingSet {
 public:
  TrainingSet();

  TrainingSet(naivebayes::ProbabilityFinder pf);

  /**
   * Overloads the >> operator for a given image
   * @param istream The input stream given
   * @param image The instance of this Image class
   * @return The input stream after operator overload
   */
  friend std::istream &operator >>(std::istream &istream, TrainingSet& trainingSet);

  void SetProbabilityFinder(naivebayes::ProbabilityFinder &pf);

  vector<naivebayes::Image> GetTestingImageList() const;

  vector<int> GetTestingLabelList() const;

  void ReadTestingLabel(const string& filename);
 private:
  void AddImageToTrainingImageList(vector<char> char_vec, naivebayes::Image &current_image);

  naivebayes::ProbabilityFinder pf_;

  vector<naivebayes::Image> testing_image_list_;

  vector<vector<char>> current_image_vec_;

  vector<int> testing_label_list;

  size_t curr_row_;
};

}
#endif  // NAIVE_BAYES_TRAINING_SET_H
