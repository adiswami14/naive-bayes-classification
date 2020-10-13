//
// Created by Adithya Swaminathan on 10/11/20.
//

#include "core/raster.h"

namespace naivebayes {

Raster::Raster() {
}

void Raster::SetRasterVector(const vector<vector<char>> &raster_vector) {
  if(raster_vector.empty()) {
    throw std::invalid_argument("Raster vector given as input is empty");
  }
  raster_vector_ = raster_vector;
}

void Raster::SetRasterImageClass(size_t raster_image_class) {
  raster_image_class_ = raster_image_class;
}

vector<vector<char>> Raster::GetRasterVector() const {
  return raster_vector_;
}

size_t Raster::GetRasterImageClass() const {
  return raster_image_class_;
}
}
