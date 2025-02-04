import socket


def main():
    sock = socket.socket()
    sock.bind(('', 8900))
    sock.listen(1)
    conn, addr = sock.accept()
    print(f"Connected to {addr}")
    while True:
        data = conn.recv(1024)
        if not data:
            break
        conn.send(data)
    conn.close()


if __name__ == "__main__":
    main()
