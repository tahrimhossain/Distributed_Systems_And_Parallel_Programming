# Makefile

# Linux
ifeq ($(shell uname), Linux)
generate_input:
	@gnome-terminal -- bash -c "cd $(PWD) && g++ input_generator.c && ./a.out 20";	
execute_pipeline:
	@gnome-terminal -- bash -c "cd $(PWD)/pipeline && mpicc -o pipelineFloyd pipelineFloyd.c -lm && mpirun -np 4 --oversubscribe ./pipelineFloyd";
execute_parallel:
	@gnome-terminal -- bash -c "cd $(PWD)/parallel && mpicc -o parallelFloyd parallelFloyd.c -lm && mpirun -np 4 --oversubscribe ./parallelFloyd";
execute_serial:
	@gnome-terminal -- bash -c "cd $(PWD)/serial && mpicc -o serialFloyd serialFloyd.c -lm && mpirun -np 1 --oversubscribe ./serialFloyd";
endif

# macOS
ifeq ($(shell uname), Darwin)
generate_input:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD) && g++-13 input_generator.c && ./a.out 20"'	
execute_pipeline:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/pipeline && mpicc -o pipelineFloyd pipelineFloyd.c -lm && mpirun -np 4 --oversubscribe ./pipelineFloyd"'
execute_parallel:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/parallel && mpicc -o parallelFloyd parallelFloyd.c -lm && mpirun -np 4 --oversubscribe ./parallelFloyd"'
execute_serial:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/serial && mpicc -o serialFloyd serialFloyd.c -lm && mpirun -np 1 --oversubscribe ./serialFloyd"'		
endif

