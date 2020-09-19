The project contains two parts: a server and a client.

The server listens for TCP connections and saves all the received data from the client in a file.
The client connects to the server and as soon as connection established, sends the content of a file to the server.


$ ./server PORT /Directory

$ ./client IPaddress PORT Filename 
