//**********************************************************************//
//Including Libraries and Header files
#include "Global.h"
#include <cstring>


using namespace std;

//Global Declaration - Variables

//**********************************************************************//
//Function Definitions
//Boolean2Finite
void Boolean2Finite(CommandLine * cline)
{
  //local variables
  ifstream infile;
  ofstream outfile;
  char line_buffer[MAX_LINE];
  char word_buffer[MAX_LINE];
  int line_num = 0;
  int variable = 0;
  set<int> variables;
  vector <vector<int> > data;

  //opening infile to read and checking if it opens
  infile.open(cline->FILE, ios::in);
  if(!infile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open input file"<<endl;
      cout<<endl;
      exit(1);
    }

  //opening outfile to write and checking if it opens
  outfile.open(cline->MODEL_FILE, ios::out);
  if(!outfile)
    {
      cout<<endl;
      cout<<"**** ERROR ****"<<endl;
      cout<<"Could not open input file"<<endl;
      cout<<endl;
      exit(1);
    }

  //now reading each line from the infile and if needed to convert
  //then converting it and writing to outfile or just writing directly
  while(infile.getline(line_buffer, MAX_LINE))
    {
      ++line_num;
      //if line is comment - just write
      if(line_buffer[0] == 'c')
	{
	  outfile.write(line_buffer, strlen(line_buffer));
	  outfile.put('\n');
	}
      //else if line is p cnf var_num clause_num - just write
      else if(line_buffer[0] == 'p')
	{
	  outfile.write(line_buffer, strlen(line_buffer));
	  outfile.put('\n');
	}
      //else its variables
      else
	{
	  char * lp = line_buffer;
	  vector <int> temp;
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
		  variable = atoi(word_buffer);
		  variables.insert(abs(variable));
		  //checking if variable = 0 i.e end of line
		  if(variable != 0)
		    {
		      //checking for sign
		      if(variable < 0)
			{
			  //variable = -variable;
			  //outfile<<variable<<"=0 ";
			  temp.push_back(variable);
			}
		      else
			{
			  //outfile<<variable<<"=1 ";
			  temp.push_back(variable);
			}
		    }
		  //else print newline in outfile
		  else
		    {
		      //outfile.put('\n');
		    }
		}
	    }while(*lp);
	  data.push_back(temp);
	}
    }

  //now writing back the domain information
  set<int>::iterator start = variables.begin();
  set<int>::iterator end = variables.end();
  end--;
  for( ; start != end; )
    {
      ++start;
      outfile<<"d "<<*start<<" 2"<<endl;
    }

  int ovsize = data.size();
  int uvsize = 0;
  int var = 0;
  for(int y=0; y<ovsize; y++)
    {
      uvsize = data[y].size();
      for(int x=0; x<uvsize; x++)
	{
	  var = data[y][x];
	  if(var < 0 )
	    {
	      var = -var;
	      outfile<<var<<"=0 ";
	    }
	  else
	    {
	      outfile<<var<<"=1 ";
	    }
	}
      outfile<<endl;
    }
  //closing file
  infile.close();
  outfile.close();
}
//End GenBenFunction
//**********************************************************************//
