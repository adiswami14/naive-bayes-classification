//
// Created by Adithya Swaminathan on 10/11/20.
//

#include "core/raster.h"

namespace naivebayes {

Raster::Raster() {
}

void Raster::SetRasterVector(const vector<vector<char>> &raster_vector) {
  raster_vector_ = raster_vector;
}

void Raster::SetRasterImageClass(size_t raster_image_class) {
  raster_image_class_ = raster_image_class;
}

void Raster::EvaluateShadingAtPoint(size_t i, size_t j) {
  if(isspace(raster_vector_[i][j])) {
    shade_map_[std::make_pair(i, j)] = 0;
  } else shade_map_[std::make_pair(i, j)] = 1;
}

void Raster::CheckOverallShading() {
  for(size_t x = 0; x<raster_vector_.size();x++) {
    for(size_t y = 0;y < raster_vector_[x].size(); y++) {
      EvaluateShadingAtPoint(x, y);
    }
  }
}
}
