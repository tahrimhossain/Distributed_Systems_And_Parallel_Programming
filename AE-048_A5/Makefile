# Makefile

# Linux
ifeq ($(shell uname), Linux)
execute_code:
	@gnome-terminal -- bash -c "cd $(PWD)/Server && javac ServerMain.java && java ServerMain"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/Client && javac ClientVigenere.java && java ClientVigenere"; \
	sleep 3; \
	gnome-terminal -- bash -c "cd $(PWD)/Client && javac ClientCaesar.java && java ClientCaesar"
endif

# macOS
ifeq ($(shell uname), Darwin)
execute_code:
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/Server && javac ServerMain.java && java ServerMain"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/Client && javac ClientVigenere.java && java ClientVigenere"'
	@sleep 3
	@osascript -e 'tell app "Terminal" to do script "cd $(PWD)/Client && javac ClientCaesar.java && java ClientCaesar"'
endif

