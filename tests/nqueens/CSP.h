#ifndef CSP_H
#define CSP_H

//Including Libraries and Header files
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "Formula.h"
using namespace std;

class CSP
{
public:
  list <int> VARS;

  // constructors
  CSP();
  CSP(list <int> vars);


  bool alldifferent();




};









#endif
