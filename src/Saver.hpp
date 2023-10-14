//
//  Saver.hpp
//  NIPD_ABM
//
//  Created by Aldric Labarthe on 12/10/2023.
//

#ifndef Saver_hpp
#define Saver_hpp

#include "constants.hpp"
#include <vector>
#include <fstream>
#include <iomanip>

struct Save {
    unsigned long int nb_cooperators;
    float cooperator_rate;
    unsigned long int global_welfare;
};

class Saver {
protected:
    std::vector<std::pair<std::size_t, Save>> memory;
    
public:
    void addSave(Save iteration);
    void saveToCSV();
};


#endif /* Saver_hpp */
