#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main( int argc , char** argv ) {
  // int count = 4 ;
   string line ;
   const char * oldstr ;
   int oldstrlen ;
   ofstream output ;
   output.open( "results4_60noR" ) ;
   ifstream infile( argv[ 1 ] ) ;
   if ( infile ) {
      while ( getline( infile , line ) ) {
        oldstr = line.c_str( );
         oldstrlen = strlen( oldstr ) + 1 ;
        for ( int i=0; i<oldstrlen; i++ )
            if (oldstr[ i ] == ':'){
              for ( int j = i + 2; j < oldstrlen; j++ ){
                output << oldstr[ j ];
                cout<<oldstr[ j ];
            }
            output << ' ' ;
            cout << ' ' ;

          }
          output << endl;
           cout << endl;
      }
   }
   infile.close( ) ;
   output.close( ) ;
   return 0 ;
}
