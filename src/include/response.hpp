#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../lib/json.hpp"
#include "../utils/helpers.hpp"
#include "requests.hpp"

// User input prompts
#define ID "id="
#define USERNAME "username="
#define PASSWORD "password="
#define TITLE "title="
#define AUTHOR "author="
#define GENRE "genre="
#define PUBLISHER "publisher="
#define PAGE_COUNT "page_count="

// Server connection details
#define PORT_HTTP 8080
#define IP_SERVER "34.254.242.81"

// Content type definitions
#define APP "application/json"

// API endpoints
#define LOGIN "/api/v1/tema/auth/login"
#define LOGOUT "/api/v1/tema/auth/logout"
#define REGISTER "/api/v1/tema/auth/register"
#define BOOKS "/api/v1/tema/library/books/"
#define ACCESS "/api/v1/tema/library/access"

// Placeholder values
#define NO_TOKEN ""
#define NO_QUERRY ""
#define NO_CONTENT_TYPE ""

/**
 * Receives a message from the server and stores it in `response`.
 *
 * @param response Reference to a string where the received response will be stored.
 * @param sockfd   Socket file descriptor for communication.
 */
void extractServerResponse(std::string &response, int &sockfd) {
    response = recvServerMessage(sockfd);
    if (response.empty()) {
        std::cout << "ERROR: No message received from the server!" << std::endl;
    }
}

/**
 * Parses the JSON response and prints an error message if applicable.
 *
 * @param jsonResponse The JSON-formatted error message.
 * @param reply        The extracted HTTP response code.
 */
void errorJSONReply(std::string &jsonResponse, std::string &reply) {
    try {
        nlohmann::json responseJSON = nlohmann::json::parse(jsonResponse);
        if (responseJSON.contains("error")) {
            std::cout << "ERROR: " << reply << " <=> " << responseJSON["error"].get<std::string>() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cout << "ERROR: Invalid JSON format received!" << std::endl;
    }
}

/**
 * Extracts the HTTP response code from the server response.
 *
 * @param response The full server response.
 * @return The extracted response code (e.g., "200", "404").
 */
std::string extractJSONCode(const std::string &response) {
    std::string responseCode;
    std::size_t spacePos = response.find(' ');
    if (spacePos != std::string::npos) {
        responseCode = response.substr(spacePos + 1, 3);
    }
    return responseCode;
}

/**
 * Extracts the JSON response body from the full HTTP response.
 *
 * @param response The full server response.
 * @return The extracted JSON content.
 */
std::string extractJSONResponse(const std::string &response) {
    std::size_t emptyLinePos = response.find(HEADER_TERMINATOR);
    if (emptyLinePos != std::string::npos && emptyLinePos + 4 < response.size()) {
        return response.substr(emptyLinePos + 4);
    }
    return "";
}

#endif /* RESPONSE_HPP */
