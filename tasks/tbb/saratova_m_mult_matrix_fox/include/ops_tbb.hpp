// Copyright 2024 Saratova Marina
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace saratova_tbb {

void ScaledIdentityMatrix(double* matrix, int n, double k = 1.0);
void IdentityMatrix(double* matrix, int n, double k = 1.0);
void GenerateRandomValue(double* matrix, int sz);

class SaratovaTaskTbb : public ppc::core::Task {
 public:
  explicit SaratovaTaskTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  double *A{}, *B{}, *C{};
  size_t n{};
};
}  // namespace saratova_tbb