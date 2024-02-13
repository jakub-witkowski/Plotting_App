#include "tplot.h"
#include "tsegment.h"

#include <iostream>
#include <vector>

TPlot::TPlot()
{
    // std::cout << "TPlot Constructor" << std::endl;
}

/* converting constructor */
TPlot::TPlot(TSegment s)
{
    for (int i = 0; i < s.get_ages_vector_size(); i++)
    {
        depths.push_back(s.get_depths(i));
        ages.push_back(s.get_ages(i));
        fit_line.push_back(s.get_fit_line(i));
        lsr_plot_values.push_back(s.get_lsr_plot_value(i));
        smoothed_lsr_plot_values.push_back(s.get_smoothed_lsr_plot_value(i));
        lsr_plot_ages.push_back(s.get_lsr_plot_age(i));
    }
    set_g1_ptr();
    set_g2_ptr();
    set_g3_ptr();
    set_g4_ptr();
    // std::cout << "TPlot Converting Constructor" << std::endl;
}

TPlot::~TPlot()
{
    // std::cout << " TPlot ~Destructor" << std::endl;
}

void TPlot::copy_ages_to_plot()
{
    for (int i = 0; i < this->segm_ptr->get_ages_vector_size(); i++)
    {
        this->set_ages(this->segm_ptr->get_ages(i));
    }
}

void TPlot::copy_depths_to_plot()
{
    for (int i = 0; i < this->segm_ptr->get_depths_vector_size(); i++)
    {
        this->set_depths(this->segm_ptr->get_depths(i));
    }
}

void TPlot::copy_fit_line_to_plot()
{
    for (int i = 0; i < this->segm_ptr->get_fit_line_vector_size(); i++)
    {
        this->set_fit_line(this->segm_ptr->get_fit_line(i));
    }
}

void TPlot::copy_lsr_plot_values_to_plot()
{
    for (int i = 0; i < this->segm_ptr->get_lsr_plot_values_vector_size(); i++)
    {
        this->set_lsr_plot_values(this->segm_ptr->get_lsr_plot_value(i));
    }
}

void TPlot::copy_smoothed_lsr_plot_values_to_plot()
{
    for (int i = 0; i < this->segm_ptr->get_smoothed_lsr_plot_values_vector_size(); i++)
    {
        this->set_smoothed_lsr_plot_values(this->segm_ptr->get_smoothed_lsr_plot_value(i));
    }
}

void TPlot::copy_lsr_plot_ages_to_plot()
{
    for (size_t i = 0; i < this->segm_ptr->get_lsr_plot_ages_vector_size(); i++)
    {
        this->set_lsr_plot_ages(this->segm_ptr->get_lsr_plot_age(i));
    }
}

/* displays data stored in the ages vector of a TSegment object */
void TPlot::display_ages_vector()
{
    std::cout << "Data in the ages vector: " << std::endl;
    for (size_t i = 0; i < get_ages_vector_size(); i++)
    {
        std::cout << i << ": " << this->ages[i] << std::endl;
    }
}

void TPlot::delete_ptrs()
{
    delete this->g1;
    delete this->g2;
    delete this->g3;
    delete this->g4;
}

void TPlot::plot()
{
    this->cnv->Divide(2,1);
    this->cnv->cd(1)->SetLeftMargin(0.15);

    this->cnv->cd(1);
    this->g1->SetTitle("Age vs Depth, raw");
    this->g1->SetMarkerColor(4);
    this->g1->SetMarkerSize(1.25);
    this->g1->SetMarkerStyle(20);
    
    this->g2->SetTitle("Polynomial fit");
    this->g2->SetLineColor(2);
    this->g2->SetLineWidth(2);

    this->multi1->Add(g1, "p");
    this->multi1->Add(g2, "l");
    this->multi1->SetName("AvD");
    this->multi1->SetTitle("Age vs depth plot with polynomial smoothing; Age (Ma); Depth (mbsf)");
    this->multi1->GetXaxis()->CenterTitle();
    this->multi1->GetYaxis()->CenterTitle();
    this->multi1->GetYaxis()->SetTitleOffset(2.25);
    this->multi1->Draw("A RY");
    
    this->cnv->cd(2);

    this->g3->SetTitle("LSR variability, raw");
    this->g3->SetLineColor(4);
    this->g3->SetLineWidth(2);

    this->g4->SetTitle("LSR variability, smoothed");
    this->g4->SetLineColor(2);
    this->g4->SetLineWidth(2);

    this->multi2->Add(g3, "l");
    this->multi2->Add(g4, "l");
    this->multi2->SetName("LSR");
    this->multi2->SetTitle("Raw vs smoothed LSR plot; Age (Ma); Linear sedimentation rate (cm/kyr)");
    this->multi2->GetXaxis()->CenterTitle();
    this->multi2->GetYaxis()->CenterTitle();
    this->multi2->Draw("A L");

    this->cnv->Modified();
    this->cnv->Update();
}

