#ifndef TSEGMENT_H
#define TSEGMENT_H

class TData;

/* include ROOT classes */
// #include "TF1.h" // ROOT class for defining 1-dimensional functions
#include "TGraph.h" // ROOT class enabling the creation of plots with X and Y axes and a set of points
//include "/snap/root-framework/928/usr/local/include/TGraph.h"
#include "TCanvas.h" // ROOT graphics class
#include "TMultiGraph.h" // A TMultiGraph allows to manipulate a set of graphs as a single entity (from ROOT documentation)
#include "TAxis.h"

#include <vector>
#include "tpolynomial.h"

class TSegment
{
public:
    TSegment(TData*, size_t, size_t);
    ~TSegment();

    void display_ages_vector();
    void display_depths_vector();
    void display_lsr_values_vector();
    void display_lsr_plot_values_vector();
    void display_lsr_plot_ages_vector();
    void compute_lsr_values();
    void perform_fitting();
    void clear_fit_line_vector();
    void delete_ptrs();
    int find_the_best_fit(int);
    bool test_for_overfitting();
    double compute_polynomial_expression(int, double);
    void get_fit_line_for_plot(int);
    void copy_ages_to_segment();
    void copy_depths_to_segment();
    void lsr_smoothing();
    void plot_to_png(std::string);

    /* setter functions */
    void set_ages(double);
    void set_depths(double);
    void set_g1_ptr();
    void set_g2_ptr();
    void set_g3_ptr();
    void set_g4_ptr();
    void add_to_fit_vector(int);
    
    /* getter functions */
    TData* get_dataset_ptr();
    int get_index_from();
    int get_index_to();
    double get_ages(int);
    double get_depths(int);
    double get_fit_line(int);
    double get_lsr_plot_value(int);
    double get_smoothed_lsr_plot_value(int);
    double get_lsr_plot_age(int);
    int get_ages_vector_size();
    int get_depths_vector_size();
    int get_fit_line_vector_size();
    int get_lsr_values_vector_size();
    int get_lsr_plot_values_vector_size();
    int get_smoothed_lsr_plot_values_vector_size();
    int get_lsr_plot_ages_vector_size();
    TGraph* get_g1_ptr();
    TGraph* get_g3_ptr();

private:
    TData* dset{nullptr}; // set by the constructor at initialisation
    size_t index_from{};
    size_t index_to{};

    std::vector<double> ages{};
    std::vector<double> depths{};
    std::vector<double> lsr_values{};
    std::vector<double> lsr_plot_values{};
    std::vector<double> lsr_plot_ages{};
    std::vector<double> fit_line{};
    std::vector<double> smoothed_lsr_values{};
    std::vector<double> smoothed_lsr_plot_values{};

    TCanvas* cnv = new TCanvas("cnv", "cnv", 0, 0, 1200, 800);
    TMultiGraph* multi1 = new TMultiGraph();
    TMultiGraph* multi2 = new TMultiGraph();
    TGraph* g1{nullptr};
    TGraph* g2{nullptr};
    TGraph* g3{nullptr};
    TGraph* g4{nullptr};

    std::vector<TPolynomial*> fit{};
};

#endif // TSEGMENT_H
