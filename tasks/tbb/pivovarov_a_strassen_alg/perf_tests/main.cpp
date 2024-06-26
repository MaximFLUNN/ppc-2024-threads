// Copyright 2024 Pivovarov Alexey
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/pivovarov_a_strassen_alg/include/ops_tbb.hpp"

TEST(tbb_pivovarov_a_strassen_alg_perf_test, test_pipeline_run) {
  int n = 64;

  // Create data
  std::vector<double> in_A = createRndMatrix(n);
  std::vector<double> in_B = createRndMatrix(n);
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = multiplyMatrix(in_A, in_B, n);

  // Create Task
  auto test = std::make_shared<TestTaskTBBParallelPivovarovStrassen>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(test);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(std::round(res[i] * std::pow(10, 3)) / std::pow(10, 3),
              std::round(out[i] * std::pow(10, 3)) / std::pow(10, 3));
  }
}

TEST(tbb_pivovarov_a_strassen_alg_perf_test, test_task_run) {
  int n = 64;

  // Create data
  std::vector<double> in_A = createRndMatrix(n);
  std::vector<double> in_B = createRndMatrix(n);
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_A.data()));
  taskDataSeq->inputs_count.emplace_back(in_A.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in_B.data()));
  taskDataSeq->inputs_count.emplace_back(in_B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = multiplyMatrix(in_A, in_B, n);

  // Create Task
  auto test = std::make_shared<TestTaskTBBParallelPivovarovStrassen>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(test);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  for (size_t i = 0; i < res.size(); ++i) {
    ASSERT_EQ(std::round(res[i] * std::pow(10, 3)) / std::pow(10, 3),
              std::round(out[i] * std::pow(10, 3)) / std::pow(10, 3));
  }
}
