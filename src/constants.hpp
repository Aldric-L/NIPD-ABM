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
#include <cassert>
#include <utility>

#define AGENTS_POP_SIZE 300
#define AGENT_MEMORY_DURATION 3
#define COOPERATE 1
#define MAX_AI_TICKS 5000
#define GLOBAL_REWARD_PER_COOPERATIVE_AGENT 2
#define COOPERATION_COST 1

#define GRID_WIDTH 50
#define PIXEL_SIZE 10
#define GRID_PIXEL_WIDTH GRID_WIDTH * PIXEL_SIZE
#define GRID_PIXEL_SIZE (GRID_PIXEL_WIDTH)*(GRID_PIXEL_WIDTH)

#define NN_INPUT_LEN 2+AGENT_MEMORY_DURATION*2
#define POP_LOG 0


#define AKML_NEURAL_LAYER_NB 4
#define AKML_NN_STRUCTURE { NN_INPUT_LEN, NN_INPUT_LEN*2, NN_INPUT_LEN, 1 }

#include <AKML.hpp>
#include <GeneticAlgorithm.cpp>

#endif /* constants_hpp */
