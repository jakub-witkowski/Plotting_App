#include "../LSR-Plot-GUI/tdata.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

TData dataset("1050.csv");

TEST(PlottingAppTests, Load_Input_Raw_Data)
{
    // TData dataset("1050.csv");
    dataset.load_input();
    ASSERT_TRUE(dataset.get_raw_data_size() > 0);
}

TEST(PlottingAppTests, Load_Input_Ages)
{
    // TData dataset("1050.csv");
    dataset.load_input();
    ASSERT_TRUE(dataset.get_ages_vector_size() > 0);
}

TEST(PlottingAppTests, Load_Input_Depths)
{
    // TData dataset("1050.csv");
    dataset.load_input();
    ASSERT_TRUE(dataset.get_depths_vector_size() > 0);
}

TEST(PlottingAppTests, Compare_Ages_And_Depth_Vectors_Length)
{
    // TData dataset("1050.csv");
    dataset.load_input();
    ASSERT_EQ(dataset.get_depths_vector_size(), dataset.get_ages_vector_size()) << "Depths and Ages vectors are of unequal length.";
}

TEST(PlottingAppTests, Test_Segment_Index_Vector)
{
    dataset.load_input();
    dataset.find_hiatus();
    ASSERT_TRUE(dataset.get_segment_indexes_size() > 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
