import socket


def main():
    serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto=0)
    print(f"Socket type: {type(serv_sock)}")

    serv_sock.bind(("127.0.0.1", 9090))
    serv_sock.listen(10)

    while True:
        client_sock, client_addr = serv_sock.accept()
        print('Connected by', client_addr)
        while True:
            try:
                data = client_sock.recv(1024)
                if not data:
                    break
                client_sock.sendall(data)
            except ConnectionResetError:
                break

        client_sock.close()


if __name__ == "__main__":
    main()
