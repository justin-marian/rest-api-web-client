#ifndef ADD_BOOK
#define ADD_BOOK

#include "../response.hpp"

/**
 * Adds a new book to the library system.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has entered the library.
 * @param jwt    JWT token for authentication.
 * @param reply  Reference to a string where the server response will be stored.
 */
void add_book(char* conn, int& sockfd, bool& login, bool& enter, std::string& jwt, std::string& reply)
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

    nlohmann::json json;

    // Get book details from the user
    std::string title;
    std::cout << TITLE;
    std::getline(std::cin >> std::ws, title);
    json["title"] = title;

    std::string author;
    std::cout << AUTHOR;
    std::getline(std::cin >> std::ws, author);
    json["author"] = author;

    std::string genre;
    std::cout << GENRE;
    std::getline(std::cin >> std::ws, genre);
    json["genre"] = genre;

    // Validate page count (must be an integer)
    std::string page_count;
    std::cout << PAGE_COUNT;
    std::getline(std::cin >> std::ws, page_count);

    // Check if the page count is a valid integer
    for (char c : page_count) {
        if (!std::isdigit(c)) {
            std::cout << "ERROR: Page count must be an integer!" << std::endl;
            return;
        }
    }
    json["page_count"] = page_count;

    std::string publisher;
    std::cout << PUBLISHER;
    std::getline(std::cin >> std::ws, publisher);
    json["publisher"] = publisher;

    // Convert the JSON object to a string representation
    std::string jsonStr = json.dump(); 
    size_t jsonLength = jsonStr.length(); 

    // Construct and send the POST request to add the book
    // - conn: The server connection details.
    // - BOOKS: The endpoint URL for adding a new book.
    // - jwt: The authentication token required for authorization.
    // - APP: The content type (usually "application/json").
    // - jsonStr.c_str(): The serialized JSON payload.
    // - jsonLength: The length of the JSON string.
    // - {}: No additional headers.
    // - 0: No extra data.
    std::string message = POST(conn, BOOKS, jwt, APP, jsonStr.c_str(), jsonLength, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);
    bool isJsonResponseEmpty = jsonResponse.empty();

    // If no JSON response is present, assume book was added successfully
    if (isJsonResponseEmpty) {
        std::cout << reply << " - Book successfully added." << std::endl;
        return;
    }

    // Handle potential errors returned by the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* ADD_BOOK */
