#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Agent.hpp"

#include <AgentBasedUtilities/BaseAgent.cpp>
#include <AgentBasedUtilities/Save.hpp>
#include <AgentBasedUtilities/CSV_Saver.hpp>

int main()
{
    std::cout << "Hello terrible world \n";
    
    auto window = sf::RenderWindow{ { GRID_PIXEL_WIDTH, GRID_PIXEL_WIDTH }, "SFML Simulation" };
    window.setFramerateLimit(25);
    
    akml::BaseGeneticAlgorithm<4, NN_INPUT_LEN, 1, AGENTS_POP_SIZE> ga;
    ga.setNewGenerationRates(0, 0.9);
    
    std::array <std::array <Agent* , GRID_WIDTH>, GRID_WIDTH> grid;
    std::array <Agent* , AGENTS_POP_SIZE> agents;
    
    for (int rows(0); rows < GRID_WIDTH; rows++){
        for (int columns(0); columns < GRID_WIDTH; columns++){
            grid[rows][columns] = nullptr;
        }
    }
    

    akml::Save<3, unsigned long int, float, unsigned long int>* currentSave;
    akml::Save<3, unsigned long int, float, unsigned long int>::default_parameters_name = {{ "nb_cooperators", "cooperator_rate", "global_welfare"}};
    akml::CSV_Saver<akml::Save<3, unsigned long int, float, unsigned long int>> memoryManager;
    
    unsigned short int agentsAssignated = 0;
    while (agentsAssignated < AGENTS_POP_SIZE) {
        unsigned short int x (rand() % (GRID_WIDTH)), y(rand() % (GRID_WIDTH));
        if (grid[x][y] != nullptr && x > 1){ x=x-1;}
        if (grid[x][y] != nullptr && x < GRID_WIDTH-1){ x=x+2;}
        if (grid[x][y] != nullptr && y > 1){ y=y-1;}
        if (grid[x][y] != nullptr && y < GRID_WIDTH-1){ y=y+2;}
        if (grid[x][y] == nullptr){
            agents[agentsAssignated] = new Agent (agentsAssignated, (rand() % 2)==1, { COOPERATE, COOPERATE, COOPERATE}, { 1, 1, 1}, ga.getNetworksPopulation()[agentsAssignated]);
            grid[x][y] = agents[agentsAssignated];;
            agents[agentsAssignated]->editPos(std::make_pair(x, y));
            agentsAssignated++;
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
            
            unsigned long int global_welfare = coop_agent_counter*GLOBAL_REWARD_PER_COOPERATIVE_AGENT;
            currentSave = new akml::Save<3, unsigned long int, float, unsigned long int> (coop_agent_counter, (float)coop_agent_counter/AGENTS_POP_SIZE, global_welfare);
            
            memoryManager.addSave(currentSave);
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                agents[agent_id]->setPreviousGlobalWelfare(global_welfare/std::pow(GLOBAL_REWARD_PER_COOPERATIVE_AGENT, AGENTS_POP_SIZE));
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
            ga.generateNewGeneration(MSE, generationsCounter/*, merging_instructions*/);
            
            for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                agents[agent_id]->editNNPointer(ga.getNetworksPopulation()[agent_id]);
            }
        }
        
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                for (std::size_t agent_id(0); agent_id < AGENTS_POP_SIZE; agent_id++){
                        if (agents[agent_id] != nullptr)
                            delete agents[agent_id];
                }
                if (POP_LOG)
                    memoryManager.saveToCSV();
                window.close();
            }
        }

        window.clear();
        
        for (int rows(0); rows < GRID_WIDTH; rows++){
            for (int columns(0); columns < GRID_WIDTH; columns++){
                if (grid[rows][columns] != nullptr){
                    sf::RectangleShape agent_visual(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    if (grid[rows][columns]->will_cooperate)
                        agent_visual.setFillColor(sf::Color::White);
                    else
                        agent_visual.setFillColor(sf::Color::Red);
                    agent_visual.setPosition(rows*PIXEL_SIZE, columns*PIXEL_SIZE);
                    window.draw(agent_visual);
                }
            }
        }
        
        window.display();
    }
}
