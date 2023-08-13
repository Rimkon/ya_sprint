#include <string>
#include <vector>
#include <iostream>
#include "document.h"
#include "search_server.h"
#include "test_example_functions.h"

using namespace std; 


void 
AssertImpl ( bool value, const string& expr_str, const string& file, 
             const string& func, unsigned line, const string& hint ){
    if ( !value ){
        cout<< file << "("s << line << " ): "s << func << ": "s;
        cout<< "ASSERT ( "s << expr_str << " )failed."s;
        if ( !hint.empty( )) {
            cout<< " Hint: "s << hint;
        }
        cout <<endl;
        abort ( );
    }
}


void 
TestExcludeStopWordsFromAddedDocumentContent ( ){
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id, content, DocumentStatus::ACTUAL, ratings );
        const auto found_docs = server.FindTopDocuments( "in"s );
        ASSERT_EQUAL ( found_docs.size( ), 1u );
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL ( doc0.id, doc_id );
    }

    {
        SearchServer server("и o y"s);
        server.SetStopWords( "in the"s );
        server.AddDocument( doc_id, content, DocumentStatus::ACTUAL, ratings );
        ASSERT_HINT ( server.FindTopDocuments( "in"s ).empty( ), 
                    "Stop words must be excluded from documents"s );
    }
}


void 
TestRelevance( )
{
    const double EPSILON = 1e-6;
             int doc_id = 42;

    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat"s};

    const string text_for_find = "my love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        server.AddDocument( doc_id,   document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));

        vector<Document> rating_from_class = server.FindTopDocuments( text_for_find );
        ASSERT ( rating_from_class.size( )== 3 );

        auto matched_documents = server.FindTopDocuments( text_for_find, DocumentStatus::ACTUAL );
        ASSERT ( abs( rating_from_class.at( 0 ).relevance - 0.6016309587 )< EPSILON );
        ASSERT ( abs( rating_from_class.at( 1 ).relevance - 0.4054651081 )< EPSILON );
        ASSERT ( abs( rating_from_class.at( 2 ).relevance - 0.1621860432 )< EPSILON );

    }
}


void 
TestRating ( )
{
             int doc_id = 42;

    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat"s};

    const string text_for_find = "my love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        server.AddDocument( doc_id,   document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));

        vector<Document> rating_from_class = server.FindTopDocuments( text_for_find );
        ASSERT ( rating_from_class.size( )== 3 );

        ASSERT ( rating_from_class.at( 0 ).rating == 4 );
        ASSERT ( rating_from_class.at( 1 ).rating == 1 );
        ASSERT ( rating_from_class.at( 2 ).rating == 3 );
    }
}


void 
TestMatchDocuments ( )
{

    int doc_id = 20;
    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat"s};

    const string text_for_find = "my love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        server.AddDocument( doc_id,   document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));

        vector<string> text;
        DocumentStatus docStatus;
        map <int, vector<string> > rezultat;

        tie ( text, docStatus )= server.MatchDocument( text_for_find, doc_id );
        rezultat[doc_id] = {"cat"s, "love"s};
        ASSERT ( doc_id == 22 );
        ASSERT ( text == rezultat.at( doc_id ));

        tie ( text, docStatus )= server.MatchDocument( text_for_find, --doc_id );
        rezultat[doc_id] = {"cat"s, "my"s};
        ASSERT ( doc_id == 21 );
        ASSERT ( text == rezultat.at( doc_id ));

        tie ( text, docStatus )= server.MatchDocument( text_for_find, --doc_id );
        rezultat[doc_id] = {"city"s, "in"s, "love"s, "my"s};
        ASSERT ( doc_id == 20 );
        ASSERT ( text == rezultat.at( doc_id ));
    }
}


void
TestMinusWordsInQuery ( )
{
    int doc_id = 20;
    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat"s};

    const string text_for_find = "my -love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        server.AddDocument( doc_id,   document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));

        
        vector<string> text;
        DocumentStatus docStatus;
        map <int, vector<string> > rezultat;

        tie ( text, docStatus )= server.MatchDocument( text_for_find, doc_id );
        
        rezultat[doc_id];
        ASSERT ( doc_id == 22 );
        ASSERT ( text == rezultat.at( doc_id ));

        tie ( text, docStatus )= server.MatchDocument( text_for_find, --doc_id );
        rezultat[doc_id] = {"cat"s, "my"s};
        ASSERT ( doc_id == 21 );
        ASSERT ( text == rezultat.at( doc_id ));

        tie ( text, docStatus )= server.MatchDocument( text_for_find, --doc_id );
        rezultat[doc_id];
        ASSERT ( doc_id == 20 );
        ASSERT ( text == rezultat.at( doc_id ));
    }
}


void
TestPredicateSearch ( )
{

    int doc_id = 20;
    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat"s};

    const string text_for_find = "my love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL,  ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL,  ratings.at( 1 ));
        server.AddDocument( doc_id++, document_text.at( 2 ), DocumentStatus::REMOVED, ratings.at( 2 ));
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::BANNED,  ratings.at( 0 ));
        server.AddDocument( doc_id,   document_text.at( 1 ), DocumentStatus::REMOVED, ratings.at( 1 ));


        const vector<Document> find_doc = server.FindTopDocuments( text_for_find, 
                                          [](int, DocumentStatus status, int )
                                          { 
                                              return status == DocumentStatus::REMOVED;
                                          } );
        ASSERT ( find_doc.at( 0 ).id == 22 ); 
        ASSERT ( find_doc.at( 1 ).id == 24 ); 

       const vector<Document> find_doc2 = server.FindTopDocuments( text_for_find, 
                                          [](int id, DocumentStatus status, int )
                                          { 
                                              return ( status == DocumentStatus::ACTUAL && id == 20 );
                                          } );

        ASSERT ( find_doc2.at( 0 ).id == 20 ); 
    }
}


