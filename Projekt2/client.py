import socket
import threading

HOST = "127.0.0.1"
PORT = 12345

def receiving(sock: socket.socket):
    # printing messages received from server
    try:
        while True:
            received = sock.recv(1024)
            if not received:
                print("---Connection closed---")
                break
            print(received.decode())
    except:
        print("---Connection error---")
    finally:
        return
    
def main():
    # creting IPv4 socket and connecting it to a server
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((HOST, PORT))
    except:
        print(f"Couldn't connect to {HOST}:{PORT}")
        return

    # getting username and sending it to a server, so it could map it to specific socket
    username = input("Enter username: ")
    sock.send(username.encode())

    # client can type and receive a message at the same time
    threading.Thread(target=receiving, args=(sock,), daemon=True).start()

    # reading client's messages and sending it to the server
    try:
        while True:
            msg = input()
            if not msg:
                print("---Connection closed---")
                break
            else:
                sock.send(msg.encode())
    except:
        pass
    finally:
        print("You left the chat")
        return

if __name__ == '__main__':
    main()
