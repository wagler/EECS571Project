import random
import sys
import csv

class Task:
    def __init__(self, period, utilization):
        self.period = period
        self.utilization = utilization
        self.runtime = period*utilization
        self.squashable = False
        self.alt = False

# Bini's UUniFast algorithm
# adapted from MaximeCheramy's implementation
def UUniFast(num_tasks, util_mean):
    out = []
    while (len(out) == 0):
        sumU = util_mean
        utils = []
        for i in range(1,num_tasks):
            nSumU = sumU * random.random() ** (1.0/(num_tasks-i))
            utils.append(sumU-nSumU)
            sumU = nSumU
        utils.append(sumU)
        if (all(u <= 1 for u in utils)):
            out = utils
    return out

def generate_periods(num_tasks, min_period, max_period):
    periods = []
    for i in range(num_tasks):
        periods.append(random.randint(min_period,max_period))
    return periods

def make_squash_tasks(tasks, prob_squashable):
    out = []
    for task in tasks:
        if (random.random() < prob_squashable):
            task.squashable = True
            alt_runtime = task.runtime / 3
            alt_period = int(alt_runtime / task.utilization) 
            alt = Task(alt_period,task.utilization)
            task.alt = alt
        out.append(task)
    return out

if __name__ == "__main__":
    num_tasks = int(sys.argv[1])
    util_mean = float(sys.argv[2])
    min_period = int(sys.argv[3])
    max_period = int(sys.argv[4])
    prob_squashable = float(sys.argv[5])
    out_file_path = sys.argv[6]
    
    utils = UUniFast(num_tasks,util_mean)
    periods = generate_periods(num_tasks,min_period,max_period)
    tasks = [Task(p,u) for p,u in zip(periods,utils)]
    tasks = make_squash_tasks(tasks, prob_squashable)

    with open(out_file_path, mode='w') as out_file:
        out_writer = csv.writer(out_file, quotechar='"',quoting=csv.QUOTE_MINIMAL)
        out_writer.writerow(['runtime','period','utilization'])
        for task in tasks:
            if (task.squashable):
                out_writer.writerow(['s', task.runtime, task.period, task.utilization])
                out_writer.writerow(['a', task.alt.runtime, task.alt.period, task.alt.utilization])
            else:
                out_writer.writerow(['n', task.runtime, task.period, task.utilization])
