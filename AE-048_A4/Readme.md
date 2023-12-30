# Project Instructions

## Overview

This project implements a graph algorithm in serial, parallel, and pipeline versions.

The input is a randomly generated graph with a configurable number of nodes.

The programs find the shortest paths between all pairs of nodes in the graph.

## Execution

### 1. Generate input

To generate a random graph input file with 20 nodes:

```
make generate_input
```

This runs the `input_generator` program.

To change the number of nodes, modify the command in the Makefile.

### 2. Run versions

To run the parallel version:

```
make execute_parallel
```

To run the pipeline version:

``` 
make execute_pipeline
```

To run the serial version:

```
make execute_serial 
```

You will be prompted to enter the number of nodes. Since the input generator creates a 20 node graph, enter `20`.

### 3. Check results

The `stat/main.py` Python program checks correctness and calculates speedup.

Install `subprocess` and `matplotlib` to run it. 

Execute with:

```
python stat/main.py
```

It runs the versions with different node counts and compares parallel and pipeline output to the serial output.

## Notes

Let me know if any part needs more explanation or if you have additional questions!