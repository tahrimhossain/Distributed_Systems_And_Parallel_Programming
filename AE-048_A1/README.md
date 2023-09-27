## Usage

### Running the Main Code

To execute the main functionality of the program, run the following command:

```bash
make execute_code
```

This command will run the main code of the program, performing its primary tasks.

### Testing with Correct Responses

To test the program with three instances of the client, where each version sends only the correct responses, use the following command:

```bash
make execute_test_normal
```

This command will launch three instances of the client, simulating normal behavior with correct responses.

### Testing with Random Responses and Random Jokes

To test the program with three instances of the client, where each version sends random responses to the server and listens to a random number of jokes, execute the following command:

```bash
make execute_test_random
```

This command will run three instances of the client with random behavior, including sending random responses and listening to a random number of jokes from the server.