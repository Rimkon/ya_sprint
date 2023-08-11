
#include <cmath> 
#include <execution>
#include <execution>
#include "search_server.h"
#include "string_processing.h"
#include "document.h"
#include <string> 
#include <numeric> 

using namespace std;

SearchServer::
SearchServer ( const string& stop_words_text ): SearchServer ( SplitIntoWords ( stop_words_text ))
{ 
    CheckCharInWord ( stop_words_text );
}


void SearchServer::
SetStopWords ( const string& text )
{
    for ( const string& word : SplitIntoWords ( text )) 
    {
        stop_words_.insert( word );
    }
}


int SearchServer::
GetDocumentCount ( )const 
{
    return static_cast<int> (documents_.size( ));
}


void SearchServer::
AddDocument ( int  document_id, 
                   const string &document, 
                   DocumentStatus  status, 
                   const vector<int> &ratings )
{
    CheckIdDocument ( document_id );
    CheckCharInWord ( document );

    const vector<string> words = SplitIntoWordsNoStop ( document );
    double tf = CalculateTF ( static_cast<double> (words.size( )) );

    for ( string word: words )
    {
        if ( word[0] == '-' || word.empty( )|| word[word.size( )-1]=='-' )
        {
            throw invalid_argument ( "не вверный ввод символа -" );
        }
        word_to_document_freqs_[word][document_id] += tf ;
        document_to_word_freqs_[document_id][word] += tf ;
    }    

    documents_.emplace( document_id, DocumentData{ComputeAverageRating ( ratings ), status} );

    n_doc_id_.insert( document_id );

}//AddDocument


vector<Document> SearchServer::
FindTopDocuments ( const string& raw_query, DocumentStatus status )const 
{
    return FindTopDocuments ( raw_query, [status](int, DocumentStatus s, int ){return status == s;} );
}


tuple<vector<string>, DocumentStatus> SearchServer::
MatchDocument ( const string& raw_query, int document_id )const 
{
    const Query query = ParseQuery ( raw_query );

    vector<string> matched_words = AddPlusWords ( query.plus_words, document_id );
    ClearResultWithMinusWords ( query.minus_words, document_id, matched_words );

    return { matched_words, documents_.at( document_id ).status };
}

tuple<vector<string>, DocumentStatus> SearchServer::
MatchDocument (const std::execution::sequenced_policy&, const std::string& raw_query, int document_id )const 
{
    return MatchDocument ( raw_query, document_id );
}
//******************************************************************************************
//******************************************************************************************
//
tuple<vector<string>, DocumentStatus> SearchServer::
MatchDocument (const std::execution::parallel_policy& policy, const std::string& raw_query, int document_id )const 
{
    const Query query = ParseQuery ( raw_query );
    vector<string> matched_words;

    for_each ( policy, 
              query.plus_words.begin(), query.plus_words.end(), 
              [&] ( const string &word )
              {
                 if ( word_to_document_freqs_.at( word ).count( document_id )) 
                 {
                 matched_words.push_back ( word );
                 }
              }
    );

//    for ( const string& word : query.plus_words )
//    {
//        if ( word_to_document_freqs_.count( word )== 0 )
//        {
//            continue;
//        }
//
//        if ( word_to_document_freqs_.at( word ).count( document_id )) 
//        {
//            matched_words.push_back( word );
//        }
//    }//for
     //
    //
    ClearResultWithMinusWords ( query.minus_words, document_id, matched_words );

    return { matched_words, documents_.at( document_id ).status };
    
}
//
//******************************************************************************************
//******************************************************************************************
double SearchServer::
CalculateTF ( const double total_words )const 
{
    return static_cast<double> (1.0 )/ total_words;
}


double SearchServer::
CalculateIDF ( const double match_word_in_docs )const 
{
    return  std::log ( static_cast<double> (n_doc_id_.size( )) / match_word_in_docs );
}


bool SearchServer::
IsStopWord ( const string& word )const 
{
    return stop_words_.count( word )> 0;
}


vector<string> SearchServer::
SplitIntoWordsNoStop ( const string& text )const 
{
    vector<string> words;
    for ( const string& word : SplitIntoWords ( text )) 
    {
        if ( !IsStopWord ( word )) {
            words.push_back( word );
        }
    }
    return words;
}


bool SearchServer::
IsValidWord ( const string& word )
{
    return none_of ( word.begin( ), word.end( ), [](char c ){
            return c >= '\0' && c < ' ';
            } );
}


SearchServer::QueryWord SearchServer::
ParseQueryWord ( string word )const
{
    bool is_minus = false;
    if ( word[0] == '-' )
    {
        is_minus = true;
        word = word.substr( 1 );
    }
    CheckCharInWord ( word );

    if ( word.empty( )|| word[0] == '-' )
        throw invalid_argument ( "не вверный ввод символа -" );

    return {word, is_minus, IsStopWord ( word )};
}


