#pragma once
#include <iostream>


// Документ имеет следующие характеристики:
// 1) Уникальный номер документа ID
// 2) релевантность документа, это соответствие документа введеному поисковому запросу, расчитвается с помощью TF IDF
// 3) рейтинг 

struct Document {
    Document ( )= default;
    Document ( int id, double relevance, int rating ) ;

    int    id        = 0;
    double relevance = 0.0;
    int    rating    = 0;
};


// класс описывающий актуальность документа. "Актуальный", "Не подходящий", "Заблокирован", "Удалён"

enum class 
DocumentStatus {
    ACTUAL, 
    IRRELEVANT, 
    BANNED, 
    REMOVED, 
};

std::ostream& operator << (std::ostream & ost, const Document & doc );


// вспомогательная функция выводящая содержимое стурктуры Document
void PrintDocument ( const Document& document );