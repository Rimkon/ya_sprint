#pragma once

#include <cstring>
#include <iostream>
#include <vector>
#include <cmath>
#include "read_input_functions.h"



template <typename T_iterator>
class IteratorRange
{
public:
    IteratorRange ( T_iterator T_it_begin, T_iterator T_it_end, std::size_t size_n )
                   :it_begin ( T_it_begin ), it_end ( T_it_end ){};

    T_iterator begin ( ){ return it_begin; };
    T_iterator end   ( ){ return it_end;   };
    T_iterator size  ( ){ return it_end - it_begin; };

private:
    T_iterator it_begin;
    T_iterator it_end;
};



template <typename T>
class Paginator
{
public:
    Paginator ( T it_begin, T it_end, std::size_t docs_on_list )// получаем вектор на найденные документы. И по сколько их выводить.
    {
        int total_docs_     = distance ( it_begin, it_end );
        total_listings_ = ceil ( total_docs_ / static_cast<double> (docs_on_list ));
        
        int listings_on_page = docs_on_list;

        if ( total_docs_ < listings_on_page )
            listings_on_page = total_docs_;


        auto listings_begin = it_begin;

        for ( int i = 0; i < total_listings_; i++ )
        {
            if ( (listings_begin + listings_on_page )< it_end )
                find_docs_.push_back( { listings_begin, listings_begin + listings_on_page, docs_on_list } );
            else
            {
                find_docs_.push_back( { listings_begin, it_end, docs_on_list } );
                break;
            }
            listings_begin += docs_on_list;
            
        }

    } //Paginator ( )

    auto   begin ( )const { return find_docs_.begin( ); };
    auto   end ( )const { return find_docs_.end( ); };
    size_t size ( )const { return find_docs_.size( ); };

private:
    int total_listings_;
    std::vector<IteratorRange<T> > find_docs_; // например храним по 2 документа в каждом векторе.

};



template <typename T_iterator>
std::ostream& operator << (std::ostream & ost, IteratorRange <T_iterator> it_range )
{
    for ( auto doc = it_range.begin( ); doc != it_range.end( ); doc++ )
    {
        std::cout<< *doc;
    }
     return ost;
} 



template <typename Container>
auto Paginate ( const Container& c, size_t page_size ){
    return Paginator ( begin ( c ), end ( c ), page_size );
}