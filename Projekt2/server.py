import socket
import threading

HOST = "0.0.0.0"
PORT = 12345

clients = [] # list of clients' sockets
usernames = {} # dict mapping socket to username
client_lock = threading.Lock() # mutex for critical sections

def broadcast_msg(msg: bytes, sender: socket.socket):
    # locking mutex, to avoid sending multiple messages at the same time
    with client_lock:
        for c in clients:
            if c is not sender:
                try:
                    c.send(msg)
                except:
                    pass

def handle_client(connection: socket.socket, address):
    # connecting client to server making it able to see messages
    print(f"+++New connection from: {address}+++")
    try:
        # getting username from client socket
        username = connection.recv(1024).decode().strip()
        if not username:
            username = address[0]

        # adding client to list and mapping its socket to username
        with client_lock:
            clients.append(connection)
            usernames[connection] = username

        # sending message from client to all different clients but not the one sending it
        while True:
            received = connection.recv(1024)
            if not received:
                break
            broadcast_msg((f"{username}: ".encode() + received), connection)
    except ConnectionResetError:
        pass
    finally:
        # removing client from list and closing connection to it
        with client_lock:
            clients.remove(connection)
            removed = usernames.pop(connection)
        connection.close()
        print(f"---User {removed} has disconnected---")
        broadcast_msg(f"---User {removed} has disconnected---".encode(), None)

def main():
    # creating listening IPv4 socket
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen()
    print(f'Server listening on {HOST}:{PORT}...')

    # accepting clients and handling multiple clients parallelly via threads
    try:
        while True:
            connection, address = server.accept()
            threading.Thread(target=handle_client, args=(connection, address), daemon=True).start()
    except KeyboardInterrupt:
        print('\nClosing server...')
    finally:
        server.close()

if __name__ == '__main__':
    main()