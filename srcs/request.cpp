
#include "../includes/webserv.hpp"

std::string request::getContentType(std::string &buffer) const
{
	size_t pos = 0;
	size_t offset = 0;
	std::string type;

	pos = buffer.find("Content-Type:");
	if (pos == std::string::npos)
	{
		std::cerr << "Error: no Content-Type founded\n";
		return (0);
	}
	offset = pos + 14;
	pos = buffer.find("\r\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "Error: no Content-Type founded\n";
		return (0);
	}
	type = buffer.substr(offset, pos - offset);
	return (type);
}
/*
14.17 Content-Type

   The Content-Type entity-header field indicates the media type of the
   entity-body sent to the recipient or, in the case of the HEAD method,
   the media type that would have been sent had the request been a GET.

       Content-Type   = "Content-Type" ":" media-type

   Media types are defined in section 3.7. An example of the field is

       Content-Type: text/html; charset=ISO-8859-4

   Further discussion of methods for identifying the media type of an
   entity is provided in section 7.2.1.
*/

unsigned int request::getContentLength(std::string &buffer) const
{
	size_t pos = 0;
	size_t offset = 0;
	std::string length;

	pos = buffer.find("Content-Length:");
	if (pos == std::string::npos)
	{
		std::cerr << "Error: no Content-Length founded\n";
		return (0);
	}
	offset = pos + 16;
	pos = buffer.find("\r\n", offset);
	if (pos == std::string::npos)
	{
		std::cerr << "Error: no Content-Length founded\n";
		return (0);
	}
	length = buffer.substr(offset, pos - offset);
	return (atoi(length.c_str()));
}

/*
 14.13 Content-Length

   The Content-Length entity-header field indicates the size of the
   entity-body, in decimal number of OCTETs, sent to the recipient or,
   in the case of the HEAD method, the size of the entity-body that
   would have been sent had the request been a GET.

       Content-Length    = "Content-Length" ":" 1*DIGIT

   An example is

       Content-Length: 3495

   Applications SHOULD use this field to indicate the transfer-length of
   the message-body, unless this is prohibited by the rules in section
   4.4.
   Any Content-Length greater than or equal to zero is a valid value.
   Section 4.4 describes how to determine the length of a message-body
   if a Content-Length is not given.

   Note that the meaning of this field is significantly different from
   the corresponding definition in MIME, where it is an optional field
   used within the "message/external-body" content-type. In HTTP, it
   SHOULD be sent whenever the message's length can be determined prior
   to being transferred, unless this is prohibited by the rules in
   section 4.4.
*/
