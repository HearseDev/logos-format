import sys
from subprocess import Popen, PIPE, STDOUT

filterList = ["%hook","%end","%log","%orig","%class","%new","%init", "%ctor","%group","%subclass"]


fileContentsList = sys.stdin.read().splitlines()

newList = []
skipLine = False

for line in fileContentsList:
    for filter in filterList:
        if line.find(filter) != -1:
            newList.append("/* clang-format off */")
            newList.append(line)
            newList.append("/* clang-format on */")
            skipLine = True;
    if skipLine == False:
      newList.append(line);
    skipLine = False;

command = ["clang-format"] + sys.argv[1:]
process = Popen(command, stdout=PIPE, stderr=None, stdin=PIPE)
stdoutData = process.communicate(input= "\n".join(newList).encode())[0]

refinedArr = stdoutData.decode().splitlines()
str = ""
for line in refinedArr:
    if line.find("/* clang-format off */") == -1 and line.find("/* clang-format on */") == -1:
        print(line)
