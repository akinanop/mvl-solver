from sys import stdout
from subprocess import PIPE, Popen
from random_sat import dimacs

def show(string):
	"""Logging."""
	stdout.write(string)
	stdout.flush()


def statistics(data):
	"""Return the deviation of data. Statistically unfounded."""
	n = len(data) * 1.0
	mean = sum(data) / n
	ss = sum((x - mean) ** 2 for x in data)
	return mean, (ss / n) ** 0.5

def run_z3(input, timeout):
	process = Popen(
		["C:\\Users\\Amministratore\\Downloads\\z3-4.3.2-x64-win\\z3-4.3.2-x64-win\\bin\\z3", "-T:" + str(timeout),  "-st", "-dimacs", "-in"],
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
	show("> NVARS=%d,NCLAUSES=%d;ITER=%d:\t" % (nvars, nclauses, iterations))
	times = [] # this collects the times from the iterations
	# average_time = 0 # average time
	satisfi_perc = 0 # percentage of satisfiable formulae
	unknown_perc = 0 # percentage of unknown (timeout)
	skipped_perc = 0 # percentage of skipped (no time reported by z3)
	
	i = 0
	while i < iterations:
		show(str(i) + " ")
		rnd = dimacs(nvars, nclauses)
		output = run_z3(rnd, timeout)

		elapsed = get_time(output)
		if elapsed is None:
			skipped_perc += 1
			continue
		
		#average_time += elapsed
		times.append(elapsed)
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
	
	print("")
	if len(times) < 2:
		print ("[WARNING] z3 didn't return time: void result :(")
		return 0.0, 0.0, 1.0, 0.0
	
	average_time, variance = statistics(times)
	satisfi_perc /= 1.0 * (iterations - unknown_perc)
	unknown_perc /= 1.0 * iterations
	skipped_perc /= 1.0 * iterations

	return average_time, satisfi_perc, unknown_perc, skipped_perc, variance



#if __name__ == "__main__":
#	print collect()
