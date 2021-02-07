# clibs
Small C compilation with frequently needed libraries, like fifos

So far the following have been implemented:

* FIFO with variable size elements. Each element can have a maximum size of 65KB, and they are inserted in the queue with headers that indicate the payload size.
* FIFO with constant size elements. Each element must have the same size but it is configurable at compile time.
