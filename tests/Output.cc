#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;


int main( int argc , char** argv ) {
   string line ;
   const char * oldstr;
   int oldstrlen;

   ifstream infile( argv[ 1 ] ) ;
   if ( infile ) {
      while ( getline( infile , line ) ) {
        oldstr = line.c_str();
         oldstrlen = strlen(oldstr)+1;

        for ( int i=0; i<oldstrlen; i++ )

            if (oldstr[i] == ':'){
            for ( int j=i+2; j < oldstrlen; j++){
              cout<<oldstr[j];
            }
            cout<<endl;
          }

      }
   }
   infile.close( ) ;
   return 0 ;
}
