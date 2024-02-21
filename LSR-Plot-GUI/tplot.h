#ifndef TPLOT_H
#define TPLOT_H

class TSegment;

/* include ROOT classes */
#include "TGraph.h" // ROOT class enabling the creation of plots with X and Y axes and a set of points
//#include "/snap/root-framework/928/usr/local/include/TGraph.h"
#include "TCanvas.h" // ROOT graphics class
#include "TMultiGraph.h" // A TMultiGraph allows to manipulate a set of graphs as a single entity (from ROOT documentation)
#include "TAxis.h"
// #include "TPad.h"
// #include "TAttPad.h"
#include "TLegend.h"
#include "TStyle.h"

/* include STL header files */
#include <vector>

class TPlot
{
    public:
    TPlot();
    TPlot(int, std::vector<TSegment>);
    TPlot(TSegment);
    ~TPlot();

    // int array_size;
    TCanvas* cnv = new TCanvas("cnv", "LSR-Plot output", 0, 0, 1200, 800);
    TLegend* leg_left = new TLegend(0.5,0.8,0.9,0.9);
    TLegend* leg_right = new TLegend(0.7,0.8,0.9,0.9);

    void copy_ages_to_plot();
    void copy_depths_to_plot();
    void copy_fit_line_to_plot();
    void copy_pretty_fit_line_to_plot();
    void copy_ages_for_pretty_fit_line_to_plot();
    void copy_lsr_plot_values_to_plot();
    void copy_smoothed_lsr_plot_values_to_plot();
    void copy_lsr_plot_ages_to_plot();
    void display_ages_vector();
    void delete_ptrs();
    void plot();
    void plot_from_array();
    // void plot_to_png(std::string);

    void set_ages(double);
    void set_depths(double);
    void set_fit_line(double);
    void set_lsr_plot_values(double);
    void set_smoothed_lsr_plot_values(double);
    void set_lsr_plot_ages(double);
    void set_pretty_fit_line(double);
    void set_ages_for_pretty_fit_line(double);

    void set_segm_ptr(TSegment*);
    void set_g1_ptr();
    void set_g2_ptr();
    void set_g2_ptr_pretty();
    void set_g3_ptr();
    void set_g4_ptr();

    size_t get_ages_vector_size();
    size_t get_lsr_ages_vector_size();
    double get_lsr_plot_age(int);

    private:
    std::vector<double> depths{};
    std::vector<double> ages{};
    std::vector<double> fit_line{};
    std::vector<double> pretty_fit_line{};
    std::vector<double> ages_for_pretty_fit_line{};
    std::vector<double> lsr_plot_values{};
    std::vector<double> smoothed_lsr_plot_values{};
    std::vector<double> lsr_plot_ages{};

    TSegment* segm_ptr{nullptr};
    TMultiGraph* multi1 = new TMultiGraph();
    TMultiGraph* multi2 = new TMultiGraph();
    TGraph* g1{nullptr};
    TGraph* g2{nullptr};
    TGraph* g3{nullptr};
    TGraph* g4{nullptr};
    // TGraph graphs{nullptr};
    TGraph* graphs = nullptr;
    int graphs_size{};
};

#endif
