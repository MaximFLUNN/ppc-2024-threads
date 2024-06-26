// Copyright 2024 Vyunov Danila
#include <algorithm>
#include <vector>

#include "core/task/include/task.hpp"

class HoareSortSimpleSeq : public ppc::core::Task {
 public:
  explicit HoareSortSimpleSeq(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> array{};
};

void HoarSort(std::vector<int>* arr, int l, int r);
std::vector<int> Getvec(int n);