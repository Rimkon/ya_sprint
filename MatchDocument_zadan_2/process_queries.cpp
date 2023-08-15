
    #include <algorithm> 
#include <cmath> 
#include <iostream> 
#include <map> 
#include <set> 
#include <stdexcept> 
#include <string> 
#include <execution>
#include <utility> 
#include <vector> 
#include "search_server.h"
#include "process_queries.h"

void RemoveDuplicates(SearchServer& search_server) 
{
    std::set<std::set<std::string>> unique_words;
    std::vector<int> duplicates;

    for (int document_id : search_server) {
        std::set<std::string> words;
        for (const auto& [word, _] : search_server.GetWordFrequencies(document_id)) {
            words.insert(word);
        }
        if (!unique_words.insert(words).second) {
            duplicates.push_back(document_id);
        }
    }
    for (int document_id : duplicates) {
        std::cout << "Found duplicate document id " << document_id << std::endl;
        search_server.RemoveDocument(document_id);
    }
}


std::vector<std::vector<Document>> 
ProcessQueries( const SearchServer& search_server, const std::vector<std::string>& queries)
{
std::vector<std::vector<Document>> search_result (queries.size());

std::transform (std::execution::par, cbegin (queries), cend (queries), begin (search_result),
        [&search_server]( const std::string &s ) {
            return search_server.FindTopDocuments(s);
        });

    return search_result;
}



std::vector<Document>
   ProcessQueriesJoined ( const SearchServer &  search_server,
              const std::vector<std::string> &  queries )
   {
       std::vector<std::vector<Document>> search_result = ProcessQueries (search_server, queries);
       std::vector<Document> line_search_result;
  
   for(const std::vector<Document>& docs : search_result) {
        line_search_result.insert(line_search_result.end(), docs.begin(), docs.end());}
  
  
       return line_search_result;
   }
  