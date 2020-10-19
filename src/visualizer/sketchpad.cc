#include <visualizer/sketchpad.h>

namespace naivebayes {

namespace visualizer {

using glm::vec2;

Sketchpad::Sketchpad(const vec2& top_left_corner, size_t num_pixels_per_side,
                     double sketchpad_size, double brush_radius)
    : top_left_corner_(top_left_corner),
      num_pixels_per_side_(num_pixels_per_side),
      pixel_side_length_(sketchpad_size / num_pixels_per_side),
      brush_radius_(brush_radius) {
  vector<char> row_vec;
  for(size_t x = 0; x<num_pixels_per_side;x++) {
    for(size_t y = 0; y<num_pixels_per_side; y++) {
      row_vec.push_back(' ');
    }
    curr_image_vector_.push_back(row_vec);
    row_vec.clear();
  }
  image_.SetImageVector(curr_image_vector_);
}

void Sketchpad::Draw() const {
  for (size_t row = 0; row < num_pixels_per_side_; ++row) {
    for (size_t col = 0; col < num_pixels_per_side_; ++col) {
      // Currently, this will draw a quarter circle centered at the top-left
      // corner with a radius of 20

      // TODO: Replace the if-statement below with an if-statement that checks
      // if the pixel at (row, col) is currently shaded
      if (!image_.CheckSpace(row, col)) {
        ci::gl::color(ci::Color::gray(0.3f));
      } else {
        ci::gl::color(ci::Color("white"));
      }

      vec2 pixel_top_left = top_left_corner_ + vec2(col * pixel_side_length_,
                                                    row * pixel_side_length_);

      vec2 pixel_bottom_right =
          pixel_top_left + vec2(pixel_side_length_, pixel_side_length_);
      ci::Rectf pixel_bounding_box(pixel_top_left, pixel_bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);

      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);
    }
  }
}

void Sketchpad::HandleBrush(const vec2& brush_screen_coords) {
  vec2 brush_sketchpad_coords =
      (brush_screen_coords - top_left_corner_) / (float)pixel_side_length_;

  for (size_t row = 0; row < num_pixels_per_side_; ++row) {
    for (size_t col = 0; col < num_pixels_per_side_; ++col) {
      vec2 pixel_center = {col + 0.5, row + 0.5};

      if (glm::distance(brush_sketchpad_coords, pixel_center) <=
          brush_radius_) {
        // TODO: Add code to shade in the pixel at (row, col)
        curr_image_vector_[row][col] = '+';
        image_.SetImageVector(curr_image_vector_);
      }
    }
  }
}

void Sketchpad::Clear() {
  // TODO: implement this method
  curr_image_vector_.clear();
  vector<char> row_vec;
  for(size_t x = 0; x<num_pixels_per_side_;x++) {
    for(size_t y = 0; y<num_pixels_per_side_; y++) {
      row_vec.push_back(' ');
    }
    curr_image_vector_.push_back(row_vec);
    row_vec.clear();
  }
  image_.SetImageVector(curr_image_vector_);
}

Image Sketchpad::GetImage() {
  return image_;
}

}  // namespace visualizer

}  // namespace naivebayes
