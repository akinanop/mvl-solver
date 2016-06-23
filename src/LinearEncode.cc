//**********************************************************************//
// GenerateBenchmark
// This program generates benchmarks in modified cnf form
//
//**********************************************************************//
//Including Libraries and Header files
#include "Global.h"
#include "Formula.h"
#include <cstring>
using namespace std;
//**********************************************************************//
//LinearEncode
//This function converts a finite domain file into a boolean domain
//using the Linear Encoding technique
//
// Part of this code uses the idea Shweta has implemented.
void LinearEncode(CommandLine * cline)
{
  //local variables
  ifstream infile;
  ofstream outfile;
  char line_buffer[3000];
  char word_buffer[3000];
  int num_clause = 0;
  int num_var = 0;
  int new_num_clause = 0;
  int new_num_var = 0;
  int * domain;
  int * index;
  int var, val;
  int temp;
  bool isdone = false;
  int count = 0;
  char ch, ch1;

  //opening infile to read and checking if it opens
  infile.open(cline->FILE, ios::in);
  if(!infile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open input file : "<<endl;
      cout<<endl;
      exit(1);
    }

  //opening outfile to write the new clauses
  outfile.open(cline->MODEL_FILE, ios::out);
  if(!outfile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open output file : "<<endl;
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
	  num_var = var;
	  num_clause = val;
	  domain = new int[var+1];
	  index = new int[var+1];
	  //default initialization to domain value 2
	  for(int i=0; i<var+1; i++)
	    domain[i] = 2;
	}
      //else if line is d var# domsize
      else if(line_buffer[0] == 'd')
	{
	  temp = sscanf(line_buffer, "d %d %d", &var, &val);
	  count++;
	  new_num_var += 2*val-1;
	  new_num_clause += 4*(val-2)+3+1;
	  domain[var] = val;
	}
      //else its variables
      else
	{
	  if(!isdone)
	    {
	      isdone = true;
	      new_num_clause += num_clause;
	      new_num_var += (num_var - count);
	      outfile<<"p cnf "<<new_num_var<<" "<<new_num_clause<<endl;
	      //compute the index values for each variable=domainvalue
	      index[1] = 1;
	      for(int i=2; i<num_var+1; i++)
		{
		  if(domain[i-1] == 2)
		    index[i] = index[i-1]+1;
		  else
		    index[i] = 2*domain[i-1] -1 + index[i-1];
		}
	      int var_t1 = 0;
	      int var_t2 = 0;
	      //add the extra clauses
	      for(int i=0; i<num_var+1; i++)
		{
		  if(domain[i] > 2)
		    {
		      //write the ALO condition
		      for(int j=0; j<domain[i]; j++)
			{
			  outfile<<index[i]+j<<" ";
			}
		      outfile<<" 0"<<endl;
		      var_t1 = index[i]+domain[i];
		      var_t2 = index[i];
		      outfile<<-(var_t1)<<" "<<(var_t2)<<" 0"<<endl;
		      outfile<<var_t1<<" "<<-(var_t2)<<" 0"<<endl;
		      for(int j=0; j<domain[i]-2; j++)
			{
			  outfile<<-(var_t1+1+j)<<" "<<(var_t1+j)<<" "<<var_t2+1+j<<" 0"<<endl;
			  outfile<<var_t1+1+j<<" "<<-(var_t1+j)<<" 0"<<endl;
			  outfile<<var_t1+1+j<<" "<<-(var_t2+1+j)<<" 0"<<endl;
			}
		      for(int j=0; j<domain[i]-1; j++)
			{
			  var_t1=-(index[i]+domain[i]+j);
			  var_t2=-(index[i]+1+j);
			  outfile<<var_t1<<" "<<var_t2<<" 0\n";
			}
		    }
		  else
		    {
		      //do nothing
		      ;
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
		  if(ch == '!')
		    temp = sscanf(word_buffer, "%d %c %c %d", &var, &ch, &ch1, &val);
		  //checking if variable = 0 i.e end of line
		  if(var != 0)
		    {
		      if(domain[var] > 2)
			{
			  var = index[var] + val;
			  if(ch == '!')
			    outfile<<"-"<<var<<" ";
			  else
			    outfile<<var<<" ";
			}
		      else
			{
			  if(val == 1)
			    {
			      var = index[var];
			      outfile<<var<<" ";
			    }
			  else
			    {
			      var = -index[var];
			      outfile<<var<<" ";
			    }
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
