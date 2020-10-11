#include <string>
#include <vector>

namespace naivebayes {

using std::vector;

class Image {
 private:
  vector<int> training_label_vec_;
 public:
  const size_t kLaplaceSmoothingFactor = 1;

  std::string GetBestClass() const;

  /**
   * Overloads the >> operator for a given image
   * @param istream The input stream given
   * @param image The instance of this Image class
   * @return The input stream after operator overload
   */
  friend std::istream &operator >>(std::istream &istream, Image& image);

  /**
   * Returns the training_label_vec variable
   * @return Private variable training_label_vec
   */
  vector<int> GetTrainingLabelVec() const;

  /**
   * Identifies how many training images are in a given class
   * @param class_num The number of the class
   * @return The number of training images in a certain class
   */
  size_t GetNumOfImagesInClass(size_t class_num) const;
};
}  // namespace naivebayes

/*
TODO: rename this file. You'll also need to modify CMakeLists.txt.

You can (and should) create more classes and files in include/core (header
 files) and src/core (source files); this project is too big to only have a
 single class.

Make sure to add any files that you create to CMakeLists.txt.

TODO Delete this comment before submitting your code.
*/
