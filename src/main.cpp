#include <SFML/Graphics.hpp>
#include <Matrix.cpp>
#include <NeuralNetwork.cpp>
#include <GeneticAlgorithm.cpp>

#include "constants.hpp"
#include "Agent.hpp"
#include "Saver.hpp"


int main()
{
    std::cout << "Hello terrible world";
    Saver memoryManager;
    
    auto window = sf::RenderWindow{ { POP_SIZE*5, POP_SIZE*5 }, "SFML Simulation" };
    window.setFramerateLimit(25);
    
    // AI Initialization
    std::function<void(akml::NeuralNetwork<4>&)> init_instructions = [](akml::NeuralNetwork<4>& neuralnet) {
        neuralnet.setFirstLayer<NN_INPUT_LEN>();
        neuralnet.setLayer<(NN_INPUT_LEN)*2, NN_INPUT_LEN>(2);
        neuralnet.setLayer<NN_INPUT_LEN, (NN_INPUT_LEN)*2>(3);
        neuralnet.setLayer<1, NN_INPUT_LEN>(4);
    };
    
    std::function<void(akml::NeuralNetwork<4>&, akml::NeuralNetwork<4>*, akml::NeuralNetwork<4>*)> merging_instructions =
    [](akml::NeuralNetwork<4>& child, akml::NeuralNetwork<4>* parent1, akml::NeuralNetwork<4>* parent2) {
        if (parent1 == parent2)
            akml::GeneticAlgorithmMethods::mergeLayers<akml::NeuralNetwork<4>,
            akml::NeuralLayer<NN_INPUT_LEN, 1>>(1, parent1, parent2, child);
        akml::GeneticAlgorithmMethods::mergeLayers<akml::NeuralNetwork<4>,
        akml::NeuralLayer<(NN_INPUT_LEN)*2, NN_INPUT_LEN>>(2, parent1, parent2, child);
        akml::GeneticAlgorithmMethods::mergeLayers<akml::NeuralNetwork<4>,
        akml::NeuralLayer<NN_INPUT_LEN, NN_INPUT_LEN*2>>(3, parent1, parent2, child);
        akml::GeneticAlgorithmMethods::mergeLayers<akml::NeuralNetwork<4>,
        akml::NeuralLayer<1, NN_INPUT_LEN>>(4, parent1, parent2, child);
    };
    
    akml::BaseGeneticAlgorithm<4, NN_INPUT_LEN, 1, (POP_SIZE*POP_SIZE)/4> ga (init_instructions);
    
    std::array <std::array <Agent* , POP_SIZE>, POP_SIZE> grid;
    std::array <Agent* , AGENTS_POP_SIZE> agents;
    unsigned int id(0);
    for (int rows(0); rows < POP_SIZE; rows++){
        for (int columns(0); columns < POP_SIZE; columns++){
            if ((rows)%2 == 0 && (columns)%2 == 0){
                unsigned short int d_r ((rand() % 2)==1), d_c((rand() % 2)==1);
                grid[rows+d_r][columns-d_c] = new Agent (id, (rand() % 2)==1, { COOPERATE, COOPERATE, COOPERATE}, { 0.5, 0.5, 0.5}, ga.getNetworksPopulation()[id]);
                agents[id] = grid[rows+d_r][columns-d_c];
                if (d_r != 0 || d_c != 0)
                    grid[rows][columns] = nullptr;
                id = id+1;
            }
            else{
                grid[rows][columns] = nullptr;
            }
        }
    }
        
    
    unsigned long long int AI_ticks = 0;
    std::size_t generationsCounter(1);
    while (window.isOpen())
    {
        if (AI_ticks%4 == 0 && AI_ticks<(MAX_AI_TICKS)*4){
            unsigned int coop_agent_counter (0);
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                if (agents[agent_id]->will_cooperate)
                    coop_agent_counter++;
            }
            Save currentSave;
            currentSave.nb_cooperators = coop_agent_counter;
            //currentSave.global_welfare = coop_agent_counter*GLOBAL_REWARD_PER_COOPERATIVE_AGENT;
            currentSave.global_welfare = std::pow(GLOBAL_REWARD_PER_COOPERATIVE_AGENT, coop_agent_counter);
            currentSave.cooperator_rate = (float)coop_agent_counter/AGENTS_POP_SIZE;
            memoryManager.addSave(currentSave);
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                agents[agent_id]->setPreviousGlobalWelfare(currentSave.global_welfare);
                agents[agent_id]->setCooperationCost(COOPERATION_COST);
                agents[agent_id]->makedecision();
            }
            coop_agent_counter = 0;
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                if (agents[agent_id]->will_cooperate)
                    coop_agent_counter++;
            }
            std::array< std::pair<float, akml::NeuralNetwork<4>*>, AGENTS_POP_SIZE> MSE;
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                MSE[agent_id] = std::pair(coop_agent_counter*GLOBAL_REWARD_PER_COOPERATIVE_AGENT - (int)agents[agent_id]->will_cooperate * COOPERATION_COST, agents[agent_id]->getNNAccess());
                
                
                agents[agent_id]->createMemory(coop_agent_counter/AGENTS_POP_SIZE);
            }
            
            std::sort(MSE.begin(), MSE.end());
            generationsCounter++;
            ga.generateNewGeneration(MSE, generationsCounter, merging_instructions);
            
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                agents[agent_id]->editNNPointer(ga.getNetworksPopulation()[agent_id]);
            }
        }
        
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                for (int rows(0); rows < POP_SIZE; rows++){
                    for (int columns(0); columns < POP_SIZE; columns++){
                        if (grid[rows][columns] != nullptr)
                            delete grid[rows][columns];
                    }
                }
                memoryManager.saveToCSV();
                window.close();
            }
        }

        window.clear();
        
        for (int rows(0); rows < POP_SIZE; rows++){
            for (int columns(0); columns < POP_SIZE; columns++){
                if (grid[rows][columns] != nullptr){
                    sf::RectangleShape agent_visual(sf::Vector2f(5, 5));
                    if (grid[rows][columns]->will_cooperate)
                        agent_visual.setFillColor(sf::Color::White);
                    else
                        agent_visual.setFillColor(sf::Color::Red);
                    agent_visual.setPosition(rows*5, columns*5);
                    window.draw(agent_visual);
                }
            }
        }
        
        window.display();
    }
}
