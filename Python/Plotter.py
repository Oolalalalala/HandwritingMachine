from matplotlib import pyplot as plt
import numpy as np


class Plotter: #Not finished, should be used for previewing the result
    def __init__(self):
        self.fig, self.axs = plt.subplots(2, 1)
        self.contours_plot = self.axs[0]
        self.smooth_plot = self.axs[1]
        self.contours_plot.axis('equal')
        self.smooth_plot.axis('equal')
        
    def draw_contours(self, contours):
        x = []
        y = []
        for contour in contours:
            x.append([])
            y.append([])
            for point in contour:
                if point[1] == 1:
                    self.contours_plot.plot(point[0][0], point[0][1], 'ro')
                else:
                    self.contours_plot.plot(point[0][0], point[0][1], 'bo')
                x[len(x) - 1].append(point[0][0])
                y[len(y) - 1].append(point[0][1])
            self.contours_plot.plot(x[len(x) - 1], y[len(y) - 1], 'black')
    
    def draw_commands(self, command_buffer):
        for command in command_buffer.commands:
            if command[0] == "draw_line":
                self.smooth_plot.plot([command[1], command[3]], [command[2], command[4]], 'black')
            elif command[0] == "draw_quadratic_bezier":
                t = np.linspace(0, 1, 51)
                x = ((1 - t) ** 2 * command[1] + 2 * (1 - t) * t * command[2] + t ** 2 * command[3])
                y = ((1 - t) ** 2 * command[4] + 2 * (1 - t) * t * command[5] + t ** 2 * command[6])
                self.smooth_plot.plot(x, y, 'black')
            elif command[0] == "move":
                # self.smooth_plot.plot([command[1], command[2]], [command[3], command[4]], 'green')
                pass
            else:
                print("Invalid command: " + str(command))
    
    def show_plots(self):
        plt.show()
        