#include <glog/logging.h>
#include <gtest/gtest.h>

class LogEnvironment : public ::testing::Environment {
 public:
  void SetUp() override { google::InitGoogleLogging("infer_test"); }
  void TearDown() override { google::ShutdownGoogleLogging(); }
};

TEST(HelloTest, GtestWorks) { EXPECT_EQ(1 + 1, 2); }

TEST(HelloTest, GlogWorks) {
  LOG(INFO) << "glog is working";
  SUCCEED();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new LogEnvironment());
  return RUN_ALL_TESTS();
}
