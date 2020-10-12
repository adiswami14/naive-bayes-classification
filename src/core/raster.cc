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
}
