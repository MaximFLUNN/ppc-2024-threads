// Copyright 2024 Khodyrev Fedor
#include <gtest/gtest.h>
#include <omp.h>

#include <stack>
#include <vector>

#include "omp/khodyrev_f_convex_hull/include/ops_omp.hpp"

using namespace khodyrev_omp;

TEST(khodyrev_f_convex_hull_omp, Test_convex_hull_validation) {
  int height = 4;
  int width = 3;
  std::vector<int> in = {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0};
  std::vector<int> out_seq(height * width);
  std::vector<int> out_par(height * width);

  std::vector<int> true_result = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(height);
  taskDataSeq->inputs_count.emplace_back(width);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(height + 1);
  taskDataSeq->outputs_count.emplace_back(width + 1);
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(height);
  taskDataPar->inputs_count.emplace_back(width);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(height + 1);
  taskDataPar->outputs_count.emplace_back(width + 1);

  KhodyrevTaskSequential testTaskSequential(taskDataSeq);
  KhodyrevTaskOMP testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskSequential.validation(), false);
  ASSERT_EQ(testTaskParallel.validation(), false);
}

TEST(khodyrev_f_convex_hull_omp, Test_convex_hull_with_small_image) {
  int height_in = 4;
  int width_in = 3;
  int height_out = 4;
  int width_out = 3;
  std::vector<int> in = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::vector<int> out_seq(height_out * width_out, 0);
  std::vector<int> out_par(height_out * width_out, 0);

  std::vector<int> true_result = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(height_in);
  taskDataSeq->inputs_count.emplace_back(width_in);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(height_out);
  taskDataSeq->outputs_count.emplace_back(width_out);

  KhodyrevTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(height_in);
  taskDataPar->inputs_count.emplace_back(width_in);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(height_out);
  taskDataPar->outputs_count.emplace_back(width_out);

  KhodyrevTaskOMP testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_seq[i], true_result[i]);
  }
  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_par[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_omp, Test_convex_hull_with_medium_image) {
  int height_in = 1000;
  int width_in = 100;
  int height_out = 1000;
  int width_out = 100;
  std::vector<int> in(100000, 0);
  in[0] = 1;
  in[99999] = 1;
  std::vector<int> out_seq(height_out * width_out, 0);
  std::vector<int> out_par(height_out * width_out, 0);

  std::vector<int> true_result(100000, 0);
  true_result[0] = 1;
  true_result[99999] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(height_in);
  taskDataSeq->inputs_count.emplace_back(width_in);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(height_out);
  taskDataSeq->outputs_count.emplace_back(width_out);

  KhodyrevTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(height_in);
  taskDataPar->inputs_count.emplace_back(width_in);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(height_out);
  taskDataPar->outputs_count.emplace_back(width_out);

  KhodyrevTaskOMP testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_seq[i], true_result[i]);
  }
  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_par[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_omp, Test_convex_hull_with_big_image) {
  int height_in = 1000;
  int width_in = 1000;
  int height_out = 1000;
  int width_out = 1000;
  std::vector<int> in(1000000, 0);
  in[0] = 1;
  in[999999] = 1;
  std::vector<int> out_seq(height_out * width_out, 0);
  std::vector<int> out_par(height_out * width_out, 0);

  std::vector<int> true_result(1000000, 0);
  true_result[0] = 1;
  true_result[999999] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(height_in);
  taskDataSeq->inputs_count.emplace_back(width_in);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(height_out);
  taskDataSeq->outputs_count.emplace_back(width_out);

  KhodyrevTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(height_in);
  taskDataPar->inputs_count.emplace_back(width_in);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(height_out);
  taskDataPar->outputs_count.emplace_back(width_out);

  KhodyrevTaskOMP testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_seq[i], true_result[i]);
  }
  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_par[i], true_result[i]);
  }
}

TEST(khodyrev_f_convex_hull_omp, Test_convex_hull_with_very_big_image) {
  int height_in = 3000;
  int width_in = 1000;
  int height_out = 3000;
  int width_out = 1000;
  std::vector<int> in(3000000, 0);
  in[0] = 1;
  in[1] = 1;
  std::vector<int> out_seq(height_out * width_out, 0);
  std::vector<int> out_par(height_out * width_out, 0);

  std::vector<int> true_result(3000000, 0);
  true_result[0] = 1;
  true_result[1] = 1;

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(height_in);
  taskDataSeq->inputs_count.emplace_back(width_in);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_seq.data()));
  taskDataSeq->outputs_count.emplace_back(height_out);
  taskDataSeq->outputs_count.emplace_back(width_out);

  KhodyrevTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
  ASSERT_EQ(testTaskSequential.run(), true);
  ASSERT_EQ(testTaskSequential.post_processing(), true);

  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataPar->inputs_count.emplace_back(height_in);
  taskDataPar->inputs_count.emplace_back(width_in);
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(out_par.data()));
  taskDataPar->outputs_count.emplace_back(height_out);
  taskDataPar->outputs_count.emplace_back(width_out);

  KhodyrevTaskOMP testTaskParallel(taskDataPar);
  ASSERT_EQ(testTaskParallel.validation(), true);
  ASSERT_EQ(testTaskParallel.pre_processing(), true);
  ASSERT_EQ(testTaskParallel.run(), true);
  ASSERT_EQ(testTaskParallel.post_processing(), true);

  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_seq[i], true_result[i]);
  }
  for (std::vector<int>::size_type i = 0; i < in.size(); i++) {
    EXPECT_EQ(out_par[i], true_result[i]);
  }
}
