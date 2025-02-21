
#include "../includes/webserv.hpp"
#include <map>
#include "../includes/Files.hpp"
#include <fstream>

FileHandler::FileHandler()
{
	setMime();
	no_file = true;
}

FileHandler::~FileHandler()
{}

void FileHandler::setFileInfo(std::string path)
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

void FileHandler::setFile(std::string path)
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
	pos += 1;
	extention = path.substr(pos, path.size() - pos);
	Content_Type = mimes[extention];
}

void	FileHandler::setMime(void)
{
	mimes["jpg"]     = "image/jpeg";
	mimes["jpeg"]    = "image/jpeg";
	mimes["gif"]     = "image/gif";
	mimes["html"]    = "text/html";
	mimes["htm"]     = "text/html";
	mimes["css"]     = "text/css";
	mimes["json"]    = "application/json";
	mimes["js"]      = "application/javascript";
	mimes["pdf"]     = "application/pdf";
	mimes["png"]     = "image/png";
	mimes["zip"]     = "application/zip";
	mimes["rar"]     = "application/x-rar-compressed";
	mimes["sh"]      = "application/x-sh";
	mimes["xml"]     = "application/xml";
	mimes["csv"]     = "text/csv";
	mimes["txt"]     = "text/plain";
	mimes["mp3"]     = "audio/mpeg";
	mimes["wav"]     = "audio/wav";
	mimes["ogg"]     = "audio/ogg";
	mimes["mp4"]     = "video/mp4";
	mimes["mov"]     = "video/quicktime";
	mimes["avi"]     = "video/x-msvideo";
	mimes["mkv"]     = "video/x-matroska";
	mimes["webm"]    = "video/webm";
	mimes["flv"]     = "video/x-flv";
	mimes["doc"]     = "application/msword";
	mimes["docx"]    = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimes["ppt"]     = "application/vnd.ms-powerpoint";
	mimes["pptx"]    = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimes["xls"]     = "application/vnd.ms-excel";
	mimes["xlsx"]    = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mimes["odt"]     = "application/vnd.oasis.opendocument.text";
	mimes["ods"]     = "application/vnd.oasis.opendocument.spreadsheet";
	mimes["rtf"]     = "application/rtf";
	mimes["epub"]    = "application/epub+zip";
	mimes["apk"]     = "application/vnd.android.package-archive";
	mimes["exe"]     = "application/x-msdownload";
	mimes["json"]    = "application/json";
	mimes["svg"]     = "image/svg+xml";
	mimes["ico"]     = "image/x-icon";
	mimes["tiff"]    = "image/tiff";
	mimes["bmp"]     = "image/bmp";
	mimes["webp"]    = "image/webp";
	mimes["7z"]      = "application/x-7z-compressed";
	mimes["tar"]     = "application/x-tar";
	mimes["gz"]      = "application/gzip";
	mimes["bz2"]     = "application/x-bzip2";
	mimes["xz"]      = "application/x-xz";
	mimes["msi"]     = "application/x-msi";
	mimes["flac"]    = "audio/flac";
	mimes["aiff"]    = "audio/aiff";
	mimes["amr"]     = "audio/amr";
	mimes["aac"]     = "audio/aac";
	mimes["midi"]    = "audio/midi";
	mimes["wma"]     = "audio/x-ms-wma";
	mimes["cue"]     = "application/x-cue";
}
