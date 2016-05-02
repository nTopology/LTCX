#NTLatticeGraph

##About

NTLattice graph is a portable .xml file that represents a lattice by its topology. A lattice is defined by a set of `nodes` and `beams`. In the future, it's possible that it will be extended to include `surfaces` and `volumes`.

NTLatticeGraph is meant to represent the topology of a lattice, with the primary element being a node. Beams are defined as connections between nodes.

For more information about how NTLatticeGraph operates in nTopology Element, see <https://github.com/nTopology/Element-Documentation>.

##Types

###Nodes
A point in 3D space that optionally has a radius.

Properties:
* id, unique (within nodes) integer
* x, double representing the X-coordinate
* y, double representing the Y-coordinate
* z, double representing the Z-coordinate
* r, double representing the radius

###Beams
A line connecting two nodes. If the nodes have diameters defined, the beam has a thickness. The thickness at any given point along the beam is defined by an interpolation function. The default interpolation function in linear.

Properties:
* id, unique (within beams) integer
* n1, a reference to a node ID
* n2, a reference to a node ID (note: N1 != N2)

##Surfaces
TBD