void
TestSearchWithStatusDocuments ( )
{

    int doc_id = 20;
    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "cat love cat some"s,
                                          "new text good text"s,
                                          "tests text for good time"s};

    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL,     ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL,     ratings.at( 1 ));
        server.AddDocument( doc_id++, document_text.at( 2 ), DocumentStatus::REMOVED,    ratings.at( 2 ));
        server.AddDocument( doc_id++, document_text.at( 3 ), DocumentStatus::IRRELEVANT, ratings.at( 3 ));
        server.AddDocument( doc_id++, document_text.at( 4 ), DocumentStatus::BANNED,     ratings.at( 4 ));


        vector<Document> find_docOk = server.FindTopDocuments( "cat"s, DocumentStatus::ACTUAL );
        ASSERT ( find_docOk.size( ) == 1 );
        ASSERT ( find_docOk.at( 0 ).id == 21 );
        vector<Document> find_docBad = server.FindTopDocuments( "some"s, DocumentStatus::ACTUAL );
        ASSERT ( find_docBad.empty());

        find_docOk = server.FindTopDocuments( "love"s, DocumentStatus::REMOVED);
        ASSERT ( find_docOk.size( ) == 1 );
        ASSERT ( find_docOk.at( 0 ).id == 22 );
        find_docBad = server.FindTopDocuments( "in"s, DocumentStatus::REMOVED);
        ASSERT ( find_docBad.empty());

        find_docOk = server.FindTopDocuments( "good"s, DocumentStatus::IRRELEVANT);
        ASSERT ( find_docOk.size( ) == 1 );
        ASSERT ( find_docOk.at( 0 ).id == 23 );
        find_docBad = server.FindTopDocuments( "cat"s, DocumentStatus::IRRELEVANT);
        ASSERT ( find_docBad.empty());

        find_docOk = server.FindTopDocuments( "for"s, DocumentStatus::BANNED);
        ASSERT ( find_docOk.size( ) == 1 );
        ASSERT ( find_docOk.at( 0 ).id == 24 );
        find_docBad = server.FindTopDocuments( "the"s, DocumentStatus::BANNED);
        ASSERT ( find_docBad.empty());
    }
}


void 
TestSortForRelevance ()
{
    int doc_id = 20;
    const vector<string> document_text = {"in my love the city"s, 
                                          "my new cat, good cat"s, 
                                          "new text good text"s,
                                          "tests text for good time"s,
                                          "cat love cat"s};

    const string text_for_find = "my love cat in city"s;
    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}, {3, 4, 3}, {0, 2, 1}};
    {
        SearchServer server("и o y"s);
        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        server.AddDocument( doc_id++, document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));
        server.AddDocument( doc_id++, document_text.at( 3 ), DocumentStatus::ACTUAL, ratings.at( 3 ));
        server.AddDocument( doc_id,   document_text.at( 4 ), DocumentStatus::ACTUAL, ratings.at( 4 ));
    
        vector<Document> find_docs = server.FindTopDocuments (text_for_find);
        vector<Document> notSort_find_docs = find_docs;

        sort (find_docs.begin(), find_docs.end()
             ,[](const Document &lhs, const Document &rhs){return lhs.relevance > rhs.relevance;});

        auto docs_size = find_docs.size();
        ASSERT (docs_size == notSort_find_docs.size());

        for (unsigned long i = 0; i < docs_size  ; i++) {
            string error = "Документ "s + to_string ( i ) + " добавлен неверно."s;
            ASSERT_EQUAL_HINT ( find_docs.at(i).relevance,  notSort_find_docs.at(i).relevance, error );
        }
    }

}


void 
TestAddedDocument ()
{
    int doc_id = 20;
    const vector<string> document_text = {"one"s, 
                                          "two"s, 
                                          "tree"s,
                                          "four"s,
                                          "five"s};

    const vector<vector<int> > ratings = {{4, 6, 3}, {3, 4, 3}, {0, 2, 1}, {3, 4, 3}, {0, 2, 1}};
    {
            
        SearchServer server("и o y"s);

    
        ASSERT ( server.GetDocumentCount () == 0); 

        server.AddDocument( doc_id++, document_text.at( 0 ), DocumentStatus::ACTUAL, ratings.at( 0 ));
        ASSERT ( server.GetDocumentCount () == 1); 

        server.AddDocument( doc_id++, document_text.at( 1 ), DocumentStatus::ACTUAL, ratings.at( 1 ));
        ASSERT ( server.GetDocumentCount () == 2); 

        server.AddDocument( doc_id++, document_text.at( 2 ), DocumentStatus::ACTUAL, ratings.at( 2 ));
        ASSERT ( server.GetDocumentCount () == 3); 
        
        server.AddDocument( doc_id++, document_text.at( 3 ), DocumentStatus::ACTUAL, ratings.at( 3 ));
        ASSERT ( server.GetDocumentCount () == 4); 

        server.AddDocument( doc_id,   document_text.at( 4 ), DocumentStatus::ACTUAL, ratings.at( 4 ));
        ASSERT ( server.GetDocumentCount () == 5); 


        vector<Document> doc = server.FindTopDocuments( "one two tree four five"s );
        ASSERT ( doc.size( )== 5 );

    }
}


// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer ( )
{
    TestExcludeStopWordsFromAddedDocumentContent ( );
    TestRating ( );
    TestRelevance( );
    TestMinusWordsInQuery  ( );
    TestMatchDocuments   ( );
    TestPredicateSearch ( );
    TestSearchWithStatusDocuments ( );
    TestSortForRelevance ();
    TestAddedDocument ();
}
