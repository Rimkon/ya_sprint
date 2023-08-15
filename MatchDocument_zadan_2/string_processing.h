#pragma once

#include <string> 
#include <vector>
#include <set>



// вход:   строка слов с пробелами
// внутри: избавляемся от пробелов
// выход:  Вектор слов.
std::vector<std::string> SplitIntoWords ( const std::string& );



// вход:   контейнер строк ( vector, set )
// внутри: Избавляемся от пустых строк в контейнере.
// выход:  Контейнер set со строками.
template <typename StringContainer>
std::set<std::string> 
MakeUniqueNonEmptyStrings ( const StringContainer &strings )
{
    std::set<std::string> non_empty_strings;
    for ( const std::string & str : strings )
    {
        if ( !str.empty( )) {
            non_empty_strings.insert( str );
        }
    }
    return non_empty_strings;
}
