import subprocess
import sys

def run_test(test):
    infilename = "input{}.txt".format(test)
    outfilename = "output{}.txt".format(test)
    input = []
    output = []

    # gather input
    with open(infilename, 'r') as infile:
        input = infile.readlines()

    # gather expected output
    with open(outfilename, 'r') as outfile:
        output = outfile.readlines()

    # reverse the ouput so that we can just pop()
    output = output[::-1]

    # remove extraneous newlines and comments
    output = [o for o in output if o.strip() != "" and not o.startswith("#")]

    # run the program
    program = subprocess.Popen('./prog2', bufsize=-1, stdin=subprocess.PIPE, stdout=subprocess.PIPE, universal_newlines=True)

    # run the interactive test
    for line in input:
        if line.strip() == "":
            # you can wreck this driver by not putting a newline in your output
            # TODO: external timeout mechanism for actual grading
            actual_output = program.stdout.readline().strip()
            print("Received: {}".format(actual_output))
            desired_output = output.pop().strip()
            if actual_output != desired_output:
                print("BAD!\nDesired: {}".format(desired_output))
                program.kill()
                return False
        else:
            print("Sending: {}".format(line.strip()))
            program.stdin.write(line)
            program.stdin.flush()
            if line.strip() == "END":
                # not going to wait on exit
                try:
                    if not program.poll():
                        program.kill()
                except ignore:
                    # suppress all exceptions
                    pass
                return True

if __name__ == "__main__":
    test_number = "01"
    if len(sys.argv) == 2:
        test_number = sys.argv[1]
    if run_test(test_number):
        print("\nCongratulations, it worked.")
