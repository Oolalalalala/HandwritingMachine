import socket
from PIL import Image

# Constants
acknowledge_byte = b'\x06'
transmission_begin_byte = b'\x01'
transmission_end_byte = b'\x00' # 0 for the segment length means end of transmission
recieve_end_buffer_size = 256 # Max value: 256


class WifiInterface:
    def __init__(self, server_ip, server_port):
        self.server_ip = server_ip
        self.server_port = server_port

    def connect(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect((self.server_ip, self.server_port))

        return True
    
    def read_bytes(self):
        while True:
            if self.client.recv(1) == transmission_begin_byte:
                break
        
        print("Transmission begin received")

        length = int.from_bytes(self.client.recv(4), 'big')

        print(f"Receiving {length} bytes")

        return self.client.recv(length)

    def send_bytes(self, data : bytes):

        self.client.send(transmission_begin_byte)
        self.__wait_for_acknowledge()

        print("Acknowledge received")

        while (len(data) > 0):
            segment_length = min(len(data), recieve_end_buffer_size - 1)

            print(f"Sending {segment_length} bytes")

            self.client.send(segment_length.to_bytes(1, byteorder='big'))
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
    wifi = WifiInterface("192.168.225.42", 8000)
    if wifi.connect():
        print("Connected to server")
    else:
        print("Failed to connect to server")

    wifi.send_bytes(b'Hello World')

    message = wifi.read_bytes()

    if len(message) != 176 * 144 * 2:
        print("Invalid image data")
        exit()

    rgb = []

    # RGB565
    for i in range(0, len(message) // 2):
        r = message[i * 2] >> 3
        g = ((message[i * 2] & 0b00000111) << 3) | (message[i * 2 + 1] >> 5)
        b = message[i * 2 + 1] & 0b00011111
        rgb.extend([r, g, b])

    # QCIF
    img = Image.frombytes('RGB', (176, 144), bytes(rgb))
    img.show()