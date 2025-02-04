import socket


def main():
    sock = socket.socket()
    sock.connect(("localhost", 8900))

    data = b"hello, world! "
    sock.send(data)
    data = sock.recv(1024)
    sock.close()
    print(data)


if __name__ == "__main__":
    main()
