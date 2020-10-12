#include <core/image.h>
#include <iostream>
#include <core/raster.h>

namespace naivebayes {

using std::count;
using std::string;

size_t raster_vector_key = 0;
size_t rows =0;

Image::Image() {
  InitializeFrequencyMap();
}

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
    size_t image_class = image.training_label_vec_.at(raster_vector_key);
    for(char ch: s) {
      char_vec.push_back(ch);
    }
    line_size = char_vec.size();
    for(size_t x = 0; x< s.length();x++) {
      if (!isspace(s.at(x))) {
        image.frequency_map_[image_class][rows][x]++;
      }
    }
    if(image.training_image_vec_.size()==line_size-1) {
        current_raster.SetRasterVector(image.training_image_vec_);
        current_raster.SetRasterImageClass(image_class);
        image.training_image_vec_.clear();
        raster_vector_key++;
        image.raster_list_.push_back(current_raster);
        rows = 0;
    } else {
      image.training_image_vec_.push_back(char_vec);
      rows++;
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

vector<naivebayes::Raster> Image::GetRasterList() const {
  return raster_list_;
}

map<size_t, vector<vector<size_t>>> Image::GetFrequencyMap() const {
  return frequency_map_;
}

void Image::InitializeFrequencyMap() {
  vector<vector<size_t>> vec_big;
  vector<size_t> vec_small;
  for(size_t cl = 0; cl <= 9; cl++) {
    for (size_t x = 0; x < 28; x++) {
      for (size_t y = 0; y < 28; y++) {
        vec_small = vector<size_t>(28, 0);
      }
      vec_big.push_back(vec_small);
    }
    frequency_map_[cl] = vec_big;
    vec_big.clear();
  }
}
}// namespace naivebayes