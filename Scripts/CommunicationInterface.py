import WifiInterface
import queue
import threading
import struct
from PIL import Image

class CommandBuffer:
    def __init__(self):
        self.commands = []
    
    def set_config(self, stroke_segment_length, stroke_speed, hover_speed):
        self.commands.append(["set_config", stroke_segment_length, stroke_speed, hover_speed])

    def move(self, start_x, start_y, end_x, end_y):
        self.commands.append(["move", start_x, start_y, end_x, end_y])

    def draw_dot(self, x, y):
        self.commands.append(["draw_dot", x, y])

    def draw_line(self, start_x, start_y, end_x, end_y):
        self.commands.append(["draw_line", start_x, start_y, end_x, end_y])

    def draw_quadratic_curve(self, x_a, x_b, x_c, y_a, y_b, y_c):
        self.commands.append(["draw_quadratic_curve", x_a, x_b, x_c, y_a, y_b, y_c])

    def draw_quadratic_bezier(self, start_x, start_y, control_x, control_y, end_x, end_y):
        # Convert to quadratic curve
        x_a = start_x - 2.0 * control_x + end_x;
        x_b = 2.0 * (control_x - start_x);
        x_c = start_x;

        y_a = start_y - 2.0 * control_y + end_y;
        y_b = 2.0 * (control_y - start_y);
        y_c = start_y;

        self.draw_quadratic_curve(x_a, x_b, x_c, y_a, y_b, y_c)

    def draw_arc(self, center_x, center_y, radius, start_angle, end_angle):
        self.commands.append(["draw_arc", center_x, center_y, radius, start_angle, end_angle])



def pack_command(command) -> bytes:

    data = b''

    if command[0] == "set_config":
        data = struct.pack("<Ifff", 1, command[1], command[2], command[3])
    
    elif command[0] == "move":
        data = struct.pack("<Iffff", 2, command[1], command[2], command[3], command[4])
    
    elif command[0] == "draw_dot":
        data = struct.pack("<Iff", 3, command[1], command[2])
    
    elif command[0] == "draw_line":
        data = struct.pack("<Iffff", 4, command[1], command[2], command[3], command[4])
    
    elif command[0] == "draw_quadratic_curve":
        data = struct.pack("<Iffffff", 5, command[1], command[2], command[3], command[4], command[5], command[6])
    
    elif command[0] == "draw_arc":
        data = struct.pack("<Iffffff", 6, command[1], command[2], command[3], command[4], command[5], command[6])

    # padding
    if len(data) < 28:
        data += b'\x00' * (28 - len(data))

    return data


class CommunicationInterface:
    def __init__(self, server_ip, server_port):
        self.command_buffers = queue.Queue()
        self.connection = WifiInterface.WifiInterface(server_ip, server_port)\
        
        self.on_pc_control_mode_end = lambda: None
        self.on_image_recieve = lambda: None

    def submit_command_buffer(self, command_buffer):
        self.command_buffers.put(command_buffer)

    def on_pc_control_mode_end(self, callback):
        self.on_pc_control_mode_end = callback

    def on_image_recieve(self, callback):
        self.on_image_recieve = callback

    def terminate(self):
        self.command_buffers.put(None)
        self.thread.join()


    # Create a thread to communicate with ESP32
    def begin(self):
        self.thread = threading.Thread(target=self.communicate_task)
        self.thread.start()
        

    def communicate_task(self):
        
        self.connection.connect()
        print("Connected to ESP32")

        current_command_buffer = None

        while True:

            # Listen for message
            message = self.connection.read_bytes()
            type = chr(message[0])

            if type == 'a': # ESP asks for command

                print("ESP32 asks for command")

                if current_command_buffer is None and self.command_buffers.empty():
                    continue
                
                request_length = message[1]
                if current_command_buffer is None:
                    current_command_buffer = self.command_buffers.get()

                    # Terminate 
                    if current_command_buffer is None:
                        break

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
                self.on_pc_control_mode_end()


            elif type == 'h': # Image header
                image_width = int.from_bytes(message[1:3], 'little')
                image_height = int.from_bytes(message[3:5], 'little')
                print(f"Image size: {image_width} x {image_height}")


            elif type == 'i': # Image data

                if len(message) - 1 != image_width * image_height:
                    print("Invalid image data")
                    print(len(message) - 1)
                    continue

                img = Image.frombytes('L', (image_width, image_height), message[1:])
                img.show()

                self.on_image_recieve(img)

            