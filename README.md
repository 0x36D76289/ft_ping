# ft_ping

Project from École 42 aiming to recreate the `ping` command in C.

## Introduction

`ft_ping` is a simplified implementation of the `ping` utility found on Unix systems. It allows testing network connectivity to a host by sending **ICMP Echo Request** packets and receiving **ICMP Echo Reply** packets.

## Compilation

To compile the project, run the following command at the project root:

```bash
make
```

This will generate the `ft_ping` executable in the root directory.

## Usage

### Privileges

`ft_ping` uses raw sockets, which require `root` privileges. It must be executed with `sudo`.

### Syntax

```bash
sudo ./ft_ping [OPTIONS] <destination>
```

### Available Options

| Option  | Description |
|---------|-------------|
| `-v`    | Verbose mode. Displays additional information about received ICMP packets (other than Echo Reply). |
| `-h`    | Displays help and exits. |

### Arguments

- **`<destination>`**: IPv4 address or hostname of the target to send ICMP packets to.

### Usage Examples

```bash
sudo ./ft_ping google.com
sudo ./ft_ping 142.250.192.78
sudo ./ft_ping -v 1.1.1.1
```

## Features

- Sending ICMP Echo Request packets.
- Receiving and analyzing ICMP Echo Reply packets.
- Calculation of Round Trip Time (**RTT**).
- Displaying statistics:
  - Number of packets sent and received.
  - Packet loss percentage.
  - Total execution time.
- Signal handling:
  - **SIGINT (Ctrl+C)**: Proper interruption and display of final statistics.
  - **SIGALRM**: Periodic packet sending.
- Error handling (name resolution, socket creation, etc.).
- Support for both IPv4 addresses and hostnames.

## Project Structure

The source code is organized as follows:

```
ft_ping/
│── src/                 # Directory containing source files
│   │── ft_ping.h        # Main header file
│   │── main.c           # Program entry point
│   │── icmp.c           # ICMP packet handling functions
│   │── utils.c          # Utility functions (name resolution, etc.)
│   │── options.c        # Command-line options handling
│   │── error.c          # Error management
│── Makefile             # Build script
```

## Limitations

- Limited functionalities compared to the original `ping` command.
- No advanced feature support yet (such as custom ICMP packets or real-time advanced statistics display).
- Error handling could be improved.

## Author

This project was created by **[Your Name/Nickname]** as part of the École 42 curriculum.

## License

This project is licensed under **[License Name, e.g., MIT]**.

