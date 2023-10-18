//
//  Agent.cpp
//  NIPD_ABM
//
//  Created by Aldric Labarthe on 12/10/2023.
//

#include "Agent.hpp"

void Agent::editNNPointer(akml::NeuralNetwork<4>* NN) {
    brainNN = new akml::NeuralNetwork<4>* (NN);
}

akml::NeuralNetwork<4>* Agent::getNNAccess() { return *brainNN; }

void Agent::setPreviousGlobalWelfare(unsigned long int pGlobalWelfare) {
    previousGlobalWelfare = pGlobalWelfare;
}

void Agent::setCooperationCost(unsigned short int cCost) {
    cooperationCost = cCost;
}

void Agent::makedecision() {
    akml::Matrix<float, NN_INPUT_LEN, 1> input;
    input(1,1) = previousGlobalWelfare;
    input(2,1) = cooperationCost;
    for (int i(1); i <= AGENT_MEMORY_DURATION; i++){
        input(2+i,1) = (float)memory[i-1];
    }
    for (int i(1); i <= AGENT_MEMORY_DURATION; i++){
        input(2+AGENT_MEMORY_DURATION+i,1) = coop_memory[i-1];
    }
    akml::Matrix <float, 1, 1> localoutput = *((*brainNN)->template process<NN_INPUT_LEN, 1>(input));
    will_cooperate = (localoutput.read(1,1) >= 0.5);
    //std::cout << "Will cooperate ? " << will_cooperate << std::endl;
}

void Agent::createMemory(const float coop_rate){
    for (int i(0); i < AGENT_MEMORY_DURATION-1; i++){
        memory[i] = memory[i+1];
        coop_memory[i] = coop_memory[i+1];
    }
    memory[AGENT_MEMORY_DURATION-1] = will_cooperate;
    coop_memory[AGENT_MEMORY_DURATION-1] = coop_rate;
}

std::pair<int, int> Agent::getPos(){ return pos; }
void Agent::editPos(const std::pair<int, int> &new_pos) { pos = new_pos; };
