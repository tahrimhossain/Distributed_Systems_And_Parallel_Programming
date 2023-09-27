# Makefile

# Linux
ifeq ($(shell uname), Linux)
execute_code:
	@gnome-terminal -- bash -c "cd $(PWD)/server && g++ server.cpp -pthread -o server && ./server"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/client && g++ client.cpp -o client && ./client"

execute_test_normal:
	@gnome-terminal -- bash -c "cd $(PWD)/server && g++ server.cpp -pthread -o server && ./server"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Normal"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Normal"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Normal"

execute_test_random:
	@gnome-terminal -- bash -c "cd $(PWD)/server && g++ server.cpp -pthread -o server && ./server"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Random"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Random"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/testcases && g++ client.cpp -o client && ./client Random"
endif

# macOS
ifeq ($(shell uname), Darwin)
execute_code:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/server && g++-13 server.cpp -pthread -o server && ./server"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/client && g++-13 client.cpp -o client && sleep 3 && ./client"'
execute_test_normal:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/server && g++-13 server.cpp -pthread -o server && ./server"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Normal"'
execute_test_random:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/server && g++-13 server.cpp -pthread -o server && ./server"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Random"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Random"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/testcases && g++-13 client.cpp -o client && sleep 3 && ./client Random"'
endif

