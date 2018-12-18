# bNEAT
[![Build Status](https://travis-ci.org/sheldonkwoodward/bNEAT.svg?branch=master)](https://travis-ci.org/sheldonkwoodward/bNEAT)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)

bNEAT is an implementation of the NEAT (nerual evolution of augmenting topologies) machine-learning methodology and was created by Sheldon Woodward and Charles Lambert. 

The learning algorithm is currently implemented to teach itself how to play Snake. In the future, the neural network and trainer can be abstracted and used for generic ML purposes.

Click the picture below to see the trained bNEAT AI playing a game of snake. 

[![IMAGE ALT TEXT HERE](https://i.imgur.com/b16uInR.png)](https://www.youtube.com/watch?v=S4cb9tJu4vs)

# Design Approach
Why build an entire NEAT implementation from scratch? All for the sake of learning. This project was not built to be the most efficient or the prettiest but it did provide a **massive** amount of valuable experience with machine-learning.

The two main parts of bNEAT are the neural network and the genetic algorithm. As described in NEAT's design, genetic algorithms are used to augment ANN topolgies. This is in contrast to backpropogation which does not modify the structure of a topology. The GA parameters can be tweaked as needed for training, this allows a variety of ways to influence the learning behavior.

The training data is dumped to log files which are then interpreted by the Python parser. It was implemented using PyGame and produces the output shown in the video above.

The design for this project is based on Ken Stanley and Risto Miikulainen's work from
2002, found [here](http://nn.cs.utexas.edu/downloads/papers/stanley.gecco02_1.pdf) and [here](http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf).

# Results
bNEAT ended up being a very succesful program. It was regularly able to train "smart" snakes like the one featured above. The most difficult part of this was refining the genetic algorithm parameters to quickly produce fit snakes.

Our fitness function is a combination of food eaten and time stayed alive. This along with roulette wheel survivor selection proved to be the most effective at training snakes. You can compare roulette wheel selection to other methods with the following chart generated from out training data:

![Selection Graph](https://i.imgur.com/Wb55DFw.png)

# Future Features
bNEAT was built over the course of three months and is by no means complete. There are many more features that can be added to improve the efficiency and usability of the project. Below are some ideas:

- Import topology dump
- Recurrence support
- Multi-threaded training
- Implement C++ random library
- Percent population replacement
- More parent and survivor selection algorithms
- Trainer abstraction
- ANN copy constructor
- Make ConnectionGene a struct
- Improved management of GA parameters
- Implement vectors with std::unique_ptr instead of deques
