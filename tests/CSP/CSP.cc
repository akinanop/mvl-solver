#include <iostream>
#include <fstream>

#include "CSP.h"
using namespace std;

// CONSTRUCTORS
CSP::CSP(){
  ;
}

CSP::CSP(list <int> vars)
{
  for(i=0; i < vars.size(); i++)
  {
    VARS.push_back(vars[i]);
  }
}

// CONSTRAINTS to MVL

CSP::alldifferent()
{
  ifstream infile;
  char line_buffer[3000];
  char word_buffer[3000];
  int clause_num = 0;
  int atom_num = 0;
  int var, val;
  char ch;
  char ch1;
  string str;
  int temp;
  vector <int> DOMAINS.reserve(10);
/*  Variable * temp_var = NULL;
  Literal * temp_atom = NULL;
  Clause * temp_clause = NULL; */

  ifstream in("in.txt");
  ofstream out("out.txt");

  while(in.getline(line_buffer, 3000))
    {

  else if(line_buffer[0] == 'd')
{
temp = sscanf(line_buffer, "d %d %d", &var, &val);
DOMAINS.push_back(val);
out << "d "<< var << " " << val << endl;
}

 else if (line_buffer[0] == 'A' && line_buffer[1] == 'D') {

 char * lp = line_buffer;
 do
   {
     char * wp = word_buffer;
     //erasing all leading space/tabs
     while (*lp && ((*lp == ' ') || (*lp == '\t')))
 {
   lp++;
 }
     //reading each value into word buffer
     while (*lp && (*lp != ' ') && (*lp != '\t') && (*lp != '\n'))
 {
   *(wp++) = *(lp++);
 }
     *wp = '\0';
     //converting into int and writing to file
     if(strlen(word_buffer) != 0)
 {
   temp = sscanf(word_buffer, "%d", &var);
   //checking if variable = 0 i.e end of line
   if(var != 0)
     { for(i=0; i < DOMAINS.size(); i++)
       {  int dom = DOMAINS[0];
          out << var <<

     }

}

}
  in.close();
  out.close();

  return true;

}
