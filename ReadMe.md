# LTCX
[![Build status](https://ci.appveyor.com/api/projects/status/0m4wmen1gjs3uouh?svg=true)](https://ci.appveyor.com/project/bradrothenberg/ntlatticegraph)

## About
NTLatticeGraph is a lightweight, portable file specification for representing 3D lattice structures. NTLatticeGraph is, appropriately enough, a graph representation; its purpose is to allow for lattices to be designed, manipulated, simulated, and manufactured easily.

## Usage
We plan to open-source a NTLatticeGraph Parser & put it here ASAP. Additionally we will include some sample files too.

Currently the file extension is `.ltcx`, the `.ltc` is for lattice & the `x` is for xml. We plan to have an even lighter weight binary version of NTLatticeGraph too.

Until then, please see the [spec](https://github.com/nTopology/NTLatticeGraph/blob/master/Spec.md "NTLatticeGraph Spec v0.1.0")  & [schema](https://github.com/nTopology/NTLatticeGraph/blob/master/schemas/NTLG_001.xsd "NTLatticeGraph schema v0.1.0").

### Directories
This project has the following folders & files:

- *lib*
  `libNTLatticeGraph`: A c++ library for reading/writing `NTLatticeGraph` files.
    - *bin*
      Binaries for `NTLatticeGraph` library
    - *ext*
      `TinyXML2` library that `libNTLatticeGraph` depends on.
    - *source*
      `libNTLatticeGraph` source.
    - *vc14*
      MS Visual Studio 2015 project file.
    - *CMakeLists.txt*
      For cmake.
- *samples*
  Sample `.ltcx` files.
- *schemas*
  `.xml` schema for `NTLatticeGraph`.
- *License.md*
  MIT license.
- *Spec.md*
  `NTLatticeGraph` Spec.

### License
See the file [LICENSE.md](https://github.com/nTopology/NTLatticeGraph/blob/master/License.md "NTLatticeGraph License v0.1.0")
