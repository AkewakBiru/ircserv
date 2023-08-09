#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    char buffer[1024] = {0};
    std::vector<struct pollfd> pfds(10); // Assuming 10 client connections

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(9997);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Set up the pollfd for the server socket
    pfds[0].fd = serverSocket;
    pfds[0].events = POLLIN; // Monitor for incoming connections

    std::cout << "Listening for connections..." << std::endl;

    while (true) {
        int pollCount = poll(pfds.data(), pfds.size(), -1); // Wait indefinitely for events
        if (pollCount == -1) {
            std::cerr << "Error in poll()." << std::endl;
            break;
        }

        // Check if the server socket has incoming connections
        if (pfds[0].revents & POLLIN) {
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientSocket == -1) {
                std::cerr << "Error accepting client connection." << std::endl;
                continue;
			std::cout << "new connection on socket " << clientSocket << "\r\n";
            }

            // Add the new client socket to the pollfd array
            for (size_t i = 1; i < pfds.size(); ++i) {
                if (pfds[i].fd == -1) {
                    pfds[i].fd = clientSocket;
                    pfds[i].events = POLLIN | POLLOUT; // Monitor for incoming data and write availability
                    break;
                }
            }
        }

        // Check for incoming data or write availability on connected sockets
        for (size_t i = 1; i < pfds.size(); ++i) {
            if (pfds[i].fd == -1) {
                continue; // Skip unconnected sockets
            }

            // Check for incoming data
            if (pfds[i].revents & POLLIN) {
                int bytesRead = recv(pfds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead <= 0) {
                    // Client closed the connection or an error occurred
                    std::cout << "Client disconnected." << std::endl;
                    close(pfds[i].fd);
                    pfds[i].fd = -1;
                    continue;
                }

                // Process received data
                std::cout << "Received data from client: " << buffer << std::endl;

                // Clear the buffer for the next receive
                memset(buffer, 0, sizeof(buffer));
            }

            // Check for write availability (POLLOUT)
            if (pfds[i].revents & POLLOUT) {
                // Here, you can send a message to the client
                const char* message = "Hello from server!";
                int bytesSent = send(pfds[i].fd, message, strlen(message), 0);
                if (bytesSent == -1) {
                    std::cerr << "Error sending data to client." << std::endl;
                }
            }
        }
    }

    // Close all open sockets
    for (size_t i = 0; i < pfds.size(); ++i) {
        if (pfds[i].fd != -1) {
            close(pfds[i].fd);
        }
    }
    close(serverSocket);

    return 0;
}
