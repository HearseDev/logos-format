#include <array>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

// a tpyical formatter will read the contents of the file from stdin
// and then format and spit out the formatted contents to stdout
int main(int argc, const char **argv) {
  string currentLine;      // currentLine of the contents
  vector<string> inputVec; // dynamic array to store the files line by line
  // filters that will be used to check against every line
  array<string, 10> filterArr = {"%hook",  "%end",     "%log",  "%orig",
                                 "%class", "%new",     "%init", "%ctor",
                                 "%group", "%subclass"};
  bool skipline = false;
  // retrieve contents from stdin
  // store contents of file in a dynamic array with the filters being turned off
  // by clang-format
  while (getline(cin, currentLine)) {
    for (const string &filter : filterArr) {
      if (currentLine.find(filter) != string::npos) {
        inputVec.emplace_back("/* clang-format off */");
        inputVec.push_back(currentLine);
        inputVec.emplace_back("/* clang-format on */");
        skipline = true;
      }
    }
    if (!skipline) {
      inputVec.push_back(currentLine);
    }
    skipline = false;
  }

  string command = "clang-format ";
  string cfargs;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      cfargs += argv[i];
      cfargs += " ";
    }
    command += cfargs;
  }
  command += ">> ";
  // get name/path of a temporary file to write to
  string tmpdir = filesystem::temp_directory_path();
  string tmppath = tmpdir + "/logos-format_XXXXXX";

  char tmp[tmppath.length()];
  strcpy(tmp, tmppath.c_str());
  int fd = mkstemp(tmp);
  const string fullcmd = command + tmp;
  // run clang-format and store output in a temporary file
  FILE *pFile = popen(fullcmd.c_str(), "w");
  // Handle error
  if (pFile == nullptr) {
    exit(-1);
  }

  // Send to clang-format stdin
  string s;
  for (const auto &piece : inputVec) {
    s += piece;
    s += "\n";
  }
  const char *data = s.c_str();
  size_t nNumWritten = fwrite(data, 1, strlen(data), pFile);
  pclose(pFile);
  pFile = nullptr;
  delete pFile;

  ifstream tempFile(tmp); // create ifstream for the tempFile

  // clear both inputVec and currentLine for reuse
  currentLine.clear();
  inputVec.clear();

  // read contents of tempfile line by line in a dynamic array
  // but remove the added clang-format on/off lines to the file
  if (tempFile.is_open()) {
    while (getline(tempFile, currentLine)) {
      if (currentLine.find("/* clang-format on */") == string::npos &&
          currentLine.find("/* clang-format off */") == string::npos) {
        inputVec.emplace_back(currentLine);
      }
    }
  }
  // this is the formatted output
  // we just have to send it to stdout
  for (auto &s : inputVec) {
    cout << s << endl;
  }

  tempFile.clear();
  tempFile.close();
  // close file descriptor
  // remove tmp file created by clang-format after the
  // formatting has completed
  close(fd);
  remove(tmp);
  return 0;
}
