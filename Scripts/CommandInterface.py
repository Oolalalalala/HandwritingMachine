import WifiInterface

class CommandBuffer:
    def __init__(self):
        self.buffer = []
        self.wifi = WifiInterface.WifiInterface()

    def add_command(self, command):
        self.buffer.append(command)

    def execute(self):
        for command in self.buffer:
            self.wifi.send_command(command)

    def clear(self):
        self.buffer = []

class CommandInterface:
    def __init__(self):
        self.command_buffer = CommandBuffer()

    def submit():
        pass