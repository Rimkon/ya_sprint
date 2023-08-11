#include "document.h"


Document::Document ( int id, double relevance, int rating ) 
             :        id ( id ) 
             , relevance ( relevance ) 
             ,    rating ( rating ) { }


std::ostream& 
operator << (std::ostream & ost, const Document & doc )
{
    return ost << "{ document_id = " << doc.id
 << ", relevance = " << doc.relevance
 << ", rating = " << doc.rating << " }";
}


void 
PrintDocument ( const Document& document ){
    using namespace std; 
             cout<< "{ "s
             << "document_id = "s << document.id << ", "s
             << "relevance = "s << document.relevance << ", "s
             << "rating = "s << document.rating << " }"s <<endl;
}