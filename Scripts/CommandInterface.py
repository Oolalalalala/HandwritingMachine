import WifiInterface

class CommandBuffer:
    def __init__(self):
        self.buffer = []
        self.wifi = WifiInterface.WifiInterface()
    
    def set_config(self, stroke_segment_length, stroke_speed, hover_speed):
        pass

    def move(self, start_x, start_y, end_x, end_y):
        pass

    def draw_dot(self, x, y):
        pass

    def draw_line(self, start_x, start_y, end_x, end_y):
        pass

    def draw_quadratic_curve(self, x_a, x_b, x_c, y_a, y_b, y_c):
        pass

    def draw_quadratic_bezier(self, start_x, start_y, control_x, control_y, end_x, end_y):
        pass

    def draw_arc(self, center_x, center_y, radius, start_angle, end_angle):
        pass

    def clear(self):
        self.buffer = []


class CommandInterface:
    def __init__(self):
        self.command_buffer = CommandBuffer()

    def submit(self, command_buffer):
        pass

    def on_image_recieve(self, callback):
        self.on_image_recieve = callback
