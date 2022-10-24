#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bitset>
#include "helper_functions.h"


using std::cout; using std::cin; using std::endl;
using std::cerr; using std::vector; using std::string; using std::stringstream;





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
    
    if ((is_power_of_2(num_sets) == 0) || (is_power_of_2(num_blocks) == 0 || (num_blocks < 4)))  {
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
    
    Cache cache;
    vector<Set> v(num_sets);
    cache.sets = v;
    
    for (vector<Set>::iterator it = cache.sets.begin(); it != cache.sets.end(); ++it) {
        
        vector<Slot> v1(num_blocks);
        
        it->slots = v1;
        
    }
    cout << "# of sets " << cache.sets[0].slots.size() << endl; 
    
    int count = 0;
    for (size_t i = 0; i < cache.sets.size(); i++) {
        cout << "-----" << endl;
        for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
            cout << count << endl;
            count++;
        }
        
       
    }
    
    cout << "Count " << count << endl;
    
    


    string line;
    
    while (std::getline(cin, line)){
        //get if load or store
        string operation;
        stringstream ss(line);
        ss >> operation;
        //get the address
        //cout << operation << endl;
        string address;
        ss >> address;
        //convert to number
        stringstream s2;
        s2 << std::hex << address;
        uint32_t num;
        s2 >> num;
        uint32_t address_tag= get_tag(num, num_sets, num_blocks);
        uint32_t address_index = get_index(num, num_sets, num_blocks);
            

    if (operation == "l") {
    int read_hit = 0;
    for (size_t i = 0; i < cache.sets.size(); i++) {
        
        for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
            Slot current_slot = cache.sets[i].slots[j];
            
            if ((current_slot.valid == true) && (current_slot.tag == address_tag)) {
                //read hit
                current_slot.index = address_index;
                read_hit = 1;


            }

        
        }
    }

        if (read_hit == 0) {
            for (size_t i = 0; i < cache.sets.size(); i++) {
                for (size_t j = 0; j < cache.sets[i].slots.size(); j++) {
                    Slot current_slot = cache.sets[i].slots[j];
            
                    if (current_slot.valid == false) {
                    //read miss
                        current_slot.valid = true;
                        current_slot.tag = address_tag;
                        current_slot.index = address_index;
                


                    }
            }   }
        
        }       

    
    
        
       
    }


        
        
       
        
        
        
    }





  
    
  

    return 0;
}
