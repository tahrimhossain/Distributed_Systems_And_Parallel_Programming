import subprocess


def compare_files(file1_path, file2_path):
    # Read the contents of the first file
    with open(file1_path, 'r') as file1:
        content1 = file1.read()

    # Read the contents of the second file
    with open(file2_path, 'r') as file2:
        content2 = file2.read()

    # Check if the files have the same content
    return content1 == content2


def clean():
    clean_serial = 'cd ../serial && rm -rf stat.txt'
    clean_parallel = 'cd ../parallel && rm -rf stat.txt'
    clean_pipeline = 'cd ../pipeline && rm -rf stat.txt'
    subprocess.run(clean_serial, shell=True, capture_output=True, text=True)
    subprocess.run(clean_parallel, shell=True, capture_output=True, text=True)
    subprocess.run(clean_pipeline, shell=True, capture_output=True, text=True)

def generateInput(n):
    command = 'cd .. && g++-13 input_generator.c && ./a.out '+str(n)
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    print("Output:", result.stdout)
    print("Return Code:", result.returncode)

def runSerial(n):
    command = 'cd ../serial && mpicc -o main main.c -lm && mpirun -np 1 ./main '+str(n)
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    print("Output:", result.stdout)
    print("Return Code:", result.returncode)


def runParallel(n,p):
    command = 'cd ../parallel && mpicc -o main main.c -lm && mpirun --oversubscribe -np '+str(p)+' ./main '+str(n)
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    print("Output:", result.stdout)
    print("Return Code:", result.returncode)


def runPipeline(n,p):
    command = 'cd ../pipeline && mpicc -o main main.c -lm && mpirun --oversubscribe -np '+str(p)+' ./main '+str(n)
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    print("Output:", result.stdout)
    print("Return Code:", result.returncode)



n = [20,30,40]
p = [4,9,16]
clean()
for index in range(len(n)):
    generateInput(n[index])
    runSerial(n[index])
    runParallel(n[index],p[index])
    runPipeline(n[index],p[index])
    print(compare_files('../serial/output.txt','../parallel/output.txt'))
    print(compare_files('../serial/output.txt','../pipeline/output.txt'))

