import os
import sys

from serv_sync import create_serv_socket, get_client_sock, accept_request, write_response


def serve_client_fork(client_info):
    child_pid = os.fork()
    if child_pid:
        # still in parent process
        client_info["socket"].close()
        return child_pid

    req_info = accept_request(client_info)
    if (req_info["request"] is None) or (len(req_info["request"]) == 0):
        print(f"Client #{req_info['id']} unexpectedly disconnected")
    else:
        write_response(req_info)

    os._exit(0)  # do not use sys.exit after fork()


def reap_children(active_children):
    for child_pid in active_children.copy():
        child_pid, _ = os.waitpid(child_pid, os.WNOHANG)
        if child_pid:
            active_children.discard(child_pid)


def start_server(port=8888):
    serv_sock = create_serv_socket(port)
    client_id = 0
    children = set()
    try:
        while True:
            client_info = get_client_sock(serv_sock, client_id)
            child_pid = serve_client_fork(client_info)
            print(f"Forked with PID {child_pid}")
            children.add(child_pid)
            client_id += 1
            # kill children
            reap_children(children)
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
