# Adaptive EDF Scheduler for FreeRTOS

We implemented earliest-deadline-first task scheduling in the FreeRTOS kernel.


All the code that's relevant to the project is located in the `FreeRTOS/FreeRTOS/Demo/Posix_GCC` and `task_generator` directories.
To generate a task set, go to the `task_generator` directory and run `python3 UUniFast.py [num tasks] [total utilization] [min period] [max period] [probability of a checkpointed task being generated] [output file]`. Then, run `python3 TaskFuncGen [input csv file] source.c` to generate the C code for the task set. Then copy `source.c` to `FreeRTOS/FreeRTOS/Demo/Posix_GCC/`

The file that holds all the task code is `source.c`, located in `FreeRTOS/FreeRTOS/Demo/Posix_GCC`.
Once a source.c file is placed in that directory, make sure you're in `FreeRTOS/FreeRTOS/Demo/Posix_GCC` and run `make posix_demo`.
This should create a build directory `FreeRTOS/FreeRTOS/Demo/Posix_GCC/build` with an executable called `posix_demo` inside.
This is the executable you should run.
