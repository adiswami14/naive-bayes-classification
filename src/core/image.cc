#include <core/image.h>
#include <iostream>
#include <core/raster.h>

namespace naivebayes {

using std::count;
using std::string;

size_t raster_vector_key = 0;

std::string Image::GetBestClass() const {
  return "CS 126";
}

std::istream &operator >>(std::istream &istream, Image& image) {
  naivebayes::Raster current_raster;
  string s;
  size_t line_size;
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
    line_size = char_vec.size();
    if(image.training_image_vec_.size()==line_size-1) {
        current_raster.SetRasterVector(image.training_image_vec_);
        current_raster.SetRasterImageClass(image.training_label_vec_.at(raster_vector_key));
        current_raster.CheckOverallShading();
        image.training_image_vec_.clear();
        std::cout<<raster_vector_key << "     " << image.training_label_vec_.at(raster_vector_key)<<std::endl;
        raster_vector_key++;
        image.raster_list_.push_back(current_raster);
    } else {
      image.training_image_vec_.push_back(char_vec);
    }
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