import socket

# Constants
acknowledge_byte = b'0x06'
transmission_begin_byte = b'0x02'
transmission_end_byte = b'0x03'
recieve_end_buffer_size = 256 # Max value: 256


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

        self.client.send(transmission_begin_byte)
        self.__wait_for_acknowledge()

        while (len(data) > 0):
            segment_length = min(len(data), recieve_end_buffer_size - 1)

            self.client.send(segment_length.to_bytes(1))
            self.client.send(data[:segment_length])

            data = data[segment_length:]

            self.__wait_for_acknowledge()

        self.client.send(transmission_end_byte)
        self.__wait_for_acknowledge()

        
    def __wait_for_acknowledge(self):
        while True:
            if self.client.recv(1) == acknowledge_byte:
                return True
    

if __name__ == '__main__':
    wifi = WifiInterface("192.168.0.1", 80)
    print(wifi.connect())

    wifi.send_bytes(b'Hello World')