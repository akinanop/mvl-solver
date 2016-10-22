

import sys
import os.path 
complete_pebbler = open("complete_pebbler.txt",'a')
vertices = []
edges = []

def ccg_1():
  infile = open("graph_descr.txt", 'r')
  for line in infile:
     if line[0] == "s" or line[0] == "n" or line[0] == "t":
        vertices.append(line[:line.index(":")])
ccg_1()

def ccg_2():
   infile = open ("graph_descr.txt", 'r')
   for line in infile:
      if line[0] == "p":
          
          edges.append("(" + line[line.index(":")+1:line.index("\n")] + "," +
line[line.index("(")+1:line.index(")")] + ")" )

ccg_2()







complete_pebbler.write("c" + " This is a graph with vertices" + str(vertices) + " and edges " +
str(edges) + "\n" )
complete_pebbler.write("p" + " " + str(len(vertices)) + " " + str(len(edges)) + "\n")

def ccg_3():
   for i in edges:
       complete_pebbler.write("e" + " " + i[1:i.index(",")] + " " + i[i.index(",")+1:i.index(")")] + "\n")

ccg_3()  



def souf():
   infile = open("graph_descr.txt", 'r')
   for line in infile:
     for i in range(len(line)): 
            if (line[0])== "s":
             if (line[i]) == ":" :
                 print line[i+2:]
                 sources = open("sources.txt",'a')
                 sources.write(line)
                                  
souf()
def sources_into_final():
       infile = open("sources.txt", 'r')
       for line in infile:
          if len(line)>1:
              complete_pebbler.write(line[line.index(":")+2:])

sources_into_final()
def tarf():
    infile = open("graph_descr.txt",'r')
    for line in infile:
        for i in range(len(line)):
             if (line[0]) == "t":
               if line[i] == ":":
                 
                 targets = open("targets.txt",'a')
                 targets.write(line [(i+1):])
tarf()



set_1 = set()


def tarnegf():
    infile = open("targets.txt", 'r')
    outfile = open("targets_negatives.txt",'w')
    for line in infile:
       for i in range(len(line)):
         if  line[i:i+3] == " !=" :
                line_help = (line[i+4:] + " ")
                list_1 = list(line[:i])
                list_2 = list_1[::-1]
                list_2[list_2.index(" "):] = []
                list_3 = list_2[::-1]
                if len(line_help) > 1:
                    set_1.add((''.join(list_3)) + " = " + line_help[0:line_help.index(" ")])
         if   line[i:i+2] == " =" :
                line_help = (line[i+3:] + " ")
                list_1 = list(line[:i])
                list_2 = list_1[::-1]
                list_2[list_2.index(" "):] = []
                list_3 = list_2[::-1]
                if len(line_help) > 1:
                    set_1.add((''.join(list_3)) + " != " + line_help[0:line_help.index(" ")])
    outfile = open ("targets_negative.txt", 'a')  
    for i in set_1:
         if i[len(i)-1] == ("\n"):
            outfile.write(i)
         else:
            outfile.write(i + "\n") 
tarnegf()


def tarnegs_into_final():
       infile = open("targets_negative.txt", 'r')
       for line in infile:
          if len(line)>1:
              complete_pebbler.write(line)

tarnegs_into_final()




def parents():
   infile = open("graph_descr.txt", 'r')
   outfile = open("pre_parent_nodes.txt",'a')
   for line in infile:
     
            if (line[0]) == "p":
              print ("Suliko")
              line_parent = line[(line.index(":")+2)]
                     
              print  (line)
              outfile.write(line)       
parents()
def relating_parents():
    infile = open ("pre_parent_nodes.txt",'r')
    for line_1 in infile:
        for line_2 in infile:
             if line_1[line_1.index("("): line_1.index(")")]  == line_2[line_2.index("("):line_2.index(")")]:
                  print ( ">>" + line_1[line_1.index(":")+2:]  + ">>" + "\n")  
                  outfile = open ("parent_nodes.txt", 'a')
                  outfile.write ((">" + "\n" + line_1[(line_1.index(":")+2):]  + line_2[(line_2.index(":")+2):]  +  ">>" + "\n" )) 
                  break
