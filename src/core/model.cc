#include <core/image.h>
#include <core/model.h>
#include <fstream>
#include <iostream>

namespace naivebayes {

using std::count;
using std::string;

Model::Model() {
  //default 28x28 grid
  InitializeFrequencyMap(28);
  image_vector_key_=0;
  curr_row_=0;
}

Model::Model(size_t frequency_map_size) {
  InitializeFrequencyMap(frequency_map_size);
  image_vector_key_=0;
  curr_row_=0;
}

std::string Model::GetBestClass() const {
  return "CS 126";
}

std::istream &operator >>(std::istream &istream, Model& model) {
  naivebayes::Image current_image;
  string s;
  getline(istream, s);
  if(s.size()!=0) {
    vector<char> char_vec;
    size_t image_class = model.training_label_vec_.at(model.image_vector_key_);

    // get all characters from a line
    for (char ch : s) {
      char_vec.push_back(ch);
    }

    model.UpdateFrequencyMap(s, image_class);
    model.AddCurrentImageToList(char_vec, current_image, image_class);
  }

  return istream;
}

void Model::SetTrainingLabelVec(const vector<int> &training_label_vec) {
  if(training_label_vec.empty()) {
    throw std::invalid_argument("Empty training label vector passed in");
  }
  training_label_vec_ = training_label_vec;
}

vector<int> Model::GetTrainingLabelVec() const{
  return training_label_vec_;
}

size_t Model::GetNumOfImagesInClass(size_t class_num) const {
  return count(training_label_vec_.begin(), training_label_vec_.end(), class_num);
}

void Model::SetFrequencyMap(const map<size_t, vector<vector<size_t>>> &frequency_map) {
  if(frequency_map.empty()) {
    throw std::invalid_argument("Empty frequency map passed in");
  }
  frequency_map_ = frequency_map;
}

map<size_t, vector<vector<size_t>>> Model::GetFrequencyMap() const {
  return frequency_map_;
}

void Model::SetImageList(const vector<naivebayes::Image> &image_list) {
  if(image_list.empty()) {
    throw std::invalid_argument("Empty frequency map passed in");
  }
  image_list_ = image_list;
}

void Model::ReadLabels(const string &filename) {
  std::ifstream istream(filename);
  int training_val;
  while(istream.good()) {
    istream>>training_val;
    training_label_vec_.push_back(training_val);
  }
}

vector<naivebayes::Image> Model::GetImageList() const {
  return image_list_;
}

void Model::InitializeFrequencyMap(size_t frequency_map_size) {
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

void Model::UpdateFrequencyMap(const string& s, size_t image_class) {
  for(size_t pos =0; pos<s.length();pos++) {
    char ch = s.at(pos);
    if (!isspace(ch)) {
      frequency_map_[image_class][curr_row_][pos]++;
    }
  }
}

void Model::AddCurrentImageToList(const vector<char> &char_vec, naivebayes::Image &current_image, size_t image_class) {
  size_t line_size = char_vec.size();
  if(training_image_vec_.size()==line_size-1) {
    training_image_vec_.push_back(char_vec);
    current_image.SetImageVector(training_image_vec_);
    current_image.SetImageClass(image_class);
    training_image_vec_.clear();
    image_vector_key_++;
    image_list_.push_back(current_image);
    curr_row_ = 0;
  } else {
    training_image_vec_.push_back(char_vec);
    curr_row_++;
  }
}
}// namespace naivebayes