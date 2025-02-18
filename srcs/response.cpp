#include "../includes/webserv.hpp"

std::ostream & operator<<(std::ostream & o, response & i)
{
	o << i.status_line << "\r\n";
	for (std::map<std::string, std::string>::iterator it = i.headers.begin(); it != i.headers.end(); it++)
		o << it->first << it->second << "\r\n";
	o << "\r\n";
	o << i.body;
	return (o);
}

std::string getHtmlPage(std::string str) {
	std::ifstream html(str.c_str());
	std::string htmlPage;
	if (html.is_open()) {
		// std::cout << GREEN << "ssssss" << RESET << std::endl;
		std::string line;
		while (getline(html, line)) {
			htmlPage += line + "\n";
		}
		html.close();
		return htmlPage;
	}
	// std::cout << GREEN << "ssssss" << RESET << std::endl;
	return "";
}

std::string mainPage = getHtmlPage("srcs/html/index.html");
std::string galeryPage = getHtmlPage("srcs/html/galery.html");

std::string to_string(long i) {
	std::stringstream s;
	s << i;
	return s.str();
}

int send_response(std::string message, int fd) {
	std::string head =
		"HTTP/1.1 200 OK\r\nContent-Length: " + to_string(message.length()) + "\r\n\r\n" + message;
	return send(fd, head.c_str(), head.size(), MSG_NOSIGNAL);
}

std::string fillDirectoryListing(std::string listing) {
	std::string head =
		"<!DOCTYPE html>\n"
		"<html lang=\"fr\">\n"
		"  <head>\n"
		"    <meta charset=\"UTF-8\" />\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
		"    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\" />\n"
		"    <title>WeebServ UwU</title>\n"
		"    <style>\n"
		"      body {\n"
		"        font-family: Arial, sans-serif;\n"
		"        background-color: #f4f4f4;\n"
		"        margin: 0;\n"
		"        padding: 0;\n"
		"      }\n"
		"      header {\n"
		"        background-color: #333;\n"
		"        color: white;\n"
		"        padding: 10px 0;\n"
		"        text-align: center;\n"
		"      }\n"
		"      main {\n"
		"        padding: 20px;\n"
		"      }\n"
		"      footer {\n"
		"        background-color: #333;\n"
		"        color: white;\n"
		"        text-align: center;\n"
		"        padding: 10px 0;\n"
		"        position: fixed;\n"
		"        bottom: 0;\n"
		"        width: 100%;\n"
		"      }\n"
		"      a {\n"
		"        color: #007bff;\n"
		"        text-decoration: none;\n"
		"      }\n"
		"      a:hover {\n"
		"        text-decoration: underline;\n"
		"      }\n"
		"      a:active {\n"
		"        color: rgba(255, 0, 0, 0.65);\n"
		"      }\n"
		"    </style>\n"
		"  </head>\n"
		"  <body>\n"
		"    <header>\n"
		"      <h1>Webserv</h1>\n"
		"    </header>\n"
		"    <main>\n"
		"      <h2>Directory Listing</h2>\n";
	std::string tail = "    </main>\n"
					   "    <footer>\n"
					   "      <p>&copy; WeebServ UwU Baka Pro Max club</p>\n"
					   "    </footer>\n"
					   "  </body>\n"
					   "</html>";
	return head + listing + tail;
}