# Makefile

# Linux
ifeq ($(shell uname), Linux)
execute_pipeline:
	@gnome-terminal -- bash -c "cd $(PWD)/pipeline && mpicc -o main main.c -lm && mpirun -np 4 ./main";
execute_parallel:
	@gnome-terminal -- bash -c "cd $(PWD)/parallel && mpicc -o main main.c -lm && mpirun -np 4 ./main";	
endif

# macOS
ifeq ($(shell uname), Darwin)
execute_pipeline:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/pipeline && mpicc -o main main.c -lm && mpirun -np 4 ./main"'
execute_parallel:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/parallel && mpicc -o main main.c -lm && mpirun -np 9 --oversubscribe ./main"'
execute_serial:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/serial && mpicc -o main main.c -lm && mpirun -np 1 ./main"'		
endif

