from subprocess import PIPE, Popen
from fixed_clauses import dimacs
import re

def run_z3(input, timeout):
	process = Popen(
		["/home/npona/Downloads/z3-4.4.0.55ca6ce44b2f-x64-ubuntu-14.04/bin/z3", "-T:" + str(timeout),  "-st", "-dimacs", "-in"],
		stdout=PIPE, stdin=PIPE)
	out = process.communicate(input=input)[0]
	return out.decode()

def get_time(output):
	_, last, _ = output.rsplit("\n", 2)
	if not last.startswith(" :total-time"):
		#print(output, last)
		if not last.startswith(" :"):
			raise Exception("Implementation error :(")
		return
	last = last.strip().rsplit(" ", 1)[-1][:-1]
	return float(last)

def collect(iterations = 50, timeout = 60, nvars = 1000, nclauses = 4000):
	average_time = 0 # average time
	satisfi_perc = 0 # percentage of satisfiable formulae
	unknown_perc = 0 # percentage of unknown (timeout)
	skipped_perc = 0 # percentage of skipped (no time reported by z3)
	
	i = 0
	while i < iterations:
		rnd = dimacs(nvars, nclauses)
		output = run_z3(rnd, timeout)

		elapsed = get_time(output)
		if elapsed is None:
			skipped_perc += 1
			continue
		
		average_time += elapsed
		i += 1
		if output.startswith("sat"):
			satisfi_perc += 1
		elif output.startswith("unsat"):
			pass
		else:
			unknown_perc += 1
			#if unknown_perc > iterations / 5.0:
			#	average_time = - 1.0 * average_time
			#	break
	
	if iterations == unknown_perc:
		print ("z3 didn't return time: void result :(", nvars, nclauses)
		return 0.0, 0.0, 1.0, 0.0

	average_time /= 1.0 * (iterations - unknown_perc)
	satisfi_perc /= 1.0 * (iterations - unknown_perc)
	unknown_perc /= 1.0 * iterations
	skipped_perc /= 1.0 * iterations

	return average_time, satisfi_perc, unknown_perc, skipped_perc, nclauses*1.0/nvars



#if __name__ == "__main__":
#	print collect()

print "---Fixed number of clauses SAT testing---"

number_variables = raw_input("Number of variables:")
iterations = raw_input("Iterations:")
timeout = raw_input("Timeout (optional):")

print "The result of the experiment is in the file 'fc_result%s.dat'" % number_variables

res_file = open("fc_result%s.dat" % number_variables, 'w')

def experiment(number_variables):
    result = [collect(int(iterations),int(timeout),int(number_variables),i) for i in range(2*int(number_variables),8*int(number_variables))]
    return result

def filtered(string):
    string = re.sub('[()]','', string)
    string1 = re.sub('[, ]', '\t', string)
    return string1

def resulting():
    resulting = experiment(int(number_variables))
    res_file.write("# Fixed clause number experiment, variables: %s, iterations: %s timeout: %s \n" % (number_variables, iterations, timeout))
    res_file.write("# Time: Sat %: Unknown %: Skipped %: \n")
    for i in resulting:
        res_file.write(filtered(str(i)) + "\n")

resulting()
res_file.close()

print "Done!"

#if __name__ == "__main__":
#	print collect()
