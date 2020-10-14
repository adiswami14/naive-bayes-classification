#include <core/image.h>
#include <iostream>
#include <core/raster.h>

namespace naivebayes {

using std::count;
using std::string;

size_t raster_vector_key; //tracks current key of image corresponding to training label
size_t curr_row; //tracks current row of the image to be scanned

Image::Image() {
  //default 28x28 grid
  InitializeFrequencyMap(28);
  raster_vector_key=0;
  curr_row=0;
}

Image::Image(size_t frequency_map_size) {
  InitializeFrequencyMap(frequency_map_size);
  raster_vector_key=0;
  curr_row=0;
}

std::string Image::GetBestClass() const {
  return "CS 126";
}

std::istream &operator >>(std::istream &istream, Image& image) {
  naivebayes::Raster current_raster;
  string s;
  getline(istream, s);

  if(s.length() <= 2) {
    try {
      int training_val = std::stoi(s, nullptr, 10);
      image.training_label_vec_.push_back(training_val);
    } catch(std::invalid_argument) {
      std::cout<<"training labels read!"<<std::endl;
    }
  } else {
    vector<char> char_vec;
    size_t image_class = image.training_label_vec_.at(raster_vector_key);

    //get all characters from a line
    for(char ch: s) {
      char_vec.push_back(ch);
    }

    image.UpdateFrequencyMap(s, image_class);
    image.AddCurrentRasterToList(char_vec, current_raster, image_class);
  }
  return istream;
}

void Image::SetTrainingLabelVec(const vector<int> &training_label_vec) {
  if(training_label_vec.empty()) {
    throw std::invalid_argument("Empty training label vector passed in");
  }
  training_label_vec_ = training_label_vec;
}

vector<int> Image::GetTrainingLabelVec() const{
  return training_label_vec_;
}

size_t Image::GetNumOfImagesInClass(size_t class_num) const {
  return count(training_label_vec_.begin(), training_label_vec_.end(), class_num);
}

void Image::SetFrequencyMap(const map<size_t, vector<vector<size_t>>> &frequency_map) {
  if(frequency_map.empty()) {
    throw std::invalid_argument("Empty frequency map passed in");
  }
  frequency_map_ = frequency_map;
}

map<size_t, vector<vector<size_t>>> Image::GetFrequencyMap() const {
  return frequency_map_;
}

void Image::SetRasterList(const vector<naivebayes::Raster> &raster_list) {
  if(raster_list.empty()) {
    throw std::invalid_argument("Empty frequency map passed in");
  }
  raster_list_ = raster_list;
}

vector<naivebayes::Raster> Image::GetRasterList() const {
  return raster_list_;
}

void Image::InitializeFrequencyMap(size_t frequency_map_size) {
  vector<vector<size_t>> vec_big;
  vector<size_t> vec_small;

  for(size_t cl = 0; cl <= 9; cl++) {
    for (size_t x = 0; x < frequency_map_size; x++) {
      for (size_t y = 0; y < frequency_map_size; y++) {
        vec_small = vector<size_t>(frequency_map_size, 0);
      }
      vec_big.push_back(vec_small);
    }
    frequency_map_[cl] = vec_big;
    vec_big.clear();
  }

}

void Image::UpdateFrequencyMap(const string& s, size_t image_class) {
  for(size_t pos =0; pos<s.length();pos++) {
    char ch = s.at(pos);
    if (!isspace(ch)) {
      frequency_map_[image_class][curr_row][pos]++;
    }
  }
}

void Image::AddCurrentRasterToList(const vector<char> &char_vec, naivebayes::Raster &current_raster, size_t image_class) {
  size_t line_size = char_vec.size();
  if(training_image_vec_.size()==line_size-1) {
    training_image_vec_.push_back(char_vec);
    current_raster.SetRasterVector(training_image_vec_);
    current_raster.SetRasterImageClass(image_class);
    training_image_vec_.clear();
    raster_vector_key++;
    raster_list_.push_back(current_raster);
    curr_row = 0;
  } else {
    training_image_vec_.push_back(char_vec);
    curr_row++;
  }
}
}// namespace naivebayes