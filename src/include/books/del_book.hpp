#ifndef DEL_BOOK
#define DEL_BOOK

#include "../response.hpp"

/**
 * Deletes a book from the library system.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has entered the library.
 * @param jwt    JWT token for authentication.
 * @param reply  Reference to a string where the server response will be stored.
 */
void del_book(char *conn, int &sockfd, bool &login, bool &enter, std::string &jwt, std::string &reply)
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

    // Prompt for book ID
    std::cout << "Enter the book ID: ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    // Validate that the input is a valid integer
    for (char c : input) {
        if (!std::isdigit(c)) {
            std::cout << "ERROR: Book ID must be an integer!" << std::endl;
            return;
        }
    }

    // Convert input to an integer and construct the request URL
    int id = std::stoi(input);
    std::string book_id = BOOKS + std::to_string(id);

    // Construct the DELETE request
    // - conn: The server connection details.
    // - book_id: The endpoint URL for deleting a specific book.
    // - jwt: The authentication token required for authorization.
    // - NO_TOKEN: No additional cookies or authorization headers needed.
    // - NO_TOKEN: No additional session tokens used.
    // - 0: No additional headers.
    // - {}: Empty list of headers (since DELETE requests typically require only authentication).
    // - 0: No body content, as DELETE requests generally do not include a request payload.
    std::string message = DELETE(conn, book_id, jwt, NO_TOKEN, NO_TOKEN, 0, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);
    bool isJsonResponseEmpty = jsonResponse.empty();

    // If no JSON response is present, assume deletion was successful
    if (isJsonResponseEmpty) {
        std::cout << reply << " - Book successfully deleted." << std::endl;
        return;
    }

    // Handle potential errors returned by the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* DEL_BOOK */
