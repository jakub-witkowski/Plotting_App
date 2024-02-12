#include "tdata.h"

#include <fstream>
#include <string>
#include <clocale>

TData::TData(std::string f) : filename(f)
{
    // std::cout << "TData Constructor" << std::endl;
}

TData::~TData()
{
    // std::cout << " TData ~Destructor" << std::endl;
}

/* reads the dataset from the file and stores it in the raw_data vector */
void TData::load_input()
{
    std::ifstream input{filename};

    for (std::string line; std::getline(input, line);)
    {
        set_raw_data(line);
    }

    /* force using '.' as decimal separator */
    const char* old_locale = std::setlocale(LC_NUMERIC, nullptr);
    std::setlocale(LC_NUMERIC, "C");

    /* Separate data on coma, convert substring to double and copy to the respective depths or ages vector */
    for (size_t i = 0; i < raw_data.size(); i++)
    {
        size_t pos = raw_data[i].find(",");
        set_depths(std::stod(raw_data[i].substr(0, pos)));
        set_ages(std::stod(raw_data[i].substr(pos + 1)));
    }
    std::setlocale(LC_NUMERIC, old_locale);
}

void TData::display_raw_data()
{
    std::cout << "Data in the raw_data vector: " << std::endl;
    for (size_t i = 0; i < get_raw_data_size(); i++)
    {
        std::cout << get_raw_data(i) << std::endl;
    }
}

void TData::display_ages_vector()
{
    std::cout << "Data in the ages vector: " << std::endl;
    for (size_t i = 0; i < get_ages_vector_size(); i++)
    {
        std::cout << i << ": " << get_ages(i) << std::endl;
    }
}

void TData::display_depths_vector()
{
    std::cout << "Data in the depths vector: " << std::endl;
    for (size_t i = 0; i < get_depths_vector_size(); i++)
    {
        std::cout << i << ": " << get_depths(i) << std::endl;
    }
}

void TData::display_segment_indexes_vector()
{
    std::cout << "Data in the segment_indexes vector:" << std::endl;
    for (size_t i = 0; i < get_segment_indexes_size(); i++)
    {
        std::cout << "segment_indexes[" << i << "]: " << segment_indexes[i].first << " " << segment_indexes[i].second << std::endl;
    }
}

bool TData::is_ages_vector_sorted()
{
    bool result{true};

    for (size_t i = 1; i < get_ages_vector_size(); i++)
    {
        if (get_ages(i) < get_ages(i-1))
            result = false;
    }

    return result;
}

bool TData::is_depths_vector_sorted()
{
    bool result{true};

    for (size_t i = 1; i < get_depths_vector_size(); i++)
    {
        if (get_depths(i) < get_depths(i-1))
            result = false;
    }

    return result;
}

bool TData::test_input_order()
{
    bool a = is_ages_vector_sorted();
    bool d = is_depths_vector_sorted();
    return ((a == true) && (d == true) ? true : false);
}

int TData::find_hiatus()
{
    int number_of_hiatuses{};
    std::pair<int,int> previous_index{};
    std::pair<int,int> current_index{};
    current_index.first = 0;
    previous_index = current_index;

    for (size_t i = 1; i < this->get_depths_vector_size(); i++)
    {
        if (this->get_depths(i) == this->get_depths(i-1))
        {    
            number_of_hiatuses++;
            current_index.second = i-1;

            if (number_of_hiatuses == 1)
            {
                this->set_segment_index(this->make_index(current_index.first,current_index.second));
                previous_index = current_index;
            }
            else if ((number_of_hiatuses > 1) && (i < this->get_depths_vector_size() - 1))
            {
                this->set_segment_index(this->make_index(previous_index.second + 1, current_index.second));
                previous_index = current_index;
            }        
        }
        
        if (i == this->get_depths_vector_size() - 1)
        {
            current_index.second = i;
            if (number_of_hiatuses == 0)
            {
                current_index.first = 0;
            }
            else
                current_index.first = previous_index.second + 1;
            this->set_segment_index(this->make_index(current_index.first, current_index.second));    
        }
    }

    return number_of_hiatuses;
}

std::pair<size_t,size_t> TData::make_index(int from, int to)
{
    std::pair<size_t,size_t> index{};
    index.first = from;
    index.second = to;
    return index;
}

/* getter functions */
size_t TData::get_raw_data_size()
{
    return this->raw_data.size();
}

size_t TData::get_ages_vector_size()
{
    return this->ages.size();
}

size_t TData::get_depths_vector_size()
{
    return this->depths.size();
}

size_t TData::get_segment_indexes_size()
{
    return this->segment_indexes.size();
}

std::string TData::get_raw_data(int i)
{
    return this->raw_data[i];
}

double TData::get_ages(int i)
{
    return this->ages[i];
}

double TData::get_depths(int i)
{
    return this->depths[i];
}

std::pair<size_t, size_t> TData::get_index(int i)
{
    return this->segment_indexes[i];
}

/* setter functions */
void TData::set_raw_data(std::string l)
{
    this->raw_data.push_back(l);
}

void TData::set_ages(double a)
{
    this->ages.push_back(a);
}

void TData::set_depths(double d)
{
    this->depths.push_back(d);
}

void TData::set_segment_index(std::pair<int,int> i)
{
    this->segment_indexes.push_back(i);
}
