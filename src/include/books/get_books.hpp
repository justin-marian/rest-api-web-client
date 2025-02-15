#ifndef GET_BOOKS
#define GET_BOOKS

#include "../response.hpp"

/**
 * Retrieves a list of all books available in the library system.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has entered the library.
 * @param jwt    JWT token for authentication.
 * @param reply  Reference to a string where the server response will be stored.
 */
void get_books(char *conn, int &sockfd, bool &login, bool &enter, std::string &jwt, std::string &reply)
{
    // Check if the user is logged in
    if (!login) {
        std::cout << "ERROR: You are not logged in!" << std::endl;
        return;
    }

    // Check if the user has entered the library
    if (!enter) {
        std::cout << "ERROR: You must enter the library first!" << std::endl;
        return;
    }

    // Construct and send the GET request to retrieve all books
    // - conn: The server connection details.
    // - BOOKS: The API endpoint for fetching all books.
    // - NO_TOKEN: No additional authorization token needed.
    // - jwt: Authentication token required for authorization.
    // - {}: No additional headers.
    // - 0: No extra parameters.
    std::string message = GET(conn, BOOKS, NO_TOKEN, jwt, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);

    // Parse the JSON response
    nlohmann::json responseJSON;
    try {
        responseJSON = nlohmann::json::parse(jsonResponse);
    } catch (const std::exception &e) {
        std::cout << "ERROR: Failed to parse server response!" << std::endl;
        return;
    }

    // Check if the JSON response contains an error
    if (responseJSON.contains("error")) {
        std::cout << "ERROR: " << reply << " <=> " << responseJSON["error"].get<std::string>() << std::endl;
        return;
    }

    // Display the list of books in a readable format
    if (responseJSON.is_array() && !responseJSON.empty()) {
        std::cout << "List of books:\n";
        for (const auto &book : responseJSON) {
            std::cout << "- ID: " << book.value("id", "N/A") 
                      << ", Title: " << book.value("title", "Unknown") 
                      << ", Author: " << book.value("author", "Unknown") << std::endl;
        }
    } else {
        std::cout << "No books available in the library." << std::endl;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* GET_BOOKS */
