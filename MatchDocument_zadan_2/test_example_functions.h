#pragma once

#include <iostream>
#include <string>
/* #include <vector> */
/* #include "search_server.h" */

                            // Функция выводящая информацию об исключении при сравнивании объектов
                            // T и U объекты вызывающие исключение
                            // t_str и u_str имена этих объектов
                            // file это файл где было вызвано исключение
                            // func функция где былo вызвано исключение
                            // line номер строки с исключением
                            // hint комментарий который выдало исключение
template <typename T, typename U>
void 
AssertEqualImpl ( const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file, 
                  const std::string& func, unsigned line, const std::string& hint ){

    using std::string_literals::operator""s;

    if ( t != u ){
        std::cout<< std::boolalpha;
        std::cout<< file << "("s << line << " ): "s << func << ": "s;
        std::cout<< "ASSERT_EQUAL ( "s << t_str << ", "s << u_str << " )failed: "s;

        std::cout<< t << " != "s << u << "."s;
        
        if ( !hint.empty( )) {
            std::cout<< " Hint: "s << hint;
        }
        std::cout <<std::endl;
        abort ( );
    }
}


                            // Функция выводящая информацию об исключении
                            // file это файл где было вызвано исключение
                            // func функция где былo вызвано исключение
                            // line номер строки с исключением
                            // hint комментарий который выдало исключение
void 
AssertImpl ( bool value, const std::string& expr_str, const std::string& file, 
             const std::string& func, unsigned line, const std::string& hint );


// Макросы
// #a - при раскрытии макроса вместо параметра вставляется строка
#define ASSERT_EQUAL( a, b )           AssertEqualImpl ( (a ), (b ), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s )
#define ASSERT_EQUAL_HINT( a, b, hint )AssertEqualImpl ( (a ), (b ), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint ))

#define ASSERT( expr )           AssertImpl ( !!(expr ), #expr, __FILE__, __FUNCTION__, __LINE__, ""s )
#define ASSERT_HINT( expr, hint )AssertImpl ( !!(expr ), #expr, __FILE__, __FUNCTION__, __LINE__, (hint ))


// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent ( );
void TestSearchWithStatusDocuments ( );
void TestRelevance         ( );
void TestRating            ( );
void TestMatchDocuments    ( );
void TestMinusWordsInQuery ( );
void TestPredicateSearch   ( );
void TestSortForRelevance  ( );
void TestAddedDocument     ( );
void TestSearchServer      ( );