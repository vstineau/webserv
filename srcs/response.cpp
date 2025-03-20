#include "../includes/webserv.hpp"

response &response::operator=(response &r)
{
	status_line = r.status_line;
	headers = r.headers;
	body = r.body;
	cgi_rep = r.cgi_rep;

	return *this;
}

// std::string getHtmlPage(std::string str) {
// 	std::ifstream html(str.c_str());
// 	std::string htmlPage;
// 	if (html.is_open()) {
// 		// std::cout << GREEN << "ssssss" << RESET << std::endl;
// 		std::string line;
// 		while (getline(html, line)) {
// 			htmlPage += line + "\n";
// 		}
// 		html.close();
// 		return htmlPage;
// 	}
// 	// std::cout << GREEN << "ssssss" << RESET << std::endl;
// 	return "";
// }

// std::string mainPage = getHtmlPage("srcs/html/index.html");
// std::string galeryPage = getHtmlPage("srcs/html/galery.html");

std::string to_string(long i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

void fillBodyResponse(std::string &content)
{
	std::string head = "<!DOCTYPE html>\n"
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
					   "    <main>\n";
	// "      <h2>"
	// "</h2></h2>\n";
	std::string tail = "    </main>\n"
					   "    <footer>\n"
					   "      <p>&copy; WeebServ UwU Baka Pro Max club</p>\n"
					   "    </footer>\n"
					   "  </body>\n"
					   "</html>";
	content = head + content + tail;
}

std::string response::repInString(void) const
{
	if (!cgi_rep.empty())
	{
		std::string rep(status_line);
		rep += "\r\n";
		rep += cgi_rep;
		return rep;
	}
	std::string r;
	r += status_line;
	r += "\r\n";
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = headers.begin(); it != headers.end(); it++)
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			r += it->first;
			r += it->second[i];
			r += "\r\n";
		}
	}
	r += "\r\n";
	r += body;
	return (r);
}
