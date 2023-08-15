#include <vector>
#include "request_queue.h"

using namespace std; 

    RequestQueue::RequestQueue ( const SearchServer& search_server )
                   : search_server_( search_server )
                   { 

                   }




    vector<Document> 
    RequestQueue::AddFindRequest ( const string& raw_query, DocumentStatus status )
    {
             CheckActualFinds ( );
             return FindResult ( search_server_.FindTopDocuments( raw_query, status ));
    }


    vector<Document> 
    RequestQueue::AddFindRequest ( const string& raw_query )
    {
             CheckActualFinds ( );
             return FindResult (search_server_.FindTopDocuments( raw_query ));
    }


    int 
    RequestQueue::GetNoResultRequests ( )const 
    {
        return empty_docs;
    }



    void 
    RequestQueue::CheckActualFinds ( )
    {
         ++ tik_tak_count_;
         
         if ( tik_tak_count_ > min_in_day_ )
         {
            requests_.pop_front( );
            empty_docs --;
         }

    }
    

    std::vector<Document> 
    RequestQueue::FindResult (const std::vector<Document> &result )
    {
        if  ( result.empty() )
        {
            empty_docs ++;
        }
        else
        {
            int res = result.size();
            requests_.push_back( { res } );
          
        }
         return result;
    }