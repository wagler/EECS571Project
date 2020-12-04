UUnifast will generate a set of simulated tasks, each with a runtime, period, and utilization. 
The project can be built by running 'make UUniFast'. 
The program is run as 'UUniFast [number of task sets] [number of tasks per set] [mean utilization for each taskset to target] [minimum period a task can have] [maximum period a task can have] [output file path]
The program will then generate each task set and all the tasks inside it. It will write the results to the specified output file, in csv format. An example output file would look like:

runtime,period,utilization
14.0954,44,0.32035
12.4046,32,0.387643
2.07938,10,0.207938
1.1243,33,0.0340697

Keep in mind that the UUniFast algorithm will generate task utilizations by pulling values from a uniform distribution centered at the specified mean utilization. The task periods will also be generated from a uniform distribution.

Example Usage:
./UUniFast 10 10 0.95 10 50 tasksets.txt

This generates 10 task sets, each with 10 tasks. The mean utilization will be 0.95, in each task set. The task periods will be chosen from a uniform distribution with range [10,50]. The resulting tasks will be output to the file 'tasksets.txt'