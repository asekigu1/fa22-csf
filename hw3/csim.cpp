#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>

using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector;

int is_power_of_2(int n){
    //while n is even keep on dividing by 2
    while (n % 2 == 0) {
        n = n /2;
    }
    //n is now odd, if it is 1 then it is a power of 2, any other odd number means it wasn't
    if (n == 1){
        return 1;
    }
    else {
        return 0;
    }

}

class Slot {
    uint32_t tag;
    bool valid;
    uint32_t time_stamp;

};

class Set {
    vector<Slot> slots;
};

class Cache {
    vector<Set> sets;
};




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
    if ((is_power_of_2(num_sets) == 0) || (is_power_of_2(num_blocks == 0) || (num_blocks < 4)))  {
        cerr << "Invalid parameter in validation of num_sets to num_bytes";
        return 1;
    }

    //  validate write-allocate and write-through modes
    bool write_allocate;
    bool write_through;
    
    if (strcmp(argv[4],"write-allocate") == 0) {
        write_allocate = true;
    } else if (strcmp(argv[4],"no-write-allocate") == 0) {
        write_allocate = false;
    } else {
        cerr << "Invalid parameter in write-allocate";
        return 1;
    }
    if (strcmp(argv[5],"write-through") == 0) {
        write_through = true;
    } else if (strcmp(argv[5], "write-back") == 0) {
        write_through = false;
    } else {
        cerr << "Invalid parameter in write-through";
        return 1;
    }
    if (!write_allocate && !write_through) {
        cerr << "Cannot pass no-write-allocate and write-back";
        return 1;
    }

    //  validate eviction
    bool lru;
    if (strcmp(argv[6],"lru") == 0) {
        lru = true;
    } else if (strcmp(argv[6],"fifo") == 0) {
        lru = false;
    } else {
        cerr << "Invalid parameter in eviction";
        return 1;
    }
    

  
    
  

    return 0;
}
