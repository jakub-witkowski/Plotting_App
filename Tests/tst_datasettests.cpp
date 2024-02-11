#include "../LSR-Plot-GUI/tdata.h"
#include "../LSR-Plot-GUI/tsegment.h"
#include "../LSR-Plot-GUI/tpolynomial.h"
// #include "../LSR-Plot-GUI/tplot.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

TData* dataset = new TData("1050.csv");

TEST(Class_TData_Tests, Load_Input_Raw_Data)
{
    dataset->load_input();
    ASSERT_TRUE(dataset->get_raw_data_size() > 0);
}

TEST(Class_TData_Tests, Load_Input_Ages)
{
    dataset->load_input();
    ASSERT_TRUE(dataset->get_ages_vector_size() > 0);
}

TEST(Class_TData_Tests, Load_Input_Depths)
{
    dataset->load_input();
    ASSERT_TRUE(dataset->get_depths_vector_size() > 0);
}

TEST(Class_TData_Tests, Compare_Ages_And_Depth_Vectors_Length)
{
    dataset->load_input();
    ASSERT_EQ(dataset->get_depths_vector_size(), dataset->get_ages_vector_size()) << "Depths and Ages vectors are of unequal length.";
}

TEST(Class_TData_Tests, Test_Segment_Index_Vector)
{
    dataset->load_input();
    dataset->find_hiatus();
    ASSERT_TRUE(dataset->get_segment_indexes_size() > 0);
}

TEST(Class_TSegment_Tests, Load_Segment_Depths)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    ASSERT_TRUE(segment->get_depths_vector_size() > 0);
    delete segment;
}

TEST(Class_TSegment_Tests, Load_Segment_Ages)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_ages_to_segment();
    ASSERT_TRUE(segment->get_ages_vector_size() > 0);
    delete segment;
}

TEST(Class_TSegment_Tests, Test_Segment_g1_ptr)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->set_g1_ptr();
    ASSERT_TRUE(segment->get_g1_ptr() != nullptr);
    delete segment;
}

TEST(Class_TSegment_Tests, Test_Segment_g3_ptr)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    segment->set_g3_ptr();
    ASSERT_TRUE(segment->get_g3_ptr() != nullptr);
    delete segment;
}

TEST(Class_TSegment_Tests, Test_LSR_Plot_Values_Vector)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_TRUE(segment->get_lsr_plot_values_vector_size() > 0);
    delete segment;
}

TEST(Class_TSegment_Tests, Test_LSR_Ages_Values_Vector)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_TRUE(segment->get_lsr_plot_ages_vector_size() > 0);
    delete segment;
}

TEST(Class_TSegment_Tests, Compare_LSR_Ages_And_LSR_Values_Vectors_Length)
{
    dataset->load_input();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_EQ(segment->get_lsr_plot_ages_vector_size(), segment->get_lsr_plot_values_vector_size()) << "Depths and Ages vectors are of unequal length.";
    delete segment;
}

/*TEST(Class_TPlot_Tests, Converting_Constructor_Test)
{
    dataset->load_input();
    TSegment segment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment.copy_depths_to_segment();
    segment.copy_ages_to_segment();
    segment.compute_lsr_values();

}*/

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    delete dataset;
}
