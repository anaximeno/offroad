# OffRoad - Computing Offloader
The goal of this application is to be able to run a file in a remote computer/server as if it were running in your current computer/server.

### Architecture
The initial architecture of this program consists in two nodes that communicate with each other, where the first one called **Requester Nodes** (**rnodes** for short) communicates with the second one **Processor Nodes** (**pnodes** for short) in a structure similar to the client/server approach (for know, the goal is to allow multiple and distributed processor nodes in the system), and the connection is made through sockets.

### How to build?
This was made primarily for the Linux Operating Systems, but may work on other unix based ones as well.
After cloning this repository make sure to also clone is sub-repositories by running inside the cloned directory of this repository:
```bash
git submodule update --init
```
For compiling the program, from the root directory of this program execute the following command:
```bash
make
```

### How to run?
After the completion of the build process the program can be executed by the following:
```bash
./offroad
```
<!-- TODO -->