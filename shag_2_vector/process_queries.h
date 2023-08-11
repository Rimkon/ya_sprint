#pragma once
#include "search_server.h"
#include "document.h"
#include "search_server.h"

std::vector<std::vector<Document>> 
ProcessQueries( const SearchServer& search_server,  const std::vector<std::string>& queries);


void RemoveDuplicates(SearchServer& search_server) ;

std::vector<Document> ProcessQueriesJoined ( 
    const SearchServer &  search_server,         
    const std::vector<std::string> &  queries );