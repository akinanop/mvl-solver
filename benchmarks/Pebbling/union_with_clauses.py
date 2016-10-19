#-*- coding: utf-8 -*-       
import sys
import os.path

buf_union = []
buf_union_2 = []
def union_1():

   infile = open ("sunnyroad_totheend.txt", 'r')
  

   for line in infile:
         buf_union.append(line[0:line.index(" ")])
         break

   for line_1 in infile:
    
   
             if line_1[0:line_1.index(" ")] != buf_union[len(buf_union)-1]:
                 
                 buf_union.append(line_1[0:line_1.index(" ")])
                 
          
  
union_1()

print buf_union
 
def union_2():
    infile = open("graph_descr.txt", 'r')

    for line in infile:
        for i in buf_union:
   
            if (" " + i + " ") in line:
               buf_union_2.append(line[0:line.index(" ")])                 
                  

              
               

union_2()
             
print buf_union_2
buf_union_3 = []                              

def union_3():
    infile = open("pre_parent_nodes.txt",'r')

    for line in infile:
      for i in buf_union_2:
    
            if i in line[line.index(":"):]:
               
                
                buf_union_3.append(line[0:line.index("\n")])          
           
union_3()

set_buf_union_3 = set(buf_union_3)
print set_buf_union_3
set_buf_union_4 = set()

def union_4():
    for i in set_buf_union_3:
        
        set_buf_union_4.add(i)
        

union_4()      
ls_buf_union_4 = list(set_buf_union_4)
set_buf_union_5 = set() 
print set_buf_union_4
               


print set_buf_union_5

buf_union_6 = []

dls  = {}
dls_2 = {}
def union_5(): 

    infile = open("graph_descr.txt", 'r')
    for i in set_buf_union_4:
        dls[i[i.index(":")+2: len(i)]] = i[i.index("(")+1:i.index(")")]

union_5()
print dls

def union_6(line):
   infile = open("graph_descr.txt", 'r')
  
   for key in dls:
        buf_help = [infile.readlines()]
        for i in buf_help:
              if len(line)>1:
                   
                   if line.startswith(key[:len(key)]):
                          print line[0:line.index("\n")]
                          print ("Great")
                          dls_2[line[line.index(":")+1:]] = dls[key]      
                          break
                
                      
                       
                            
                                                  
                                                   
def meta_union_6():
  infile = open("graph_descr.txt", 'r')
  for line in infile:
       union_6(line)
       for line_1 in infile:
           if line_1[0] != " ":
               if line_1 != line:
                  union_6(line_1) 

meta_union_6()
print dls
print dls_2
dls_3 = {}

def union_7():

    infile = open("graph_descr.txt", 'r' )
   
    for line in infile:
           for key in dls_2:
          
              if len(line)>1 and line.startswith(dls_2[key]):
                    dls_2[key] = line[line.index(":")+2:]
                

union_7() 
print dls_2

def union_8():
    infile = open("sunnyroad_totheend.txt", 'r')
    outfile = open ("hello_mary_lou.txt",'a')
    bufhelp_1 = infile.readlines()
    for i in bufhelp_1:
        for j in dls_2:
             if i[0:i.index(" ")] in j:
                outfile.write (i[:i.index("\n")-1] + dls_2[j])
                break                 
                          
                                   
                      
union_8()            
