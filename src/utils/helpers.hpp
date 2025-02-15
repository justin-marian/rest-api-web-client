#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>

#define BUFFLEN 4096
#define LINELEN 1000

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)

#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

// Opens a connection with server host_ip on port portno, returns a socket
int openConnection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// Closes a server connection on socket sockfd
void closeConnection(int sockfd);

// Adds a line to a string message
void httpMessage(char *message, const char *line);

// Trims the whitespace from a string
std::string httpMessageTrim(const std::string &str);

// Sends a message to a server
void sendServerMessage(int sockfd, const std::string &message);

// Receives and returns the message from a server
std::string recvServerMessage(int sockfd);

#endif // HELPERS_HPP
