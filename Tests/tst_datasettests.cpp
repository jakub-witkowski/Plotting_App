#include "../LSR-Plot-GUI/tdata.h"
#include "../LSR-Plot-GUI/tsegment.h"
#include "../LSR-Plot-GUI/tpolynomial.h"
#include "../LSR-Plot-GUI/tplot.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <vector>

TData* dataset{nullptr};

TEST(Class_TData_Tests, Load_Input_Raw_Data)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    ASSERT_TRUE(dataset->get_raw_data_size() > 0);
    delete dataset;
}

TEST(Class_TData_Tests, Load_Input_Ages)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    ASSERT_TRUE(dataset->get_ages_vector_size() > 0);
    delete dataset;
}

TEST(Class_TData_Tests, Load_Input_Depths)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    ASSERT_TRUE(dataset->get_depths_vector_size() > 0);
    delete dataset;
}

TEST(Class_TData_Tests, Compare_Ages_And_Depth_Vectors_Length)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    ASSERT_EQ(dataset->get_depths_vector_size(), dataset->get_ages_vector_size()) << "Depths and Ages vectors are of unequal length.";
    delete dataset;
}

TEST(Class_TData_Tests, Test_Segment_Index_Vector)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    ASSERT_TRUE(dataset->get_segment_indexes_size() > 0);
    delete dataset;
}

TEST(Class_TSegment_Tests, Load_Segment_Depths)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    ASSERT_TRUE(segment->get_depths_vector_size() > 0);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Load_Segment_Ages)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_ages_to_segment();
    ASSERT_TRUE(segment->get_ages_vector_size() > 0);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Test_Segment_g2_ptr)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->set_g2_ptr_pretty();
    ASSERT_TRUE(segment->get_g2_ptr_pretty() != nullptr);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Test_Segment_Data_Point_Number)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    EXPECT_EQ(2*(segment->get_ages_vector_size() - 1), segment->get_lsr_plot_ages_vector_size());
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Test_LSR_Plot_Values_Vector)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_TRUE(segment->get_lsr_plot_values_vector_size() > 0);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Test_LSR_Ages_Values_Vector)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_TRUE(segment->get_lsr_plot_ages_vector_size() > 0);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Compare_Number_Of_Tiepoints_Between_Age_Vs_Depth_And_LSR_Plots_Test)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_TRUE(segment->get_lsr_plot_ages_vector_size() == (segment->get_ages_vector_size() -1) * 2);
    delete segment;
    delete dataset;
}

TEST(Class_TSegment_Tests, Compare_LSR_Ages_And_LSR_Values_Vectors_Length)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->compute_lsr_values();
    ASSERT_EQ(segment->get_lsr_plot_ages_vector_size(), segment->get_lsr_plot_values_vector_size()) << "Depths and Ages vectors are of unequal length.";
    delete segment;
    delete dataset;
}

TEST(Class_TPolynomial_Tests, Test_Fit_Vector_Size)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->set_g1_ptr();
    for (size_t i = 0; i < 10; i++)
    {
        segment->add_to_fit_vector(i);
    }
    ASSERT_TRUE(segment->get_fit_vector_size() == 10);
    delete segment;
    delete dataset;
}

TEST(Class_TPolynomial_Tests, Test_TPolynomial_String_Member)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->set_g1_ptr();
    for (size_t i = 0; i < 10; i++)
    {
        segment->add_to_fit_vector(i);
    }
    EXPECT_STREQ("pol 4", segment->get_fit_phrase(4).c_str());
    delete segment;
    delete dataset;
}

TEST(Class_TPolynomial_Tests, Test_F_Pointer)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment* segment = new TSegment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment->copy_depths_to_segment();
    segment->copy_ages_to_segment();
    segment->set_g1_ptr();
    for (size_t i = 0; i < 10; i++)
    {
        segment->add_to_fit_vector(i);
    }
    EXPECT_FALSE(segment->get_f_ptr(9) == nullptr);
    delete segment;
    delete dataset;
}

TEST(Class_TPlot_Tests, Converting_Constructor_Test)
{
    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment segment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment.copy_depths_to_segment();
    segment.copy_ages_to_segment();
    segment.set_g1_ptr();
    segment.compute_lsr_values();
    for (size_t i = 0; i < 10; i++)
    {
        segment.add_to_fit_vector(i);
    }
    segment.perform_fitting();
    segment.get_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.get_pretty_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.set_g2_ptr_pretty();
    segment.lsr_smoothing();
    TPlot* plot = new TPlot(segment);
    EXPECT_GT(plot->get_lsr_ages_vector_size(), plot->get_ages_vector_size());
    delete plot;
    delete dataset;
}

TEST(Class_TPlot_Tests, Vector_Size_Comparison_Against_Dataset)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment segment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment.copy_depths_to_segment();
    segment.copy_ages_to_segment();
    segment.set_g1_ptr();
    segment.compute_lsr_values();
    for (size_t i = 0; i < 10; i++)
    {
        segment.add_to_fit_vector(i);
    }
    segment.perform_fitting();
    segment.get_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.get_pretty_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.set_g2_ptr_pretty();
    segment.lsr_smoothing();
    TPlot* plot = new TPlot(segment);
    EXPECT_EQ(dataset->get_ages_vector_size(), plot->get_ages_vector_size());
    delete plot;
    delete dataset;
}

TEST(Class_TPlot_Tests, Compare_Number_Of_Tiepoints_Between_Age_Vs_Depth_And_LSR_Plots)
{
    dataset = new TData("1090.csv");
    dataset->load_input();
    dataset->find_hiatus();
    TSegment segment(dataset, dataset->get_index(0).first, dataset->get_index(0).second);
    segment.copy_depths_to_segment();
    segment.copy_ages_to_segment();
    segment.set_g1_ptr();
    segment.compute_lsr_values();
    for (size_t i = 0; i < 10; i++)
    {
        segment.add_to_fit_vector(i);
    }
    segment.perform_fitting();
    segment.get_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.get_pretty_fit_line_for_plot(segment.find_the_best_fit(0));
    segment.set_g2_ptr_pretty();
    segment.lsr_smoothing();
    TPlot* plot = new TPlot(segment);
    EXPECT_EQ((plot->get_ages_vector_size() - 1) * 2, plot->get_lsr_ages_vector_size());
    delete plot;
    delete dataset;
}

TEST(Class_TPlot_Tests, Two_Argument_Constructor_Test)
{
    std::vector<TSegment> segments{};

    dataset = new TData("1050.csv");
    dataset->load_input();
    dataset->find_hiatus();

    for (size_t i = 0; i < dataset->get_segment_indexes_size(); i++)
    {
        segments.push_back(TSegment(dataset, dataset->get_index(i).first, dataset->get_index(i).second));
        segments[i].copy_ages_to_segment();
        segments[i].copy_depths_to_segment();
        segments[i].set_g1_ptr();
        segments[i].compute_lsr_values();
    }

    for (size_t i = 0; i < segments.size(); i++)
    {
        for (int j = 0; j < 10; j++)
        {
            segments[i].add_to_fit_vector(j);
        }

        segments[i].perform_fitting();
        segments[i].get_fit_line_for_plot(segments[i].find_the_best_fit(0));
        segments[i].get_pretty_fit_line_for_plot(segments[i].find_the_best_fit(0));
        segments[i].set_g2_ptr_pretty();
        segments[i].lsr_smoothing();
    }

    TPlot* plot = new TPlot((int)segments.size(), segments);
    ASSERT_TRUE((int)segments.size() == plot->get_graphs_size());
    delete plot;
    delete dataset;
    segments.clear();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
