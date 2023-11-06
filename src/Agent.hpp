//
//  Agent.hpp
//  NIPD_ABM
//
//  Created by Aldric Labarthe on 12/10/2023.
//

#ifndef Agent_hpp
#define Agent_hpp

#include "constants.hpp"

class Agent : public akml::BaseAgent {
protected:
    std::array <bool, AGENT_MEMORY_DURATION> memory;
    std::array <float, AGENT_MEMORY_DURATION> coop_memory;
    std::pair<int, int> pos;
    unsigned long int previousGlobalWelfare = 0;
    unsigned short int cooperationCost = 0;
    
public:
    const bool altruist;
    const unsigned int id;
    bool will_cooperate;

    Agent(const unsigned int id, const bool is_altruist, const std::array <bool, AGENT_MEMORY_DURATION> memory_initialization, const std::array <float, AGENT_MEMORY_DURATION> coop_memory_initialization, akml::NeuralNetwork<4>* NN) : akml::BaseAgent(id, NN), id(id), altruist(is_altruist), memory(memory_initialization), coop_memory(coop_memory_initialization) {
        will_cooperate = true;
    };
    
    void setPreviousGlobalWelfare(unsigned long int pGlobalWelfare);
    void setCooperationCost(unsigned short int cCost);
    void makedecision();
    void createMemory(const float coop_rate);
    
    std::pair<int, int> getPos();
    void editPos(const std::pair<int, int>& new_pos);

};

#endif /* Agent_hpp */
