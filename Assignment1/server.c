// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    //                                               &opt, sizeof(opt)))
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // The privilege seperation part starts here

    //data structure containing user account information
    struct passwd *pw;
    if( (pw = getpwnam("nobody")) == NULL)
    {
        perror("unable to get the user info");
        exit(EXIT_FAILURE);
    }
    
    // creating child process
    pid_t child_process = fork();
    if(child_process < 0)
    {
        perror("Creating child process failed");
        exit(EXIT_FAILURE);        
    }
    // get the user id
    pid_t unprivileged_user_id = pw->pw_uid;
    if(child_process == 0) // child created successfully
    {
        // Drop privilege to unprivileged_user "nobody"
         if(setuid(unprivileged_user_id) < 0){
            perror("drop privilege failed");
            exit(EXIT_FAILURE);
        }

        valread = read( new_socket , buffer, 1024);
        if(valread < 0) 
        {
            perror("unable to read");
            exit(EXIT_FAILURE);
        }
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        printf("inside child process, uid: %d", getuid());
    }
    else // parent waits for the child to exit 
    {
        printf("Inside parent process, uid : %d",getuid());
        int status = 0;
        while ((wait(&status)) > 0);
    }

    return 0;
}
