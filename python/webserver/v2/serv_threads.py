import sys
import threading

from serv_sync import create_serv_socket, get_client_sock, accept_request, write_response


def serve_client_thread(client_sock, client_id):
    client_info = {"socket": client_sock, "id": client_id}
    req_info = accept_request(client_info)
    if (req_info["request"] is None) or (len(req_info["request"]) == 0):
        print(f"Client #{req_info['id']} unexpectedly disconnected")
    else:
        write_response(req_info)


def start_server(port=8888):
    serv_sock = create_serv_socket(port)
    client_id = 0
    try:
        while True:
            client_info = get_client_sock(serv_sock, client_id)
            client_sock = client_info["socket"]
            client_id = client_info["id"]
            t = threading.Thread(target=serve_client_thread, args=(client_sock, client_id))
            t.start()
            client_id += 1
            # it is better to have list of threads and try to join each thread

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
