#NTLatticeGraph

##About

NTLattice graph is a portable .xml file that represents a lattice by it's topology. A lattice is defined by a set of `nodes`, `beams`, & in the future `surfaces`.

NTLatticeGraph is meant to represent the topology of a lattice, with the primary element being a node. Beams are defined as connections between nodes. In the future, surfaces can be defined by nodes too (in counter-clockwise order)

##Types

###Nodes
A Point in 3D space that optionally has a diameter.

Properties:
* id, unique (within nodes) integer
* X, double representing the X-coordinate
* Y, double representing the Y-coordinate
* Z, double representing the Z-coordinate
* D, double representing the diameter

###Beams
A line connecting two nodes. If the nodes have diameters defined, this edge can have a thickness. The thickness is to be defined by an interpolation function. The default interpolation function in linear.

Properties:
* id, unique (within beams) integer
* N1, a reference to a node ID
* N2, a reference to a node ID (note: N1 != N2)

##Surfaces
TBD

##Meta-data

###Beam Types
functions that represent interpolations along a beam.
