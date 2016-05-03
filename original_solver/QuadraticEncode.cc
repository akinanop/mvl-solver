//**********************************************************************//
// GenerateBenchmark
// This program generates benchmarks in modified cnf form
//
// Author: Hemal A Lal
// Date : 03-08-2004
//
//**********************************************************************//
//Including Libraries and Header files
#include "Global.h"
#include "Formula.h"

using namespace std;
//**********************************************************************//
//Factorial
//Computes the factorial of a number
inline int Fact(int n)
{
  int temp = 1;
  for(int x=1; x<n+1; x++)
    temp *= x;
  return temp;
}

//QuadraticEncode
//This function converts a finite domain file into a boolean domain
//using the Quadratic Encoding technique
void QuadraticEncode(CommandLine * cline)
{
  //local variables
  ifstream infile;
  ofstream outfile;
  char line_buffer[3000];
  char word_buffer[3000];
  int num_clause = 0;
  int num_var = 0;
  int var, val;
  int temp;
  bool isdone = false;
  bool isneg = false;
  Variable * temp_var;
  vector <Variable *> VAR_LIST;
  char ch, ch1;
  
  //opening infile to read and checking if it opens 
  infile.open(cline->FILE, ios::in);
  if(!infile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open input file : "<<cline->FILE<<endl;
      cout<<endl;
      exit(1);
    }

  //opening outfile to write the new clauses
  outfile.open(cline->MODEL_FILE, ios::out);
  if(!outfile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open output file : "<<cline->MODEL_FILE<<endl;
      cout<<endl;
      exit(1);
    }

  //now reading each line from the infile and if needed to convert
  //then converting it and writing to outfile or just writing directly
  while(infile.getline(line_buffer, 3000))
    {
      //if line is comment - just write
      if(line_buffer[0] == 'c')
	{
	  //do nothing
	  ;
	}
      //else if line is p cnf var_num clause_num - just write
      else if(line_buffer[0] == 'p')
	{
	  //do nothing
	  temp = sscanf(line_buffer, "p cnf %d %d", &var, &val);
	  num_clause += val;
	}
      //else if line is d var# domsize
      else if(line_buffer[0] == 'd')
	{
	  temp = sscanf(line_buffer, "d %d %d", &var, &val);
	  temp_var = new Variable(var, val);	  
	  VAR_LIST.push_back(temp_var);
	  num_var += val;
	  num_clause += (Fact(val)/(Fact(val-2)*Fact(2)));
	  num_clause += 1;
	}
      //else its variables
      else
	{
	  if(!isdone)
	    {
	      isdone = true;
	      outfile<<"p cnf "<<num_var<<" "<<num_clause<<endl;
	      int var_count = VAR_LIST.size();
	      for(int x=0; x<var_count; x++)
		{
		  int var_name = VAR_LIST[x]->VAR;
		  int var_domain = VAR_LIST[x]->DOMAINSIZE;
		  for(int y=0; y<var_domain; y++)
		    outfile<<var_name<<"="<<y<<" ";
		  outfile<<"\n";
		  for(int p=0; p<var_domain; p++)
		    {
		      for(int q=p+1; q<var_domain; q++)
			{
			  outfile<<"-"<<var_name<<"="<<p<<" "<<"-"<<var_name<<"="<<q<<""<<endl;
			}
		    }
		}
	    }
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
		  temp = sscanf(word_buffer, "%d %c %d", &var, &ch, &val);
		  if(var != 0 )
		    {
		      if(ch == '!')
			{
			  temp = sscanf(word_buffer, "%d %c %c %d", &var, &ch, &ch1, &val);
			  outfile<<"-"<<var<<"="<<val<<" ";
			}
		      else
			{
			  outfile<<var<<"="<<val<<" ";
			}
		    }	 
		}     
	    }while(*lp);
	  outfile<<"0"<<endl;
	}
    }

  //closing file
  infile.close();
  outfile.close();
}
//End LinearEncode
//**********************************************************************//
