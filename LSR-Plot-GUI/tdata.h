#ifndef TDATA_H
#define TDATA_H

#include "tsegment.h"

#include <iostream>
#include <vector>
#include <string>

class TData
{
public:
    std::string filename{};

    TData(std::string);

    ~TData();

    void load_input();
    void display_raw_data();
    void display_ages_vector();
    void display_depths_vector();
    void display_segment_indexes_vector();
    bool is_ages_vector_sorted();
    bool is_depths_vector_sorted();
    bool test_input_order();
    int find_hiatus();
    std::pair<size_t,size_t> make_index(int, int);

/* setter functions */
    void set_raw_data(std::string);
    void set_ages(double);
    void set_depths(double);
    void set_segment_index(std::pair<int,int>);

/* getter functions */
    int get_raw_data_size();
    int get_ages_vector_size();
    int get_depths_vector_size();
    int get_segment_indexes_size();
    std::string get_raw_data(int);
    double get_ages(int);
    double get_depths(int);
    std::pair<size_t,size_t> get_index(int);

private:
    std::vector<std::string> raw_data{};
    std::vector<double> ages{};
    std::vector<double> depths{};
    std::vector<std::pair<size_t,size_t>> segment_indexes{};

};

#endif // TDATA_H
