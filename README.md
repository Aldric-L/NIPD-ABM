# N-Iterated Prisoner Dilemma (NIPD) - A agent based simulation

The main purpose of this project is to reproduce a classical model in game theory : the n-iterated prisonner dilemma (I used as a model the work of CHIONG and KIRLEY (2009)). This work provides an implementation of the n-iterated prisonner dilemma with a evolutionnary approach for strategies selection. Contrary to CHIONG and KIRLEY (2009), agents in this simulation have a full scope (they play all against other, and there is no subgroups). As expected, the simulation produces the classical theoretical results of pure defection. 

## Results and screeshot

![summary plot](https://raw.githubusercontent.com/Aldric-L/NIPD-ABM/master/results/plot1_cooperator_rate.png)
![screenshot](https://raw.githubusercontent.com/Aldric-L/NIPD-ABM/master/results/screenshot.png)


## Technical informations

The simulation is implemented with my custom made framework AKML, which allows us to use NeuralNetworks and Genetic algorithm, and with the open-source framework SFML. Because the SFML is not always easy to build, the results are extracted in CSV.