relating_parents()


def eliminating_spaces(a):
    if type(a) == str:
       c = list(a)
       d = c[::-1]
       d[d.index(" "):] = []
       d = d[::-1]
       e = ''.join(d)
       return e       
 


def eliminating_signs(a):
      if type(a) == str:
          c = list(a)
          d = c[::-1]
          d[d.index(">"):] = []
          d = d[::-1]
          e = ''.join(d)
          return e
 

clauses_separated = []

def comb_clauses_separation():
   infile = open("parent_nodes.txt", 'r')
   clauses_separated =  infile.readlines()
   print clauses_separated
   clauses_separated_string = ' '.join(clauses_separated)
   print clauses_separated_string


   for i in range(2,len(clauses_separated_string)):
         if clauses_separated_string[i:(i+2)] == ">>":   
              print (eliminating_signs(clauses_separated_string[:i]))
              outfile = open (("clause_sep_%s.txt" % i),'a')
              outfile.write(eliminating_signs(clauses_separated_string[:i]) + "0")
             

comb_clauses_separation() 


 
flynt = []

def analyzing_directory():


   for i in range(4000):
  
       if os.path.isfile("clause_sep_%s.txt" % i) == True:
           infile = open (("clause_sep_%s.txt" % i), 'r')
          
           for line in infile:
               if (len(line) > 1):
                   if line[1] == "s" or line[1] == "n":
                     
                      line_rock = line[1:line.index("\n")]
                      
                      
                      flynt.append(line_rock)
                                            
                                                
analyzing_directory()


print flynt


def preparing_combining():

 infile_1 = open ("graph_descr.txt", 'r')        
              
 for i in range(4000):
  
       if os.path.isfile("clause_sep_%s.txt" % i) == True:

            


         
             for line_1 in infile_1:
                 
                        if (len(line_1) > 1):
                          
                          for j in range (len(flynt)): 
                              if line_1[0:(line_1.index(" "))] == flynt[j] :

                             
     
                                    outfile = open(("clause_sep_comb_%s.txt" % i), 'a') 
                                    outfile.write(line_1[line_1.index(":")+1:])                        
                                    print line_1[(line_1.index(": ")+2):]                
                                    break                                   

preparing_combining()
prep_fin = []
def preparing_final():

   
     for j in range(4000):
 
        

           if os.path.isfile("clause_sep_comb_%s.txt" % j) == True:
          


                infile = open(("clause_sep_comb_%s.txt" % j), 'r')
                prep_fin = infile.readlines()
                for i in range(2000):
                    
                    if len(prep_fin) > 0 :
                       outfile = open (("prep_fin_%s.txt" % i), 'a')
                       
                       outfile.write(''.join(prep_fin[0:2]))                   
                       prep_fin[0:2] = []
                                        


preparing_final()
             

com_buf_1 = []                 
com_buf_2 = []
com_buf_rev_1 = []
com_buf_rev_2 = [] 



def buf(i):
   infile = open (("prep_fin_%s.txt" % i), 'r')
 
   for line_1 in infile:
      outfile = open (("com_buf_1_%s.txt" % i), 'w')
      outfile.write(line_1)
      for line_2 in infile:
          if line_2 != line_1:
              outfile_2 = open (("com_buf_2_%s.txt" % i), 'a')
              outfile_2.write(line_2)
              


def buf_prep_final():
 

    for i in range(4000):
  
       if os.path.isfile("prep_fin_%s.txt" % i) == True:
             buf(i)
           
             

buf_prep_final()   


def eliminating_spaces(a):
    if type(a) == str:
       c = list(a)
       d = c[::-1]
       d[d.index(" "):] = []
       d = d[::-1]
       e = ''.join(d)
       return e       

                              
