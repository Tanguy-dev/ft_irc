# ft_irc Project

![42 Logo](https://img.shields.io/badge/42-Project-brightgreen.svg)

This is the ft_irc project, a part of the curriculum at École 42. The goal of this project is to implement an Internet Relay Chat (IRC) server and client, allowing users to communicate in real-time through channels and private messages.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Usage](#usage)
- [Server](#server)
- [Client](#client)
- [Building](#building)
- [Contributing](#contributing)
- [License](#license)

## Description

In the ft_irc project, we are tasked with designing and implementing a functional IRC server and client system. IRC is a widely used protocol for real-time communication in chat rooms or channels. The project involves creating a server that handles multiple clients, channels, and private messages. The clients connect to the server and can join various channels to communicate with each other.

The project aims to enhance our understanding of network programming, socket communication, and server-client architecture, while also improving our coding skills in C++.

## Features

- User authentication and registration.
- Joining and leaving channels.
- Sending and receiving messages in channels.
- Private messaging between users.
- Channel administration (e.g., creating, deleting channels).
- Basic commands (e.g., listing channels, listing users).

## Usage

To use the ft_irc project, you need to compile and run the server.

### Server

1. Clone the repository: `git clone <repository_url>`
2. Navigate to the project directory: `cd ft_irc`
3. Compile the server: `make`

Run the server: `./server <port> <password>`

### Client

1. Open a new terminal window.
2. Use irssi to connect to the server
3. Connect with : `irssi -c <ip> <port> -w <password>`

Follow the prompts to authenticate, join channels, and communicate with other users.

## Building

You can build and test the project by following these steps:

1. Clone the repository: `git clone <repository_url>`
2. Navigate to the project directory: `cd ft_irc`
3. Compile the server : `make`

The project may include additional instructions or scripts for running tests or setting up the environment.

## Contributing

Contributions to this project are not generally accepted, as it is an educational project for personal learning purposes.

## License

This project is licensed under the [MIT License](LICENSE).
