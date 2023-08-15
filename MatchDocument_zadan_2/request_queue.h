#pragma once

#include <vector>
#include <deque>
#include <string>
#include "document.h"
#include "search_server.h"


class RequestQueue {
public:

    explicit 
    RequestQueue ( const SearchServer& search_server );


    template <typename DocumentPredicate>
    std::vector<Document> 
    AddFindRequest ( const std::string& raw_query, DocumentPredicate document_predicate )
    {
             CheckActualFinds ( );
             return FindResult ( search_server_.FindTopDocuments( raw_query, document_predicate ));
    }


    std::vector<Document> 
    AddFindRequest ( const std::string& raw_query, DocumentStatus status );


    std::vector<Document> 
    AddFindRequest ( const std::string& raw_query );


    int 
    GetNoResultRequests ( )const ;


private:


    struct QueryResult {
        int docs_;
    };
    std::deque<QueryResult> requests_;

    int tik_tak_count_ = 0;
    int empty_docs = 0;
    const static int min_in_day_ = 1440;

    const SearchServer &search_server_;


    void 
    CheckActualFinds ( );
    

    std::vector<Document> 
    FindResult (const std::vector<Document> &result );
};