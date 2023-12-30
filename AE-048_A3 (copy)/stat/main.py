import matplotlib.pyplot as plt

def parse_data(file_path):
    # Initialize an empty dictionary to store the parsed data
    data = {}

    # Read the file and parse the content
    with open(file_path, 'r') as file:
        lines = file.readlines()

    for line in lines:
        # Split the line into key and value parts
        parts = line.strip().split()

        # Convert key and inner_key to appropriate types
        num_of_intervals = int(parts[0])
        num_of_processors = int(parts[1])
        runtime = float(parts[2])

        # Create or update the nested dictionary
        if num_of_intervals not in data:
            data[num_of_intervals] = {}
        data[num_of_intervals][num_of_processors] = runtime

    return data

# Example usage
def showGraph(file_path):
    parallel_spawn_data = parse_data(file_path)
    serial_data = parse_data('../serial/stat.txt')
    for num_of_intervals, inner_data in parallel_spawn_data.items():
        for num_of_processors,_ in inner_data.items():
            inner_data[num_of_processors] = serial_data[num_of_intervals][1]/inner_data[num_of_processors]
        inner_keys = list(inner_data.keys())
        speedup = list(inner_data.values())
        plt.plot(inner_keys, speedup, label=f'Number of intervals {num_of_intervals}')

    plt.xlabel('Number of processors')
    plt.ylabel('Speedup')
    plt.title('Number of intervals')
    plt.legend()
    plt.show()


showGraph('../parallel_spawn/stat.txt')
showGraph('../parallel/stat.txt')