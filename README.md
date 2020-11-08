# Polyhack2020 - Team Patatnik
This repository is about an algorithm, we developed during the Polyhack hackathon.
The goal of this challenge was to efficiently find bottlenecks in a train system.

## Our solution is an algorithm that computes a metric for classifying bottlenecks
The main idea is to model the network as a graph and the weight of each node is the capacity of the track.
We modified the max-flow algorithm to this situation, such that it is locally consistent. That is, it calculates the max-flow between two nodes but with the added restriction that the length contributing to the flow is smaller than a predefined value k. This makes sense, since a bottleneck is not resolved by sending a train to the other end of Switzerland and back. 

Our metric for classifying a bottleneck for two pairs of nodes is  the following:
We look at the flow without the construction site, and see how much the flow is reduced after adding the construction site.

## Our Heuristic for generating potential bottlenecks
We assume that bottlenecks usually appear close to construction sites, so we find a set of random pairs of nodes that are around the same construction site. We choose potential bottlenecks in the following way:
We start a random-walk from a node on a construction site in two directions and we walk a predefined number of steps. In this way, we hope to find two nodes in the vicinity of a construction site. On those two nodes, we calculate the amount of flow-reduction.

We do this often and choose the nodes which decrease the flow the most (percentage-vise). We have a threshold (given as the second parameter by the user), according to this threshold determines how 'heavy of a restriction' is counted as a bottleneck.

## Prototype/Implementation
We fully implemented a working modified Edmonds-Karp algorithm in C++, Since this graph is very sparse, it is highly efficient.
We implemented a GUI on a Flask-Webserver that allows the user to enter a date and a 'resolution', this data is then, sent to the server, evaluated and potential bottlenecks for this timespan are then displayed on a map in the browser. 
In the background, a Flask-Server runs the C++ script and returns the data to the website.


## Future improvement
The part we did not end up finishing was the visualisation of our algorithm in the context of the whole network of the SBB. We can, at the moment only display the output of the information. We would like to see the results of our algorithm in combination with the other visual features of the sbb network.

## How to use this repository
It is a flask-server, it runs a C++ script in the background, so C++ 17 needs to be installed.

After running the Flask-Application one can input two numbers (we recommend 0.1 and 01-01-2024 to start), as the day and the 'resolution' the algorithm should be run on. The results should then be immediately visible on the map (note that for a lot of values, there are very few bottlenecks caused by construction sites).
