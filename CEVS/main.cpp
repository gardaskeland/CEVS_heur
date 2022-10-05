#include "inout.h"
#include "utility/gard_random.h"
#include <vector>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    int num_iterations = -1;
    int num_runs = -1;
    string filename = "";
    vector<bool> required_arguments = {false, false, false};
    
    if (strcmp(argv[1], "-help") == 0) {
        cout << "Must give arguments -iterations, -runs and -file. No other arguments accepted. \n" <<
                "Example: ./cevs -iterations 10000 -runs 1 -file example_files/testfile.gr \n" <<
                "Graphs in files with the gr-file format and gml-format are accepted. Examples are included in example_files.\n";
                "Results will be written to to the results-folder.";
        exit(0);
    }
    
    for (int i = 1; i < argc; i+=2) {
        if (strcmp(argv[i], "-iterations") == 0) {
            cout << argv[i+1] << "\n";
            num_iterations = stoi(argv[i+1]);
            if (num_iterations < 1) {
                cout << "Number of iterations need to be larger than 0.\n";
                exit(0);
            }
            required_arguments[0] = true;
        }
        else if (strcmp(argv[i], "-runs") == 0) {
            cout << argv[i+1] << "\n";
            num_runs = stoi(argv[i+1]);
            if (num_runs < 1) {
                cout << "Number of runs need to be at least 1.\n";
                exit(0);
            }
            required_arguments[1] = true;
        }
        else if (strcmp(argv[i], "-file") == 0) {
            filename = argv[i+1];
            //check if file exists?
            required_arguments[2] = true;
        }
        else {
            cout << "Unrecognised argument:" << "argv[i]" << endl;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (!required_arguments[i]) {
            cout << "Not all required arguments given. Make sure arguments for iterations, runs and filename are given. Type ./cevs -help for help.\n";
            exit(0);
        }
    }

    int full_stop_pos;
    for (int i = filename.size() - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            full_stop_pos = i;
            break;
        }
        if (i == 0) {
            cout << "Did not find file postfix of input file.";
        }
    }
    string postfix = filename.substr(full_stop_pos+1);
    if (strcmp("gml", postfix.c_str()) == 0) {
        run_alns_on_gml(num_iterations, num_runs, filename);
    }
    if (strcmp("gr", postfix.c_str()) == 0) {
        run_alns_on_gz(num_iterations, num_runs, filename);
    }
}