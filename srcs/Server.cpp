
#include "../includes/Server.hpp"

Server::Server() :	_server_fd(0),
										_client_fd(0),
										_backlog(10),
										_service(AF_INET),
										_domain(SOCK_STREAM),
										_protocol(0)
{}

Server::Server(config conf):	_conf(conf),
															_server_fd(0),
															_client_fd(0),
															_backlog(10),
															_service(AF_INET),
															_domain(SOCK_STREAM),
															_protocol(0)

{}

Server::~Server()
{}

//create a socket for the serveur
int	Server::SetServFd()
{
	_server_fd = socket(_domain, _service, _protocol);
	if (_server_fd < 0)
	{
		std::cerr << "Error: socket creation failed\n";
		return (1);
	}
	return (0);
}

void Server::SetRequest(int fd, request &request)
{
	_requests[fd] = request;
}

//link server socket to the IP adress and to the port(s), start listenning then accept connection from client
int Server::bindListenAccept()
{
	if (bind(_server_fd, (struct sockaddr *)&_conf.address, sizeof(_conf.address)) < 0)
	{
		std::cerr << "Error: bind operation failed\n";
		return (1);
	}
	if (listen(_server_fd, _backlog) < 0)
	{
		std::cerr << "Error: listenning operation on socket " << _server_fd << " failed\n";
		return (1);
	}
	std::cout << "Server currently listenning on port : \n";
	for (std::vector<int>::iterator it = _conf.ports.begin(); it != _conf.ports.end(); it++)
		std::cout << "- " << *it << "\n";
	_client_fd = accept(_server_fd, (struct sockaddr *)&_conf.address, (socklen_t *)&_conf.len_address);
	if (_client_fd < 0)
	{
		std::cerr << "Error: client accept failed\n";
		return (1);
	}
	std::cout << "Client connected\n";
	return (0);
}

/*    // Le serveur est prêt à gérer une requête
    std::cout << "Client connecté !" << std::endl;
  // 6. Recevoir les données envoyées par le client
    char buffer[1024] = {0};  // Tampon pour stocker les données
    int bytes_read = recv(client_fd, buffer, 1024, 0);  // Lecture de la requête
    if (bytes_read < 0) {
        std::cerr << "Erreur: Échec de la réception de la requête" << std::endl;
        return -1;
    }

    // Afficher la requête reçue
    std::cout << "Requête reçue : " << buffer << std::endl;

    // Vérifier si la requête est une requête GET
    std::string request(buffer);
    std::cout << "\033[31m" << request << "\033[0m" << std::endl;
    if (request.find("GET") == 0) {
        // Le client a fait une requête GET
        std::cout << "Requête GET détectée !" << std::endl;
    }

            // 7. Créer une réponse HTTP
        std::string http_response =
            "HTTP/1.1 200 OK\r\n"  // Ligne de statut HTTP
            "Content-Type: text/html\r\n"  // Type de contenu (HTML)
            "\r\n"  // Fin des en-têtes
            "<html><body><h1>Welcome to My Mother Fucking Webserv</h1></body></html>";  // Corps de la réponse

        // 8. Envoyer la réponse au client
        send(client_fd, http_response.c_str(), http_response.size(), 0);

        std::cout << "Réponse envoyée au client." << std::endl;
    // 9. Fermer les connexions
    close(client_fd);  // Ferme la connexion avec le client
    close(server_fd);  // Ferme le socket du serveur

    return 0;
}*/
