// Copyright 2024 Ryabkov Vladislav

#pragma once

#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <vector>

#include "core/task/include/task.hpp"
#include "tbb/blocked_range.h"
#include "tbb/mutex.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_invoke.h"
#include "tbb/task_group.h"

namespace ryabkov_batcher {
class SeqBatcher : public ppc::core::Task {
 public:
  explicit SeqBatcher(std::shared_ptr<ppc::core::TaskData> taskData) : Task(std::move(taskData)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> inv, result, a1, a2;
};

inline std::vector<int> GetRandomVector(int size) {
  std::vector<int> vect(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 1000);

  for (int i = 0; i < size; ++i) {
    vect[i] = dis(gen);
  }

  return vect;
}
}  // namespace ryabkov_batcher