void TPlot::plot_to_png(std::string f)
{
    this->cnv->Divide(2,1);
    this->cnv->cd(1);

    this->g1->SetTitle("Age vs Depth, raw");
    this->g1->SetMarkerColor(4);
    this->g1->SetMarkerSize(1.25);
    this->g1->SetMarkerStyle(20);
    
    this->g2->SetTitle("Polynomial fit");
    this->g2->SetLineColor(2);
    this->g2->SetLineWidth(2);

    this->multi1->Add(g1, "p");
    this->multi1->Add(g2, "l");
    this->multi1->SetName("AvD");
    this->multi1->SetTitle("Age vs depth plot with polynomial smoothing; Age (Ma);");
    this->multi1->GetXaxis()->CenterTitle();
    this->multi1->GetYaxis()->CenterTitle();
    this->multi1->Draw("A RY");
    
    this->cnv->cd(2);

    this->g3->SetTitle("LSR variability, raw");
    this->g3->SetLineColor(4);
    this->g3->SetLineWidth(2);

    this->g4->SetTitle("LSR variability, smoothed");
    this->g4->SetLineColor(2);
    this->g4->SetLineWidth(2);

    this->multi2->Add(g3, "l");
    this->multi2->Add(g4, "l");
    this->multi2->SetName("LSR");
    this->multi2->SetTitle("Raw vs smoothed LSR plot; Age (Ma); Linear sedimentation rate (cm/kyr)");
    this->multi2->GetXaxis()->CenterTitle();
    this->multi2->GetYaxis()->CenterTitle();
    this->multi2->Draw("A L");

    this->cnv->Print(f.c_str());
}

void TPlot::set_ages(double a)
{
    this->ages.push_back(a);
}

void TPlot::set_depths(double d)
{
    this->depths.push_back(d);
}

void TPlot::set_fit_line(double d)
{
    this->fit_line.push_back(d);
}

void TPlot::set_lsr_plot_values(double d)
{
    this->lsr_plot_values.push_back(d);
}

void TPlot::set_smoothed_lsr_plot_values(double d)
{
    this->smoothed_lsr_plot_values.push_back(d);
}

void TPlot::set_lsr_plot_ages(double d)
{
    this->lsr_plot_ages.push_back(d);
}

void TPlot::set_segm_ptr(TSegment* s)
{
    this->segm_ptr = s;
}

void TPlot::set_g1_ptr()
{
    this->g1 = new TGraph(this->ages.size(), &this->ages[0], &this->depths[0]);
}

void TPlot::set_g2_ptr()
{
    this->g2 = new TGraph(this->ages.size(), &this->ages[0], &this->fit_line[0]);
}

void TPlot::set_g3_ptr()
{
    this->g3 = new TGraph(this->lsr_plot_ages.size(), &this->lsr_plot_ages[0], &this->lsr_plot_values[0]);
}

void TPlot::set_g4_ptr()
{
    this->g4 = new TGraph(this->lsr_plot_ages.size(), &this->lsr_plot_ages[0], &this->smoothed_lsr_plot_values[0]);
}

/* getter functions */
size_t TPlot::get_ages_vector_size()
{
    return this->ages.size();
}

size_t TPlot::get_lsr_ages_vector_size()
{
    return this->lsr_plot_ages.size();
}

double TPlot::get_lsr_plot_age(int i)
{
    return this->lsr_plot_ages[i];
}
