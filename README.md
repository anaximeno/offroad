# OffRoad - Computing Offloader

This application's goal is to run a file in a remote computer/server as if it were running on your current computer/server.

### Architecture

The initial architecture of this program consists in two nodes that communicate with each other, where the first one called **Requester Nodes** (**rnodes** for short) communicates with the second one **Processor Nodes** (**pnodes** for short) in a structure similar to the client/server approach (the goal is to allow multiple and distributed processor nodes in the system), and the connection is made through sockets.

### How to build?

This was made primarily for the Linux Operating Systems but may work on other Unix-based ones.
After cloning this repository make sure to also clone its sub-repositories by running inside the cloned directory of this repository:

```bash
git submodule update --init
```

For compiling the program, from the root directory of this program execute the following command:

```bash
make
```

### How to run?

First, you'll need to execute the processor node to be able to make requests to it:

```bash
./offroad --process --port 9091
```

Then you will need to execute the requester node, but for this, you will need an executable file
to send to be executed by the processor node, a file named **script.sh** was provided for testing, what it does is run a simple [bash script](./script.sh) in a loop which can be stopped by hitting **cntrl+c**:

```bash
./offroad --host 127.0.0.1 --port 9091 -r script.sh
```

After that, the file will be sent to the processor node, which will execute the file. For now, the results of the processing can only be seen on the processor side but the goal is to integrate the execution to make it look as if it is running the computer that requests the service.
