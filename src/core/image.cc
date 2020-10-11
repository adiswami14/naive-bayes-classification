#include <core/image.h>
#include <iostream>

namespace naivebayes {

using std::count;
using std::string;

std::string Image::GetBestClass() const {
  return "CS 126";
}

std::istream &operator >>(std::istream &istream, Image& image) {
  /*int training_val;
  istream >> training_val;
  image.training_label_vec_.push_back(training_val);*/
  string s;
  getline(istream, s);
  if(s.length() <= 3) {
    try {
      int training_val = std::stoi(s, nullptr, 10);
      image.training_label_vec_.push_back(training_val);
    } catch(std::invalid_argument) {
    }
  } else {
    vector<char> char_vec;
    for(char ch : s) {
      char_vec.push_back(ch);
    }
    image.training_image_vec_.push_back(char_vec);
  }
  return istream;
}

vector<int> Image::GetTrainingLabelVec() const{
  return training_label_vec_;
}

vector<vector<char>> Image::GetTrainingImageVec() const{
  return training_image_vec_;
}

size_t Image::GetNumOfImagesInClass(size_t class_num) const {
  return count(training_label_vec_.begin(), training_label_vec_.end(), class_num);
}
}// namespace naivebayes