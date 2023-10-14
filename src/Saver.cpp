//
//  Saver.cpp
//  NIPD_ABM
//
//  Created by Aldric Labarthe on 12/10/2023.
//

#include "Saver.hpp"

void Saver::addSave(Save iteration){
    if (memory.size() == 0)
        memory.push_back(std::make_pair(1, iteration));
    else
        memory.push_back(std::make_pair(memory.back().first+1, iteration));
}

void Saver::saveToCSV(){
    std::ofstream file;
    file.open ("data.csv");
    for (int it(0); it < memory.size(); it++){
        file << it << "," << memory[it].second.global_welfare << ",";
        file << std::fixed << std::setprecision(3) << memory[it].second.cooperator_rate;
        file << "," << memory[it].second.nb_cooperators <<"\n";
        
    }
    file.close();
}
