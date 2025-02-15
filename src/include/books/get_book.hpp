#ifndef GET_BOOK
#define GET_BOOK

#include "../response.hpp"

/**
 * Retrieves details of a specific book from the library system.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has entered the library.
 * @param jwt    JWT token for authentication.
 * @param reply  Reference to a string where the server response will be stored.
 */
void get_book(char *conn, int &sockfd, bool &login, bool &enter, std::string &jwt, std::string &reply)
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

    // Prompt the user for the book ID
    std::cout << "Enter the book ID: ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    // Validate that the input contains only digits (i.e., is a valid number)
    for (char c : input) {
        if (!std::isdigit(c)) {
            std::cout << "ERROR: Book ID must be a number!" << std::endl;
            return;
        }
    }

    // Convert input to an integer and construct the request URL
    int id = std::stoi(input);
    std::string book_id = BOOKS + std::to_string(id);

    // Create and send the GET request to retrieve book details
    // - conn: The server connection details.
    // - book_id: The API endpoint for fetching book details.
    // - NO_TOKEN: No additional authorization token needed.
    // - jwt: Authentication token required for authorization.
    // - {}: No additional headers.
    // - 0: No extra parameters.
    std::string message = GET(conn, book_id, NO_TOKEN, jwt, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);
    bool isJsonResponseEmpty = jsonResponse.empty();

    // If no JSON response is present, indicate an unknown issue
    if (isJsonResponseEmpty) {
        std::cout << "ERROR: Unknown problem occurred!" << std::endl;
        return;
    }

    // Parse the JSON response
    nlohmann::json responseJSON = nlohmann::json::parse(jsonResponse);
    bool isJSONerror = responseJSON.contains("error");

    // Display the book details if no error is found
    if (!isJSONerror) {
        std::cout << "Book details: " << responseJSON.dump(4) << std::endl; // Pretty-print JSON
    } else {
        // Display the error message returned by the server
        std::cout << "ERROR: " << reply << " <=> " << responseJSON["error"].get<std::string>() << std::endl;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* GET_BOOK */
