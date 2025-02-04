import socket
import sys
import time


def create_serv_socket(port):
    serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto=0)

    serv_sock.bind(("127.0.0.1", port))
    serv_sock.listen()
    return serv_sock


def get_client_sock(serv_sock, client_id):
    client_sock, client_addr = serv_sock.accept()
    print(f"Accepted connection #{client_id} from {client_addr[0]}:{client_addr[1]}")
    return {"socket": client_sock, "id": client_id}


def accept_request(sock_info):
    delimiter = b"!"

    client_sock = sock_info["socket"]
    client_id = sock_info["id"]

    req = bytearray()
    while True:
        try:
            chunk = client_sock.recv(4)
            if not chunk:
                print(f"Client #{client_id} disconnected")
                break
            req += chunk
            if delimiter in chunk:
                break
        except ConnectionResetError:
            break
    return {"request": req, "socket": client_sock, "id": client_id}


def write_response(req_info):
    request = req_info["request"]
    client_sock = req_info["socket"]
    client_id = req_info["id"]

    request = request[::-1] + b'\n'
    time.sleep(1)
    client_sock.sendall(request)
    client_sock.close()
    print(f"Client #{client_id} successfully served")


def start_server(port=8888):
    serv_sock = create_serv_socket(port)
    client_id = 0
    try:
        while True:
            client_info = get_client_sock(serv_sock, client_id)
            req_info = accept_request(client_info)
            if (req_info["request"] is None) or (len(req_info["request"]) == 0):
                print(f"Client #{req_info['id']} unexpectedly disconnected")
            else:
                write_response(req_info)
            client_id += 1
    except KeyboardInterrupt:
        print("Interrupted by user")


def main():
    try:
        start_server(int(sys.argv[1]))
    except (KeyError, ValueError):
        print("Starting server with default port number")
        start_server()


if __name__ == "__main__":
    main()
