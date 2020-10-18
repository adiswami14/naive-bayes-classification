//
// Created by Adithya Swaminathan on 10/18/20.
//
#include <core/training_set.h>

#include <fstream>
namespace naivebayes {

TrainingSet::TrainingSet() {
  curr_row_=0;
}

TrainingSet::TrainingSet(naivebayes::ProbabilityFinder pf) {
  curr_row_=0;
  SetProbabilityFinder(pf);
}

std::istream &operator >>(std::istream &istream, TrainingSet& trainingSet) {
  naivebayes::Image current_image;
  string s;
  getline(istream, s);
  if(s.size()!=0) {
    vector<char> char_vec;
    // get all characters from a line
    for (char ch : s) {
      char_vec.push_back(ch);
    }
    trainingSet.AddImageToTrainingImageList(char_vec, current_image);
  }
  return istream;
}

vector<naivebayes::Image> TrainingSet::GetTestingImageList() const {
  return testing_image_list_;
}

vector<int> TrainingSet::GetTestingLabelList() const {
  return testing_label_list;
}

void TrainingSet::ReadTestingLabel(const string &filename) {
  std::ifstream istream(filename);
  int training_val;
  while(istream.good()) {
    istream>>training_val;
    testing_label_list.push_back(training_val);
  }
}

void TrainingSet::SetProbabilityFinder(naivebayes::ProbabilityFinder &pf) {
  pf_ = pf;
}

void TrainingSet::AddImageToTrainingImageList(vector<char> char_vec, naivebayes::Image &current_image) {
  size_t line_size = char_vec.size();
  if(current_image_vec_.size()==line_size-1) {
    current_image_vec_.push_back(char_vec);
    current_image.SetImageVector(current_image_vec_);
    size_t best_class = pf_.ClassifyImage(current_image);
    current_image.SetBestClass(best_class);
    current_image_vec_.clear();
    testing_image_list_.push_back(current_image);
    curr_row_ = 0;
    //likelihood_ = 0;
  } else {
    current_image_vec_.push_back(char_vec);
    curr_row_++;
  }
}


}