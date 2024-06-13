import WifiInterface
import queue
import threading
import struct
from PIL import Image
from CommandBuffer import CommandBuffer, pack_command
import time
from DetectOOXX import detect_ooxx
import numpy as np


class CommunicationInterface:
    def __init__(self, server_ip, server_port):
        self.command_buffers = queue.Queue()
        self.connection = WifiInterface.WifiInterface(server_ip, server_port)
        
        self.pc_control_mode_end_callback = lambda: None
        self.image_recieve_callback = lambda: None

        self.should_terminate = False

    def submit_command_buffer(self, command_buffer):
        self.command_buffers.put(command_buffer)

    def on_pc_control_mode_end(self, callback):
        self.pc_control_mode_end_callback = callback

    def on_image_recieve(self, callback):
        self.image_recieve_callback = callback

    def terminate(self):
        self.should_terminate = True
        self.thread.join()


    # Create a thread to communicate with ESP32
    def begin(self):
        self.thread = threading.Thread(target=self.communicate_task)
        self.thread.start()
        

    def communicate_task(self):
        if self.connection.connect():
            print("Connected")
        
        current_command_buffer = None
        image_data_incoming = False

        start_time = time.time()
        timer = 0

        while True:
            
            if self.connection.client is not None:
                #self.connection.close_connection()
                pass

            end_time = time.time()
            dt = end_time - start_time
            start_time = end_time
            timer += dt

            if self.should_terminate:
                break
    
            # Reconnect every second
            if timer > 1:
                self.connection.connect()
                print("Connected")
                timer = 0

            if self.connection.client is None:
                continue

            # Listen for message
            message = self.connection.read_bytes(0.8)
            if message is None:
                continue

            print(f"Length of message: {len(message)}")

            type = chr(message[0])
            print(f"Type: {type}")

            if type == 'a': # ESP asks for command

                print("ESP32 asks for command")

                if current_command_buffer is None and self.command_buffers.empty():
                    continue
                
                request_length = message[1]
                if current_command_buffer is None:
                    current_command_buffer = self.command_buffers.get()

                command_data = b''

                for i in range(request_length):
                    if len(current_command_buffer.commands) == 0:
                        actual_length = i + 1
                        break

                    command = current_command_buffer.commands.pop(0)
                    command_data += pack_command(command)

                command_data = struct.pack("<B", actual_length) + command_data
                self.connection.send_bytes(command_data)

                if len(current_command_buffer.commands) == 0:
                    current_command_buffer = None


            elif type == 'e': # ESP ends PCControl mode
                self.command_buffers = queue.Queue()
                self.pc_control_mode_end_callback()


            elif type == 'h': # Image header
                image_width = int.from_bytes(message[1:3], 'big')
                image_height = int.from_bytes(message[3:5], 'big')
                print(f"Image size: {image_width} x {image_height}")

                message = self.connection.read_bytes(1.0)

                if message is None:
                    print("Image data not received")
                    continue

                if len(message) != image_width * image_height:
                    print("Invalid image data")
                    print(f"Length: {len(message)}")
                    continue

                print("Image data received")

                img = Image.frombytes('L', (image_width, image_height), message)
                #img.show()

                self.image_recieve_callback(img)
                image_data_incoming = False

            elif type == 'c': # OpenCV
                image_width = int.from_bytes(message[1:3], 'big')
                image_height = int.from_bytes(message[3:5], 'big')
                print(f"Image size (OOXX): {image_width} x {image_height}")

                message = self.connection.read_bytes(1.0)

                if message is None:
                    print("Image data (for OOXX) not received")
                    continue

                
                if len(message) != image_width * image_height:
                    print("Invalid image data (OOXX)")
                    print(f"Length: {len(message)}")
                    continue

                print("Image data received")

                img = np.frombuffer(message, dtype=np.uint8).reshape((image_height, image_width))
                res = detect_ooxx(img)

                if res is None:
                    value = 0
                elif res == 'O':
                    value = 1
                elif res == 'X':
                    value = 2

                self.connection.send_bytes(struct.pack("<B", value))


            