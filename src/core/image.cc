//
// Created by Adithya Swaminathan on 10/11/20.
//

#include "core/image.h"

namespace naivebayes {

Image::Image(){
}

void Image::SetImageVector(const vector<vector<char>> &image_vector) {
  if(image_vector.empty()) {
    throw std::invalid_argument("Image vector given as input is empty");
  }
  image_vector_ = image_vector;
}

void Image::SetImageClass(size_t image_class) {
  if(image_class>9) {
    throw std::invalid_argument("Image class input is not a single-digit number");
  }
  image_class_ = image_class;
}

void Image::SetBestClass(size_t best_class) {
  if(best_class>9) {
    throw std::invalid_argument("Image class input is not a single-digit number");
  }
  best_class_ = best_class;
}

vector<vector<char>> Image::GetImageVector() const {
  return image_vector_;
}

size_t Image::GetImageClass() const {
  return image_class_;
}

size_t Image::GetBestClass() const {
  return best_class_;
}
}
