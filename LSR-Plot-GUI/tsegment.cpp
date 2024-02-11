#include "tsegment.h"
#include "tdata.h"
#include <iostream>

TSegment::TSegment(TData* ds, size_t f, size_t t) : dset(ds), index_from(f), index_to(t)
{
    // std::cout << "TSegment Constructor" << std::endl;
}

TSegment::~TSegment()
{
    // std::cout << " TSegment ~Destructor" << std::endl;
}

/* displays data stored in the ages vector of a TSegment object */
void TSegment::display_ages_vector()
{
    std::cout << "Data in the ages vector: " << std::endl;
    for (int i = 0; i < get_ages_vector_size(); i++)
    {
        std::cout << i << ": " << this->ages[i] << std::endl;
    }
}

/* displays data stored in the depths vector of a TSegment object */
void TSegment::display_depths_vector()
{
    std::cout << "Data in the depths vector: " << std::endl;
    for (int i = 0; i < get_depths_vector_size(); i++)
    {
        std::cout << i << ": " << this->depths[i] << std::endl;
    }
}

/* displays data stored in the lsr_values vector of a TSegment object */
void TSegment::display_lsr_values_vector()
{
    std::cout << "Data in the lsr_values vector: " << std::endl;
    for (int i = 0; i < get_lsr_values_vector_size(); i++)
    {
        std::cout << i << ": " << this->lsr_values[i] << std::endl;
    }
}

/* displays data stored in the lsr_plot_values vector of a TSegment object */
void TSegment::display_lsr_plot_values_vector()
{
    std::cout << "Data in the lsr_plot_values vector: " << std::endl;
    for (int i = 0; i < get_lsr_plot_values_vector_size(); i++)
    {
        std::cout << i << ": " << this->lsr_plot_values[i] << std::endl;
    }
}

/* displays data stored in the lsr_plot_ages vector of a TSegment object */
void TSegment::display_lsr_plot_ages_vector()
{
    std::cout << "Data in the lsr_plot_ages vector: " << std::endl;
    for (int i = 0; i < get_lsr_plot_ages_vector_size(); i++)
    {
        std::cout << i << ": " << this->lsr_plot_ages[i] << std::endl;
    }
}

/* calculates lsr values based on data from the ages vector and depths vector */
void TSegment::compute_lsr_values()
{
    for (int i = 1; i < this->ages.size(); i++)
    {
        this->lsr_values.push_back(((this->depths[i] - this->depths[i-1]) * 100) / ((this->ages[i] - this->ages[i-1]) * 1000));
    }

    for (int i = 0; i < this->ages.size() - 1; i++)
    {
        this->lsr_plot_values.push_back(this->lsr_values[i]);
        this->lsr_plot_values.push_back(this->lsr_values[i]);
        this->lsr_plot_ages.push_back(this->ages[i]);
        this->lsr_plot_ages.push_back(this->ages[i+1]);
    }
}

/* copies data that were originally loaded into the TDataset object (ages vector)
starting from position INDEX_FROM to position INDEX_TO */
void TSegment::copy_ages_to_segment()
{
    for (int i = this->index_from; i <= this->index_to; i ++)
    {
        this->set_ages(this->dset->get_ages(i));
    }
}

/* copies data that were originally loaded into the TDataset object (depths vector)
starting from position INDEX_FROM to position INDEX_TO */

void TSegment::copy_depths_to_segment()
{
    for (int i = this->index_from; i <= this->index_to; i ++)
    {
        this->set_depths(this->dset->get_depths(i));
    }
}

/* performs fitting */
void TSegment::perform_fitting()
{
    for (int i = 0; i < this->fit.size(); i++)
    {
        this->g1->Fit(this->fit[i]->f, "NQ");
        this->fit[i]->chi2 = this->fit[i]->f->GetChisquare();
        this->fit[i]->ndf = this->fit[i]->f->GetNDF();
        // std::cout << i << ": Chi2/ndf = " << this->fit[i]->chi2 / this->fit[i]->ndf << std::endl;
    }
}

void TSegment::clear_fit_line_vector()
{
    this->fit_line.clear();
}

void TSegment::delete_ptrs()
{
    // delete this->cnv;
    // delete this->multi1;
    // delete this->multi2;
    delete this->g1;
    delete this->g2;
    delete this->g3;
    delete this->g4;
}

int TSegment::find_the_best_fit(int ind)
{
    std::vector<std::pair<double, int>> best_fit{};
    std::pair<double, int> item{};

    for (int i = 0; i < this->fit.size() - 1; i++)
    {
        if((this->fit[i]->chi2 == 0) || (this->fit[i]->ndf == 0))
            continue;
        if(std::isnan(this->fit[i]->chi2 / this->fit[i]->ndf) == true)
            continue;
        else
        {
            item.first = this->fit[i]->chi2 / this->fit[i]->ndf;
            item.second = i;
            best_fit.push_back(item);

            // std::cout << i << ": Chi2/ndf = " << this->fit[i]->chi2 / this->fit[i]->ndf << std::endl;
        }
    }

    std::sort(best_fit.begin(), best_fit.end(),
    [](std::pair<double,int> x, std::pair<double,int> y)
    {
        return x.first < y.first;
    });

    const int best_fit_index = best_fit[ind].second;

    for (int i = 0; i <= best_fit_index; i++)
    {
        this->fit[best_fit_index]->parameters.push_back(this->fit[best_fit_index]->f->GetParameter(i));
    }

    // std::cout << "Best fit for this segment = " << this->fit[best_fit_index]->chi2 / this->fit[best_fit_index]->ndf << std::endl;
    return best_fit_index;
}

