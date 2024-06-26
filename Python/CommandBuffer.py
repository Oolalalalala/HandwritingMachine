import struct

class CommandBuffer:
    def __init__(self):
        self.commands = []
    
    def set_config(self, stroke_segment_length, stroke_speed, hover_speed):
        self.commands.append(["set_config", stroke_segment_length, stroke_speed, hover_speed])

    def move(self, start_x, start_y, end_x, end_y):
        self.commands.append(["move", float(start_x), float(start_y), float(end_x), float(end_y)])

    def draw_dot(self, x, y):
        self.commands.append(["draw_dot", float(x), float(y)])

    def draw_line(self, start_x, start_y, end_x, end_y):
        self.commands.append(["draw_line", float(start_x), float(start_y), float(end_x), float(end_y)])

    def draw_quadratic_curve(self, x_a, x_b, x_c, y_a, y_b, y_c):
        self.commands.append(["draw_quadratic_curve", float(x_a), float(x_b), float(x_c), float(y_a), float(y_b), float(y_c)])

    def draw_quadratic_bezier(self, start_x, start_y, control_x, control_y, end_x, end_y):
        # Convert to quadratic curve
        # x_a = start_x - 2.0 * control_x + end_x
        # x_b = 2.0 * (control_x - start_x)
        # x_c = start_x

        # y_a = start_y - 2.0 * control_y + end_y
        # y_b = 2.0 * (control_y - start_y)
        # y_c = start_y

        # self.draw_quadratic_curve(x_a, x_b, x_c, y_a, y_b, y_c)
        self.commands.append(["draw_quadratic_bezier", float(start_x), float(control_x), float(end_x), float(start_y), float(control_y), float(end_y)])

    def draw_arc(self, center_x, center_y, radius, start_angle, end_angle):
        self.commands.append(["draw_arc", float(center_x), float(center_y), float(radius),float(start_angle), float(end_angle)])



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
