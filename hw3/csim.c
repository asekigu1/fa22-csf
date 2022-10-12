#include <stdio.h>
#include <ctype.h>
#include <iostream>

using std::cout; using std::cin; using std::endl;
using std::cerr;


int main(int argc, char * argv[]) {
    
    // validate arguments
    if (argc != 6 && argc != 7) { 
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    //  validate number of sets is a positive power of 2
    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int num_bytes = atoi(argv[3]);
    if () {
        cerr << "Invalid parameter in validation of num_sets to num_bytes";
        return 1;
    }

    //  validate write-allocate and write-through modes
    bool write_allocate;
    bool write_through;
    
    if (argv[4] == "write-allocate") {
        write_allocate = true;
    } else if (argv[4] == "no-write-allocate") {
        write_allocate = false;
    } else {
        cerr << "Invalid parameter in write-allocate";
        return 1;
    }
    if (argv[5] == "write-through") {
        write_through = true;
    } else if (argv[5] == "write-back") {
        write_through = false;
    } else {
        cerr << "Invalid parameter in write-through";
        return 1;
    }
    if (!write_allocate && !write_thru) {
        cerr << "Cannot pass no-write-allocate and write-back";
        return 1;
    }

    //  validate eviction
    bool lru;
    if (argv[6] == "lru") {
        lru = true;
    } else if (argv[6] == "fifo") {
        lru = false;
    } else {
        cerr << "Invalid parameter in eviction";
        return 1;
    }
  
  

    return 0;
}
