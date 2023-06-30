# Webserv
This project is about implementing a HTTP web server, with C++. The server is able to serve a variety of types of file, it supports 3 basic methods which are GET, POST and DELETE.

The core operation in this program is the implementation of **Multiplexing**. This is a technique for handling multiple communication streams over a single comminucation channel or a single network socket. In the context of network programming, multiplexing refers to the ability of a single process or thread to handle multiple connections simultaneously.
This was possible through the sockets API, especially the select() function.

Concerning the code architecture we have applied the OOP paradigm, as much as possible. This was one of the great things in this project. We have conceptualize entities that we see as conerstones of the program, we got two main directories `Interfaces` and `Implementations`. Interfaces contained the classes definitions and Implementations contained the classes implementations, as the name tells. Going this way, was pretty beneficial in many aspects, for instance debugging, memory management and software enhancement. 



