import os
import argparse
import fnmatch

ERROR_REPORT = '{error_type} in {filename}\nLine number {line_number}:\n"{line}"'
DIMACS_PROBLEM_LINE = 'p conf {NumVar:d} {NumClause:d}\n'
DIMACS_DOMAIN_LINE = 'd {VarName:d} {DomainSize:d}\n'
DIMACS_CLAUSE_LINE = '{VarName1:d}!={DomainValue1:d} {VarName2:d}!={DomainValue2:d} 0\n'

def convert_problem_line(line):
    try:
        n_vertices = int(line.split()[2])
        n_edges = int(line.split()[3])
        n_clauses = n_edges * N
        with open(os.path.join(results_folder, result_filename), 'a') as result_file:
            result_file.write(DIMACS_PROBLEM_LINE.format(**{'NumVar':n_vertices, 'NumClause':n_clauses}))
            for v in range(1, n_vertices + 1):
                result_file.write(DIMACS_DOMAIN_LINE.format(**{'VarName':v, 'DomainSize':N}))
        return 'OK'
    except Exception:
        return 'Incorrect problem line'


# adding and parsing arguments: files to be converted and the number of colors
parser = argparse.ArgumentParser()
parser.add_argument("CCG_file", help = "name of mask of files in CCG format to be converted")
parser.add_argument("N", help = "the number of colors", type = int)
parser.add_argument("-folder", help = "the folder to save .dimacs file(s)")
parser.add_argument("-s", help = "skip bad-formatted files", action = "store_true")
args = parser.parse_args()

# saving the number of colors
N = args.N
# saving the results folder and create it if necessary
if args.folder is not None:
    results_folder = args.folder
    if not os.path.exists(results_folder):
        os.makedirs(results_folder)
else:
    results_folder = ''
# matching files to mask
path, files = os.path.split(args.CCG_file)
if path == '':
    files_to_match = os.listdir()
else:
    files_to_match = os.listdir(path)
filenames = []
for f in files_to_match:
    if fnmatch.fnmatch(f, files):
        filenames.append(f)

# reading and converting files        
if filenames == []:
    print("No such file(s).")
else:
    for filename in filenames:
        # creating/overwriting results file 
        print('converting ' + filename)
        result_filename = '.'.join([filename.rsplit('.', maxsplit = 1)[0],'_N' + str(N) + '.dimacs'])
        results_file = open(os.path.join(results_folder, result_filename), 'w')
        results_file.close()
        # setting flags
        problem_line_encountered = False
        omit_comments = False
        # creating empty lists for n_edges
        sources = []
        targets = []
        # reading CCG file
        with open(os.path.join(path,filename), 'r') as f:
            line_number = 1
            line = f.readline()
            while line != '':
                if line[0] == "p":
                    if not problem_line_encountered:
                        report = convert_problem_line(line)
                        problem_line_encountered = True
                    else:
                        report = 'The second problem line is found'
                elif line[0] == "e":
                    # to save the desirable clauses order we should at firsts read all the 
                    # edges to generate clauses lines later
                    try:
                        source,target = [int(s) for s in line.split()[1:3:]]
                        sources.append(source)
                        targets.append(target)
                        report = 'OK'
                    except Exception:
                        report = 'Incorrect edge line'
                    omit_comments = True
                elif line[0] == 'c':
                    # in case we haven't read any edge lines yet we just copy comments without any changes
                    if not omit_comments:
                        result_file = open(os.path.join(results_folder, result_filename), 'a')
                        result_file.write(line)
                        result_file.close()
                    report = 'OK'
                else:
                    report = 'Unknown line type'
                if report != 'OK':
                        print(ERROR_REPORT.format(**{'error_type':report, 'filename':filename, 
                                                     'line_number':line_number, 'line':line.strip()}))
                        if args.s:
                            os.remove(os.path.join(results_folder,result_filename))
                            break
                        else:
                            exit()
                line = f.readline()
        # now we should write the clauses to result file
        with open(os.path.join(results_folder, result_filename), 'a') as result_file:
            for i in range(len(sources)):
                for d in range(N):
                    result_file.write(DIMACS_CLAUSE_LINE.format(**{'VarName1':sources[i],'DomainValue1':d, 'VarName2':targets[i], 'DomainValue2':d}))
    
            