def comb(k):
    infile = open (("prep_fin_%s.txt" % k), 'r')

    infile_1 = open ("graph_descr.txt",'r')
    infile_2 = open (("com_buf_1_%s.txt" % k), 'r')
    infile_3 = open (("com_buf_2_%s.txt" % k), 'r') 
    infile_4 = open ("targets.txt", 'r')
  
    com_buf_rev_1 = []
    com_buf_rev_2 = []
    for line in infile_2:
             if len(line) > 1:   
                 
                 for i in range(len(line)):
                     if line[i] == "=":
                        if line[i-1] == " ":
                           line_1 = line[:i-1]
                           line_2 = eliminating_spaces(line_1)
                           line_comb = (line_2 + " != " + line[(i+2):(i+3)] +
line[(i+3):line.index(" ")] + "\n")  

                           
                           outfile_1 = open ("clauses_value_changed_1_%s.txt" % k, 'a')  
                           outfile_1.write(line_comb)                   
                 for j in range(len(line)):
                     if line[j] == "=":
                        if line[j-1] == "!":
                           print "Great!"
                           line_1 = line[:j-2]
                           line_2 = eliminating_spaces(line_1)
                           line_comb = (line_2 + " = " + line[(j+2):(j+3)] +
line[(j+3):line.index(" ")] + "\n")  

                           
                           outfile_1 = open ("clauses_value_changed_1_%s.txt" % k, 'a')  
                           outfile_1.write(line_comb)                   
 

    for line in infile_3:
             if len(line) > 1:   
               
                  for i in range(len(line)):
                     if line[i] == "=":
                        if line[i-1] == " ":
                           line_1 = line[:i-1]
                           line_2 = eliminating_spaces(line_1)
                           line_comb = (line_2 + " != " + line[(i+2):(i+3)] +
line[(i+3):line.index(" ")] + "\n")  

                           outfile_2 = open ("clauses_value_changed_2_%s.txt" % k, 'a')
                           outfile_2.write(line_comb)
  
                  for j in range(len(line)):
                     if line[j] == "=":
                        if line[j-1] == "!":
                           print ("Great!")
                           line_1 = line[:j-2]
                           line_2 = eliminating_spaces(line_1)
                           line_comb = (line_2 + " = " + line[(j+2):(j+3)] +
line[(j+3):line.index(" ")] + "\n")  
                           
                           outfile_2 = open ("clauses_value_changed_2_%s.txt" % k, 'a')
                           outfile_2.write(line_comb)
                        


                          
    j = infile_4.read()
    print com_buf_rev_1
    print com_buf_rev_2
    for i in com_buf_rev_1:
         for k in com_buf_rev_2:
             if len(i) > 1:
                if len(k) > 1:   
    
                    print (str(i) + " " + str(k) + " " + str(j)) 
                else:
                    return
             else:
                  return     
                             
   




def comb_prep_final():
 

    for k in range(4000):
  
       if os.path.isfile("prep_fin_%s.txt" % k) == True:
             comb(k)
       else:
             return                        
            
comb_prep_final()
buf_prep_final()
def manipulating_buffers(a,b):
   if type(a) == list:
       if type(b) == list:
  
            for i in a:
               for k in b:
                  outfile = open("all_combined_clauses.txt", 'a')
                  outfile.write(str(i) + " " + str(k)  +  "  end" )
 


def final():
 

  for i in range(4000):
     if os.path.isfile("clauses_value_changed_1_%s.txt" % i) == True:
       print ("First thing finished ever")
       infile = open (("clauses_value_changed_1_%s.txt" % i),'r')
       
       infile_1 = open (("clauses_value_changed_2_%s.txt" % i), 'r')
       com_1 = []
       com_2 = []
       for line_1 in infile_1:
            com_1.append(line_1)                
            print com_1      
           
       for line in infile:
           
            com_2.append(line)
            print com_2 
       manipulating_buffers(com_1,com_2)        
                  
                              
final()





def caution():
  with open ("all_combined_clauses.txt", 'r') as myfile:
     combined=myfile.read().replace("\n", '')
    
     combined=combined.replace("end", "\n")
     print combined
     outfile = open ("sunnyroad_totheend.txt", 'a')
     outfile.write(combined)
     outfile.close

caution()



import union_with_clauses       



def combs_into_final():
   
   infile = open("hello_mary_lou.txt",'r')
   for line in infile:
         if len(line) > 1:
            complete_pebbler.write(line)


combs_into_final()



