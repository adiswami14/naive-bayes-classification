#include <core/image.h>
#include <core/model.h>
#include <fstream>
#include <iostream>
#include <cmath>

namespace naivebayes {

using std::count;
using std::string;

Model::Model() {
  //default 28x28 grid
  InitializeFrequencyMap(28);
  image_vector_key_=0;
  curr_row_=0;
  num_in_class_ = vector<size_t>(10, 0);
}

Model::Model(size_t frequency_map_size) {
  InitializeFrequencyMap(frequency_map_size);
  image_vector_key_=0;
  curr_row_=0;
  num_in_class_ = vector<size_t>(10, 0);
}

Model::Model(std::map<size_t, vector<vector<double>>> map) {
  InitializeFrequencyMap(28);
  image_vector_key_=0;
  curr_row_=0;
  num_in_class_ = vector<size_t>(10, 0);
  feature_prob_map_ = map;
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

void Model::SetNumOfImagesInClass(){
  for(int x=0;x<=9;x++) {
    num_in_class_[x] = std::count(training_label_vec_.begin(), training_label_vec_.end(), x);
  }
}

vector<size_t> Model::GetNumOfImagesInClass() const {
  return num_in_class_;
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
  if(istream.eof()) {
    prior_probabilities_ = CalculatePriorProbabilities();
  }
}

vector<size_t> Model::GetBestClassList() const {
  return best_class_list;
}

vector<naivebayes::Image> Model::GetImageList() const {
  return image_list_;
}

vector<double> Model::CalculatePriorProbabilities() {
  for(size_t num = 0; num <= 9; num++) {
    //the number of times a certain number shows up in training labels over total number of training labels
    double probability_of_num =
        (kLaplaceSmoothingFactor+ static_cast<double>(std::count(training_label_vec_.begin(), training_label_vec_.end(), num)))
        /((10*kLaplaceSmoothingFactor)+training_label_vec_.size());

    prior_probabilities_.push_back(probability_of_num);
  }
  return prior_probabilities_;
}

size_t Model::ClassifyImage(naivebayes::Image &image) {
  double likelihood_score;
  size_t best_class = 0;
  double max = -5000.0;
  for(size_t cl = 0; cl <= 9; cl++) {
    likelihood_score=log(prior_probabilities_.at(cl));
    for (size_t r = 0; r < 28; r++) {
      for (size_t c = 0; c < 28; c++) {
        if(image.CheckSpace(r, c)) {
          likelihood_score +=
              log(1-feature_prob_map_[cl][r][c]);
        } else likelihood_score +=log(feature_prob_map_[cl][r][c]);
      }
    }
    if(likelihood_score>max) {
      best_class = cl;
      max = likelihood_score;
    }
  }

  return best_class;
}

double Model::FindProbabilityOfShadingAtPoint(size_t image_class, std::pair<size_t, size_t> pair) {
  double numerator;
  size_t num_shaded_at_point = frequency_map_[image_class][pair.first][pair.second];
  numerator = kLaplaceSmoothingFactor+(double)num_shaded_at_point;
  double denominator;
  denominator = (2*kLaplaceSmoothingFactor)+num_in_class_[image_class];
  return numerator/denominator;
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
    size_t best_class = ClassifyImage(current_image);
    best_class_list.push_back(best_class);
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