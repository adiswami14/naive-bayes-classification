#include <core/image.h>
#include <iostream>

namespace naivebayes {

using std::count;

std::string Image::GetBestClass() const {
  return "CS 126";
}

std::istream &operator >>(std::istream &istream, Image& image) {
  int training_val;
  istream >> training_val;
  image.training_label_vec_.push_back(training_val);
  return istream;
}

vector<int> Image::GetTrainingLabelVec() const{
  return training_label_vec_;
}

size_t Image::GetLaplaceSmoothingFactor() const {
  return kLaplaceSmoothingFactor;
}

size_t Image::GetNumOfImagesInClass(size_t class_num) const {
  return count(training_label_vec_.begin(), training_label_vec_.end(), class_num);
}
}// namespace naivebayes