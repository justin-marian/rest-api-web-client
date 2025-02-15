#include "include/response.hpp"

#include "include/logging/register.hpp"
#include "include/logging/login.hpp"
#include "include/logging/logout.hpp"
#include "include/logging/enter.hpp"

#include "include/books/get_book.hpp"
#include "include/books/get_books.hpp"
#include "include/books/add_book.hpp"
#include "include/books/del_book.hpp"

int main(void)
{
    std::string cmd;
    char *conn = (char *)IP_SERVER;

    int sockfd;
    bool log = false;
    bool enter = false;

    std::string reply;
    std::string cookie;
    std::string jwt;

    while (cmd != "exit") {
        getline(std::cin, cmd);

        cmd = httpMessageTrim(cmd); // Trim leading and trailing whitespace from the command
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { 
            return std::tolower(c);  // Convert the command to lowercase for easier comparison 
        });

        sockfd = openConnection(conn, PORT_HTTP, AF_INET, SOCK_STREAM, 0);

        if (cmd == "register") register_credentials(conn, sockfd, log, reply);
        else if (cmd == "login") login(conn, sockfd, log, reply, cookie);
        else if (cmd == "logout") logout(conn, sockfd, log, enter, jwt, reply, cookie);
        else if (cmd == "enter_library") enter_library(conn, sockfd, log, enter, jwt, reply, cookie);
        else if (cmd == "get_book") get_book(conn, sockfd, log, enter, jwt, reply);
        else if (cmd == "get_books") get_books(conn, sockfd, log, enter, jwt, reply);
        else if (cmd == "add_book") add_book(conn, sockfd, log, enter, jwt, reply);
        else if (cmd == "delete_book") del_book(conn, sockfd, log, enter, jwt, reply);
        else if (cmd != "exit") std::cout << "INVALID REQUEST SEND!" << std::endl;

        closeConnection(sockfd);
    }

    return EXIT_SUCCESS;
}
