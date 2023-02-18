# include "../Interfaces/HttpServer.hpp"
# include "../parsing/parsing.hpp"

/* ----------------------------------------------------- */
/* ------------------ CANONICAL FORM ------------------- */
/* ----------------------------------------------------- */

HttpServer::HttpServer ( ServerConfiguration &serverConfig )
	: _serverConfiguration(serverConfig) 
{
	 }

HttpServer::HttpServer ( void ) { }
HttpServer::~HttpServer ( void ) { }

HttpServer	&HttpServer::operator= ( const HttpServer &obj )
{
	std::cout << "assingment operator" << std::endl;
	if (this == &obj)
		return (*this);
	this->_serverConfiguration = obj._serverConfiguration;
	return (*this);
}

HttpServer::HttpServer ( const HttpServer &obj )
{
	*this = obj;
}
/*
// */

/* ----------------------------------------------------- */
/* ------------------ MEMBER FUNCTIONS ----------------- */
/* ----------------------------------------------------- */

void	HttpServer::_setUpListeningSocket( void )
{
	struct addrinfo *bindAddress;
	int				optval; // ???

	memset(&_serverHints, 0, sizeof(_serverHints));
	_serverHints.ai_family = AF_INET;
	_serverHints.ai_socktype = SOCK_STREAM;
	_serverHints.ai_flags = AI_PASSIVE;
	std::cout << "_serverConfig host is " << _serverConfiguration.serverHost << std::endl;
	getaddrinfo(_serverConfiguration.serverHost.c_str(),
				_serverConfiguration.serverPort.c_str(),
				&_serverHints, &bindAddress);
	
	_listeningSocket = socket(bindAddress->ai_family, bindAddress->ai_socktype, bindAddress->ai_protocol);
	if (_listeningSocket < 0)
		exit (EXIT_FAILURE); // to be replaced by sth else
	std::cout << "Socket Created Successfully" << std::endl;
	optval = 1;
    setsockopt(_listeningSocket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	if (bind(_listeningSocket, bindAddress->ai_addr, bindAddress->ai_addrlen))
		exit (EXIT_FAILURE); // to be replaced by sth else
	std::cout << "Binded Successfully" << std::endl;
	freeaddrinfo(bindAddress);
	if (listen(_listeningSocket, MAXQUEUESIZE) < 0)
		exit (EXIT_FAILURE); // to be replaced by sth else
	std::cout << "Set Up for LISTENING Successfully" << std::endl;
}

void	HttpServer::_selectClients ( void )
{
	_maxSocket = _listeningSocket;
	std::list<ClientInfo >::iterator	ClientInfoIt;
	fd_set						_readFds;
	fd_set						_writeFds;

	ClientInfoIt = this->_clientsList.begin();
	FD_ZERO(&_readFds);
    FD_SET(this->_listeningSocket, &_readFds);
    FD_ZERO(&_writeFds);
    FD_SET(this->_listeningSocket, &_writeFds);
	for (; ClientInfoIt != this->_clientsList.end(); ClientInfoIt++)
	{
		FD_SET(ClientInfoIt->socket, &_readFds);
        FD_SET(ClientInfoIt->socket, &_writeFds);
		_maxSocket = std::max(_maxSocket, ClientInfoIt->socket);
	}
	std::cout << _maxSocket << std::endl;
	if (select(_maxSocket + 1, &_readFds, &_writeFds, NULL, NULL) == -1)
		errorPrinting("select has failed"); // to be moved
	std::cout << "Select oh Yeah" << std::endl;

}

void	HttpServer::setClientInfoList ( std::list<ClientInfo> & )
{

}

// void	HttpServer::_addClient ( SOCKET	clientSocket )
// {
// 	ClientInfo	client;

// 	this->_cli
// }

void	HttpServer::_acceptNewConnection( void )
{
	
// 	if (FD_ISSET(this->_listeningSocket, &(this->_readFds)))
//     {
// 		ClientInfo	newClient;
// 		this->_clientsList.push_front(newClient);
//         newClient.socket = accept(this->_listeningSocket, (struct sockaddr *) &(newClient.address), &(newClient.addressLength));
// //           fcntl(client->socket, F_SETFL, O_NONBLOCK);
//         FD_SET(newClient.socket, &(this->_readFds));
//         FD_SET(newClient.socket, &(this->_writeFds));
//         _maxSocket = std::max(_maxSocket, newClient.socket);
//         if (newClient.socket < 0)
// 			std::cerr << "accept function failed\n";
//     }
}

// void HttpServer::dropClient( SOCKET &clientSocket, std::list<ClientInfo >::iterator &ClientInfoIt)
// {
//     close(clientSocket);
//     std::list<ClientInfo >::iterator tempIterator = ClientInfoIt;
//     ClientInfoIt++;
//     this->_clientsList.erase(tempIterator);
// }


void	HttpServer::_serveClients( void )
{
	// std::list<ClientInfo >::iterator	ClientInfoIt;

	// ClientInfoIt = this->_clientsList.begin();
	// while (ClientInfoIt != this->_clientsList.end())
	// {
	// 	if (FD_ISSET(ClientInfoIt->socket, &(this->_readFds)))
	// 	{
	// 		if (ClientInfoIt->isFirstRead)
	// 		{
	// 			ClientInfoIt->parsedRequest.receiveFirstTime(ClientInfoIt->socket);
	// 			ClientInfoIt->parsedRequest.parse();

	// 			if(isUriTooLong(ClientInfoIt->parsedRequest.requestDataMap["path"]))
	// 			{
	// 				error_414(this->_clientsList, ClientInfoIt);
	// 				this->dropClient(ClientInfoIt->socket, ClientInfoIt);
	// 				continue ;
	// 			}
	// 			if (ClientInfoIt->parsedRequest.requestDataMap["method"] == "GET")
	// 			{
	// 				GETMethod getRequest;
	// 				getRequest.callGET(ClientInfoIt);
	// 			}
	// 			else if (ClientInfoIt->parsedRequest.requestDataMap["method"] == "DELETE")
	// 			{
	// 				// calling get method function.
	// 				client_data_it++;
	// 				continue ;
	// 			}
	// 			else if (ClientInfoIt->parsedRequest.requestDataMap["method"] == "POST")
	// 			{
	// 					parsingMiniHeader(ClientInfoIt);
	// 					postRequestStruct postRequest(ClientInfoIt, client_data_it, client_data, *it);
	// 					ClientInfoIt->requestBody.open("uploads/" + ClientInfoIt->uploadFileName, std::ios::binary);
	// 					ClientInfoIt->requestBody.write(ClientInfoIt->requestHeader + ClientInfoIt->bodyIndex, receivedBytes - ClientInfoIt->bodyIndex);
	// 					ClientInfoIt->requestBody.close();
	// 					exit (1);
	// 			}
	// 			ClientInfoIt->isFirstRead = false;
	// 		}
	// 		else
	// 		{
				
	// 		}
	// 	}
	// }
}

void	HttpServer::setUpMultiplexing ( void )
{
	// this->_selectClient();
	// this->_acceptNewConnection();
	// this->serve
}

void	HttpServer::setUpHttpServer( void )
{
	std::cout << this->_serverConfiguration.serverHost << std::endl;
	std::cout << this->_serverConfiguration.serverPort << std::endl;
	this->_setUpListeningSocket();
	this->_selectClients();
	// // this->_setUpMultiplexing();
}