/* tests if the fit selected by find_the_best_fit() produces unrealistic values */
bool TSegment::test_for_overfitting()
{
    bool result{false};

    for (int i = 0; i < this->fit_line.size() - 1; i++)
    {
        if (fit_line[i+1] < fit_line[i])
            result = true;
    }

    return result;
}

/* calculates smoothed depth for a given age value using the best fit found for a segment */
double TSegment::compute_polynomial_expression(int deg, double current_value)
{
    double temporary_result{};
    double total = this->fit[deg]->parameters[0];
    for (int i = 1; i <= deg; i++)
    {
        temporary_result = this->fit[deg]->parameters[i] * pow(current_value, i);
        total += temporary_result;
    }
    return total;
}

/* calculates values used for plotting the fit curve of reverse Y axis (no such option in ROOT) */
void TSegment::get_fit_line_for_plot(int deg)
{
    for (int i = 0; i < this->ages.size(); i++)
    {
        this->fit_line.push_back(compute_polynomial_expression(deg, this->ages[i]));
    }

    /* Create a TGraph object for plotting the fit line */
    // this->set_g2_ptr();
}

/* calculates smoothed LSR values */
void TSegment::lsr_smoothing()
{
    for (int i = 1; i < this->ages.size(); i++)
    {
        this->smoothed_lsr_values.push_back(((this->fit_line[i] - this->fit_line[i-1])*100)/((this->ages[i] - this->ages[i-1])*1000));
    }

    /* establish LSR values and age tiepoints for plotting vector */
    for (int i = 0; i < this->ages.size() - 1; i++)
    {
        this->smoothed_lsr_plot_values.push_back(this->smoothed_lsr_values[i]);
        this->smoothed_lsr_plot_values.push_back(this->smoothed_lsr_values[i]);
    }
    
    /* create a TGraph object for plotting the smoothed lsr values */
    // this->set_g4_ptr();
}

/* plots data stored in a TSegment object */
void TSegment::plot_to_png(std::string f)
{
    this->cnv->Divide(2,1);
    this->cnv->cd(1);

    this->g1->SetTitle("Age vs Depth, raw");
    this->g1->SetMarkerColor(4);
    this->g1->SetMarkerSize(1.25);
    this->g1->SetMarkerStyle(20);
    
    // perform_fitting();
    // get_fit_line_for_plot(find_best_fit());
    // get_fit_line_for_plot(find_the_best_fit());

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

    // lsr_smoothing();

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

/* setter functions */
void TSegment::set_ages(double a)
{
    this->ages.push_back(a);
}

void TSegment::set_depths(double d)
{
    this->depths.push_back(d);
}

void TSegment::set_g1_ptr()
{
    this->g1 = new TGraph(this->ages.size(), &this->ages[0], &this->depths[0]);
}

void TSegment::set_g2_ptr()
{
    this->g2 = new TGraph(this->ages.size(), &this->ages[0], &this->fit_line[0]);
}

void TSegment::set_g3_ptr()
{
    this->g3 = new TGraph(this->lsr_plot_ages.size(), &this->lsr_plot_ages[0], &this->lsr_plot_values[0]);
}

void TSegment::set_g4_ptr()
{
    this->g4 = new TGraph(this->lsr_plot_ages.size(), &this->lsr_plot_ages[0], &this->smoothed_lsr_plot_values[0]);
}

void TSegment::add_to_fit_vector(int d)
{
    this->fit.push_back(new TPolynomial(d));
}

/* getter functions */
TData* TSegment::get_dataset_ptr()
{
    return this->dset;
}

int TSegment::get_index_from()
{
    return this->index_from;
}

int TSegment::get_index_to()
{
    return this->index_to;
}

double TSegment::get_ages(int i)
{
    return this->ages[i];
}

double TSegment::get_depths(int i)
{
    return this->depths[i];
}

double TSegment::get_fit_line(int i)
{
    return this->fit_line[i];
}

double TSegment::get_lsr_plot_value(int i)
{
    return this->lsr_plot_values[i];
}

double TSegment::get_smoothed_lsr_plot_value(int i)
{
    return this->smoothed_lsr_plot_values[i];
}

double TSegment::get_lsr_plot_age(int i)
{
    return this->lsr_plot_ages[i];
}

int TSegment::get_fit_line_vector_size()
{
    return this->fit_line.size();
}

int TSegment::get_ages_vector_size()
{
    return this->ages.size();
}

int TSegment::get_depths_vector_size()
{
    return this->depths.size();
}

int TSegment::get_lsr_values_vector_size()
{
    return this->lsr_values.size();
}

int TSegment::get_lsr_plot_values_vector_size()
{
    return this->lsr_plot_values.size();
}

int TSegment::get_smoothed_lsr_plot_values_vector_size()
{
    return this->smoothed_lsr_plot_values.size();
}

int TSegment::get_lsr_plot_ages_vector_size()
{
    return this->lsr_plot_ages.size();
}

TGraph* TSegment::get_g1_ptr()
{
    return this->g1;
}

TGraph* TSegment::get_g3_ptr()
{
    return this->g3;
}
