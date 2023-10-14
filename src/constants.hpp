//
//  constants.h
//  NIPD_ABM
//
//  Created by Aldric Labarthe on 12/10/2023.
//

#ifndef constants_hpp
#define constants_hpp

#include <iostream>
#include <array>
#include <cmath>

#include <Matrix.hpp>
#include <NeuralNetwork.hpp>
#include <GeneticAlgorithm.hpp>


#define POP_SIZE 30
#define AGENT_MEMORY_DURATION 3
#define COOPERATE 1
#define MAX_AI_TICKS 5000
#define GLOBAL_REWARD_PER_COOPERATIVE_AGENT 2
#define COOPERATION_COST 1

#define AGENTS_POP_SIZE (POP_SIZE*POP_SIZE)/4
#define NN_INPUT_LEN 2+AGENT_MEMORY_DURATION*2


#endif /* constants_hpp */
