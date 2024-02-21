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
    for (size_t i = 1; i < this->ages.size(); i++)
    {
        this->lsr_values.push_back(((this->depths[i] - this->depths[i-1]) * 100) / ((this->ages[i] - this->ages[i-1]) * 1000));
    }

    for (size_t i = 0; i < this->ages.size() - 1; i++)
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
    for (size_t i = this->index_from; i <= this->index_to; i ++)
    {
        this->set_ages(this->dset->get_ages(i));
    }
}

/* copies data that were originally loaded into the TDataset object (depths vector)
starting from position INDEX_FROM to position INDEX_TO */

void TSegment::copy_depths_to_segment()
{
    for (size_t i = this->index_from; i <= this->index_to; i ++)
    {
        this->set_depths(this->dset->get_depths(i));
    }
}

/* performs fitting */
void TSegment::perform_fitting()
{
    for (size_t i = 0; i < this->fit.size(); i++)
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

void TSegment::clear_pretty_fit_line_vector()
{
    this->pretty_fit_line.clear();
}

/*void TSegment::delete_ptrs()
{
    // delete this->cnv;
    // delete this->multi1;
    // delete this->multi2;
    delete this->g1;
    delete this->g2;
    delete this->g3;
    delete this->g4;
}*/

int TSegment::find_the_best_fit(int ind)
{
    std::vector<std::pair<double, int>> best_fit{};
    std::pair<double, int> item{};

    for (size_t i = 0; i < this->fit.size() - 1; i++)
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

    for (size_t i = 0; i < this->pretty_fit_line.size() - 1; i++)
    {
        if (pretty_fit_line[i+1] < pretty_fit_line[i])
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
    for (size_t i = 0; i < this->ages.size(); i++)
    {
        this->fit_line.push_back(compute_polynomial_expression(deg, this->ages[i]));
    }
}

void TSegment::get_pretty_fit_line_for_plot(int deg)
{
    double step{};

    if ((*(this->ages.end() - 1) - *(this->ages.begin())) < 10)
        step = 0.25;
    else
        step = 0.5;

    if (this->ages_for_pretty_fit_line.size() == 0)
    {
        this->ages_for_pretty_fit_line.push_back(this->ages[0]);
        size_t index{1};
        while (*(this->ages_for_pretty_fit_line.end() - 1) < (*(this->ages.end() - 1) - step))
        {
            this->ages_for_pretty_fit_line.push_back(this->ages_for_pretty_fit_line[index - 1] + step);
            index++;
        }
    }
    
    if (*(this->ages_for_pretty_fit_line.end() - 1) < *(ages.end() - 1))
        this->ages_for_pretty_fit_line.push_back(*(this->ages.end() - 1));

    for (size_t i = 0; i < this->ages_for_pretty_fit_line.size(); i++)
    {
        this->pretty_fit_line.push_back(compute_polynomial_expression(deg, this->ages_for_pretty_fit_line[i]));
    }
}

/* calculates smoothed LSR values */
void TSegment::lsr_smoothing()
{
    for (size_t i = 1; i < this->ages.size(); i++)
    {
        this->smoothed_lsr_values.push_back(((this->fit_line[i] - this->fit_line[i-1])*100)/((this->ages[i] - this->ages[i-1])*1000));
    }

    /* establish LSR values and age tiepoints for plotting vector */
    for (size_t i = 0; i < this->ages.size() - 1; i++)
    {
        this->smoothed_lsr_plot_values.push_back(this->smoothed_lsr_values[i]);
        this->smoothed_lsr_plot_values.push_back(this->smoothed_lsr_values[i]);
    }
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

void TSegment::set_g2_ptr_pretty()
{
    this->g2 = new TGraph(this->ages_for_pretty_fit_line.size(), &this->ages_for_pretty_fit_line[0], &this->pretty_fit_line[0]);
}

/*void TSegment::set_g2_ptr()
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
}*/

void TSegment::add_to_fit_vector(int d)
{
    this->fit.push_back(new TPolynomial(d));
}

/* getter functions */
TData* TSegment::get_dataset_ptr()
{
    return this->dset;
}

size_t TSegment::get_index_from()
{
    return this->index_from;
}

size_t TSegment::get_index_to()
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

double TSegment::get_pretty_fit_line(int i)
{
    return this->pretty_fit_line[i];
}

double TSegment::get_ages_for_pretty_fit_line(int i)
{
    return this->ages_for_pretty_fit_line[i];
}

size_t TSegment::get_fit_line_vector_size()
{
    return this->fit_line.size();
}

size_t TSegment::get_ages_vector_size()
{
    return this->ages.size();
}

size_t TSegment::get_depths_vector_size()
{
    return this->depths.size();
}

size_t TSegment::get_lsr_values_vector_size()
{
    return this->lsr_values.size();
}

size_t TSegment::get_lsr_plot_values_vector_size()
{
    return this->lsr_plot_values.size();
}

size_t TSegment::get_smoothed_lsr_plot_values_vector_size()
{
    return this->smoothed_lsr_plot_values.size();
}

size_t TSegment::get_lsr_plot_ages_vector_size()
{
    return this->lsr_plot_ages.size();
}

size_t TSegment::get_pretty_fit_line_vector_size()
{
    return this->pretty_fit_line.size();
}

size_t TSegment::get_ages_for_pretty_fit_line_vector_size()
{
    return this->ages_for_pretty_fit_line.size();
}

TGraph* TSegment::get_g2_pretty_ptr()
{
    return this->g2;
}

/*TGraph* TSegment::get_g1_ptr()
{
    return this->g1;
}

TGraph* TSegment::get_g3_ptr()
{
    return this->g3;
}*/
