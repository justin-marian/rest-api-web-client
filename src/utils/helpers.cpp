#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>

#include "helpers.hpp"
#include "buffer.hpp"

// Throws an error with the provided message
inline void error(const char *msg) {
    throw std::runtime_error(msg);
}

/**
 * Appends an HTTP header line to the message.
 *
 * @param message The message buffer.
 * @param line    The line to append.
 */
void httpMessage(char *message, const char *line) {
    strcat(message, line);
    strcat(message, "\r\n");
}

/**
 * Trims leading and trailing whitespace from a string.
 *
 * @param str The input string.
 * @return The trimmed string.
 */
std::string httpMessageTrim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * Opens a connection to a server.
 *
 * @param host_ip      The hostname or IP address of the server.
 * @param portno       The port number.
 * @param ip_type      The IP type (AF_INET).
 * @param socket_type  The socket type (SOCK_STREAM).
 * @param flag         Additional socket flags.
 * @return The socket file descriptor.
 */
int openConnection(char *host_ip, int portno, int ip_type, int socket_type, int flag) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Resolve hostname to IP if needed
    server = gethostbyname(host_ip);
    if (server == NULL) {
        error("ERROR: No such host found");
    }

    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0) {
        error("ERROR: Failed to open socket");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect the socket
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        error("ERROR: Failed to connect to server");
    }

    return sockfd;
}

/**
 * Closes a socket connection.
 *
 * @param sockfd The socket file descriptor.
 */
void closeConnection(int sockfd) {
    close(sockfd);
}

/**
 * Sends a message to a server via a socket.
 *
 * @param sockfd  The socket file descriptor.
 * @param message The message to send.
 */
 void sendServerMessage(int sockfd, const std::string &message) {
    int bytes, sent = 0;
    int total = message.length();

    do {
        bytes = write(sockfd, (const void*)(message.c_str() + sent), total - sent);
        if (bytes < 0) {
            error("ERROR: Failed to write message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

/**
 * Receives a message from a server via a socket.
 *
 * @param sockfd The socket file descriptor.
 * @return The received message as a string.
 */
 std::string recvServerMessage(int sockfd) {
    char response[BUFFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFFLEN);
        if (bytes < 0) {
            error("ERROR: Failed to read response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);

            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);

    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFFLEN);
        if (bytes < 0) {
            error("ERROR: Failed to read response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }

    std::string result = buffer.data;
    buffer_free(&buffer);
    return result;
}
