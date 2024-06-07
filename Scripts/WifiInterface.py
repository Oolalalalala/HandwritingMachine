import socket

class WifiInterface:
    def __init__(self, server_ip, server_port):
        self.server_ip = server_ip
        self.server_port = server_port

    def connect(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect((self.server_ip, self.server_port))

        return True
    
    def read_bytes(self, size):
        return self.client.recv(size)

    def send_bytes(self, data : bytes):
        self.client.send(data)
    

if __name__ == '__main__':
    ci = WifiInterface("192.168.0.1", 80)
    print(ci.connect())
    #print(ci.send_command("test"))