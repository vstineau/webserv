
#include "../includes/webserv.hpp"
#include "../includes/Files.hpp"
#include <fstream>

File::File()
{
	mimes["jpg"]	= "image/jpeg";
	mimes["jpeg"]	= "image/jpeg";
	mimes["gif"]	= "image/gif";
	mimes["html"]	= "text/html";
	mimes["htm"]	= "text/html";
	mimes["css"]	= "text/css";
	mimes["json"] = "application/json";
	mimes["js"]		= "application/javascript";
	mimes["pdf"]	= "application/pdf";
	mimes["png"]	= "image/png";
	mimes["zip"]	= "application/zip";
	mimes["rar"]	= "application/x-rar-compressed";
	mimes["sh"]		= "application/x-sh";

	no_file = true;
}

File::~File()
{}

void File::setFileInfo(std::string path)
{
	struct stat sb;
	if (path.empty())
		return ;
	if (stat(path.c_str(), &sb) == -1)
	{
		no_file = true;
		return ;
	}
	no_file = false;
	std::cout << "Type de fichier :                ";
	switch (sb.st_mode & S_IFMT)
	{
		case S_IFBLK:  std::cout << "périphérique de bloc\n";      break;
		case S_IFCHR:  std::cout << "périphérique de caractère\n"; break;
		case S_IFDIR:  type = DIRECTORY; std::cout << "répertoire\n";                break;
		case S_IFIFO:  std::cout << "FIFO/tube\n";                 break;
		case S_IFLNK:  std::cout << "lien symbolique\n";           break;
		case S_IFREG:  type = CLASSIC_FILE; std::cout << "fichier ordinaire\n";         break;
		case S_IFSOCK: std::cout << "socket\n";                    break;
		default:       std::cout << "inconnu ?\n";                 break;
	}
	file_size = (long long)sb.st_size;
}

void File::setFile(std::string path)
{
	std::string line;
	std::ifstream ifs(path.c_str());
	if (!ifs)
	{
		std::cout << "code mieux fdp\n";
		(NULL);
	}
	while (std::getline(ifs, line))
	{
		filestring += line;
		filestring += "\n";
	}
	std::size_t pos;
	pos = path.find(".");
	if (pos == std::string::npos)
	{
		extention = "NO EXTENTION";
		return ;
	}
	extention = path.substr(pos, path.size() - pos);
}
