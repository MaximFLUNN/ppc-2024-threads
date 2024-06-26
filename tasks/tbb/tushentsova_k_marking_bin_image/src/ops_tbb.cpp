// Copyright 2024 Tushentsova Karina
#include "tbb/tushentsova_k_marking_bin_image/include/ops_tbb.hpp"

#include <oneapi/tbb.h>

using namespace std::chrono_literals;

bool markingImageTBB::run() {
  internal_order_test();
  markingImage();
  return true;
}
bool markingImageTBB::validation() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  return (height * width == taskData->inputs_count[1] && taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool markingImageTBB::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<uint32_t *>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t *>(taskData->inputs[0])[1];
  sourse.resize(height);
  destination.resize(height);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j)
      sourse[i].push_back(reinterpret_cast<uint8_t *>(taskData->inputs[1])[i * width + j]);
    destination[i].resize(width, 0);
  }
  return true;
}

bool markingImageTBB::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j)
      reinterpret_cast<uint8_t *>(taskData->outputs[0])[i * width + j] = destination[i][j];
  return true;
}

void markingImageTBB::markingImage() {
  uint32_t curLabel = 0;
  std::list<uint32_t> vec;
  std::vector<std::vector<uint32_t *>> arr;
  arr.resize(height);

  tbb::parallel_for(tbb::blocked_range<size_t>(0, height), [&](const tbb::blocked_range<size_t> &r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      arr[i].resize(width, nullptr);
    }
  });

  for (size_t i = 0; i < width; ++i) {
    if (sourse[0][i] == 0) {
      if (i == 0 || arr[0][i - 1] == nullptr) {
        vec.push_back(++curLabel);
        arr[0][i] = &vec.back();
      } else
        arr[0][i] = arr[0][i - 1];
    }
  }

  for (size_t i = 1; i < height; ++i) {
    if (sourse[i][0] == 0) {
      if (arr[i - 1][0] == nullptr) {
        vec.push_back(++curLabel);
        arr[i][0] = &vec.back();
      } else
        arr[i][0] = arr[i - 1][0];
    }

    for (size_t j = 1; j < width; ++j) {
      if (sourse[i][j] == 0) {
        if (arr[i - 1][j] == arr[i][j - 1]) {
          if (arr[i - 1][j] == nullptr) {
            vec.push_back(++curLabel);
            arr[i][j] = &vec.back();
          } else
            arr[i][j] = arr[i][j - 1];
        } else {
          if (arr[i - 1][j] == nullptr)
            arr[i][j] = arr[i][j - 1];
          else if (arr[i][j - 1] == nullptr)
            arr[i][j] = arr[i - 1][j];
          else {
            *(arr[i - 1][j]) = *(arr[i][j - 1]);
            arr[i][j] = arr[i][j - 1];
          }
        }
      }
    }
  }

  size_t count = 0;
  size_t cur = 0;
  vec.sort();
  for (auto &label : vec) {
    if (cur != label) {
      cur = label;
      count++;
    }
    label = count;
  }

  tbb::parallel_for(tbb::blocked_range2d<size_t>(0, height, 0, width), [&](const tbb::blocked_range2d<size_t> &r) {
    for (size_t i = r.rows().begin(); i != r.rows().end(); ++i)
      for (size_t j = r.cols().begin(); j != r.cols().end(); ++j)
        if (arr[i][j] != nullptr) destination[i][j] = *(arr[i][j]);
  });
}