#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main( int argc , char** argv ) {
   int count = 4 ;
   string line ;
   const char * oldstr ;
   int oldstrlen ;
   ofstream output ;
   output.open( "results.dat" ) ;
   ifstream infile( argv[ 1 ] ) ;
   if ( infile ) {
      while ( getline( infile , line ) ) {
        oldstr = line.c_str( );
         oldstrlen = strlen( oldstr ) + 1 ;

         if ( count == 34 ||  count == 60 || count == 66 || count == 68 || count == 81 ) count++ ;
         if ( count == 71 ||  count == 86 ) count = count + 2 ;

        for ( int i=0; i<oldstrlen; i++ )


            if (oldstr[ i ] == ':'){
              for ( int j = i + 2; j < oldstrlen; j++ ){
                output << oldstr[ j ];
            }
            output << ' ' ;
          } else if ( oldstr[ i ] == 'T' && oldstr[ i + 1 ] == 'h' ) { output << count << endl ; count++ ; }

      }
   }
   infile.close( ) ;
   output.close( ) ;
   return 0 ;
}
