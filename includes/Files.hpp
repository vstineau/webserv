

#include "../includes/webserv.hpp"
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

enum File_type {
	CLASSIC_FILE,
	DIRECTORY
};

class File {
	
	public: 
		File();
		~File();
		void setFileInfo(std::string path);
		void setFile(std::string path);
		std::map<std::string, std::string>	mimes;
//		getContentType();
//		execCgi();
		long long		file_size;
		std::string	filestring;
		std::string	path;
		File_type		type;
		std::string	extention;
		bool				no_file;
};



//int main(int argc, char *argv[])
//{
//    struct stat sb;
//
//    if (argc != 2) {
//        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
//        exit(EXIT_FAILURE);
//    }
//
//    if (stat(argv[1], &sb) == -1) {
//        perror("stat");
//        exit(EXIT_SUCCESS);
//    }
//
//	 std::cout << "Type de fichier :                ";
//
//    switch (sb.st_mode & S_IFMT) {
//		 case S_IFBLK:  std::cout << "périphérique de bloc\n";      break;
//    case S_IFCHR:  std::cout << "périphérique de caractère\n"; break;
//    case S_IFDIR:  std::cout << "répertoire\n";                break;
//    case S_IFIFO:  std::cout << "FIFO/tube\n";                 break;
//    case S_IFLNK:  std::cout << "lien symbolique\n";           break;
//    case S_IFREG:  std::cout << "fichier ordinaire\n";         break;
//    case S_IFSOCK: std::cout << "socket\n";                    break;
//    default:       std::cout << "inconnu ?\n";                 break;
//    }
//
//	 std::cout << "Taille du fichier :                " << (long long)sb.st_size << " octets\n";
//	 return 0;
//}