SearchServer::Query SearchServer::
ParseQuery ( const string& text )const 
{
    CheckCharInWord ( text );
    Query query;

    if ( text.empty( ))
    {
        throw invalid_argument ( "попытка добавить пустой документ" );
    }

    for ( const string& word : SplitIntoWordsNoStop ( text )) 
    {
        const QueryWord query_word = ParseQueryWord ( word );

        if ( ! query_word.is_stop )
        {
            if ( query_word.is_minus )
            {
                query.minus_words.insert( query_word.word );
            }
            else
            {
                query.plus_words.insert( query_word.word );
            }
        }
    }
    return query;
}


int SearchServer::
ComputeAverageRating ( const vector<int> & ratings )
{
    if ( ratings.empty( )) {
        return 0;
    }
    int rating_sum = accumulate ( ratings.begin( ), ratings.end( ), 0 );

    return rating_sum / static_cast<int> (ratings.size( ));
}


void SearchServer::
EraseTFIDFwithMinusWords ( const set<string> &minus_words, map<int, double> &tf_idf )const
{
    for ( const auto& word : minus_words )
    {
        if ( word_to_document_freqs_.count( word )== 0 )
        {
            continue;
        }

        if ( word_to_document_freqs_.count( word )> 0 )
        {
            for ( const auto& [document_id, TF] : word_to_document_freqs_.at( word ))
            {
                tf_idf.erase( document_id );
            }
        }
    } // for minus_words
}


vector<string> SearchServer::
AddPlusWords ( const set<string> &plus_words, const int document_id )const
{
    vector<string> matched_words;

    for ( const string& word : plus_words )
    {
        if ( word_to_document_freqs_.count( word )== 0 )
        {
            continue;
        }

        if ( word_to_document_freqs_.at( word ).count( document_id )) 
        {
            matched_words.push_back( word );
        }
    }//for
    return matched_words;
}


void SearchServer::
ClearResultWithMinusWords ( const set<string> &minus_words, const int document_id, 
        vector<string> &matched_words )const
{
    for ( const string& word : minus_words )
    {
        if ( word_to_document_freqs_.count( word )== 0 )
        {
            continue;
        }

        if ( word_to_document_freqs_.at( word ).count( document_id )) 
        {
            matched_words.clear( );
            break;
        }
    }
}


vector<Document> SearchServer::
ResultMatchedDocuments ( const map<int, double> &tf_idf )const
{
    vector<Document> matched_documents;
    for ( const auto&[document_id, relevance] : tf_idf )
    {
        matched_documents.push_back( 
                { document_id, relevance, documents_.at( document_id ).rating } );
    }
    return matched_documents;
}


vector<Document> SearchServer::
SetDocumentForRelevance (vector<Document> matched_documents ) const
{
    const double inaccuracy = 1e-6;

    sort ( matched_documents.begin( ), matched_documents.end( ), 
           [inaccuracy](const Document& lhs, const Document& rhs )
           {
                if ( abs ( lhs.relevance - rhs.relevance )< inaccuracy )
                {
                    return lhs.rating > rhs.rating;  // при одинаковой релевантности сортировка идет по рейтингу
                } 
                else 
                {
                    return lhs.relevance > rhs.relevance;
                }
           });//sort

    return matched_documents;
}


void SearchServer::
SetCurrentSizeForRezult ( vector<Document> &matched_documents, const long unsigned int max_result_document_count )const
{
    if ( static_cast<long unsigned int>(matched_documents.size( )) > max_result_document_count )
    {
        matched_documents.resize( max_result_document_count );
    }
}


void SearchServer::
CheckIdDocument ( const int document_id )const 
{
    if ( document_id < 0 )
    {
        throw invalid_argument ( "id не может быть отрицательным" );
    }

    if (n_doc_id_.find(document_id) != n_doc_id_.end())
    {
        throw invalid_argument ( "Ошибка ID документов, совпадают" );
    }
}


void SearchServer::
CheckCharInWord ( const string& text )const 
{
    if ( ! IsValidWord ( text ))
    {
        throw invalid_argument ( "не допустимый диапазон символов от 0 до 31" );
    }
}


const map<string, double>& SearchServer::
GetWordFrequencies ( int document_id )const
{
    if ( document_to_word_freqs_.count( document_id )== 0 )
    { 
        throw std::out_of_range ( "Документ с таким ID не найден" ); 
    }
    return document_to_word_freqs_.at( document_id );
}


void SearchServer::
RemoveDocument ( int document_id )
{
    RemoveDocument ( std::execution::seq, document_id );
}

void SearchServer::
RemoveDocument  (const std::execution::sequenced_policy& , int document_id)
{
    ParSeqRemoveDocument (std::execution::seq, document_id);
}
 
void SearchServer::
RemoveDocument  (const std::execution::parallel_policy& , int document_id)
{
    ParSeqRemoveDocument (std::execution::par, document_id);
}
