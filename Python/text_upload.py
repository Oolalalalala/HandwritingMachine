import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph
import numpy as np
import matplotlib.pyplot as plt
from CommandBuffer import *
from CommunicationInterface import CommunicationInterface

buffer = CommandBuffer()
com = CommunicationInterface("192.168.225.42", 8000)

pt_to_mm = 127/360

class Plotter: #Not used and not finished, should be used for previewing the result
    def __init__(self):
        self.contours_plot = plt.figure(1)
        self.smooth_plot = plt.figure(2)
        self.contours_plot.gca().set_aspect('equal')
        self.smooth_plot.gca().set_aspect('equal')
        

def convert_text_to_commands(text: str, font_dir: str, font_size: int):
    current_pos = [0, 0]
    starting_pos = [0, 0]
    for char in text:
        r_current_pos, r_pre_char_width = convert_char_to_commands(char, font_dir, font_size, current_pos, starting_pos)
        current_pos = r_current_pos
        starting_pos[0] += r_pre_char_width

#Not the final version, just for testing
def convert_char_to_commands(char: str, font_dir: str, font_size: float, current_pos: tuple, starting_pos: tuple):
    font = describe.openFont(font_dir)
    g = glyph.Glyph(glyphquery.glyphName(font, char))
    width = glyphquery.width(font, g.glyphName)
    line_height = glyphquery.lineHeight(font)
    contours = g.calculateContours(font)
    
    # #Print and plot the contours (for debugging purposes)
    # for coutour in contours:
    #     print(coutour)
    #     print()
    #     x = []
    #     y = []
    #     for point in coutour:
    #         if point[1] == 1:
    #             plt.plot(point[0][0], point[0][1], 'ro')
    #         else:
    #             plt.plot(point[0][0], point[0][1], 'bo')
    #         x.append(point[0][0])
    #         y.append(point[0][1])
    #     plt.plot(x, y)
    # plt.show()
    
    #Convert the contour to the correct size
    adjusted_contours = []
    adjusted_contour_homes = []
    for contour in contours:
        adjusted_contour = []
        for point in contour:
            adjusted_contour.append(((starting_pos[0] + point[0][0] * font_size / line_height, starting_pos[1] + (-1) * point[0][1] * font_size / line_height), point[1]))
        adjusted_contours.append(adjusted_contour)
        adjusted_contour_homes.append(adjusted_contour[0][0])
    
    current_contour_home = current_pos
    for adjusted_contour in adjusted_contours:
        
        #Move to the starting point of the contour
        if adjusted_contour[0][0] != current_contour_home:
            buffer.move(current_contour_home[0], current_contour_home[1], adjusted_contour[0][0], adjusted_contour[0][1])
            current_contour_home = adjusted_contour[0]
        
        #Create some temporary points
        pre_pre_point = adjusted_contour[-2]
        pre_point = adjusted_contour[-1]
        pre_temp_point = None
        if pre_point[1] == 0 and pre_pre_point[1] == 0:
            temp_point = (((pre_pre_point[0][0] + pre_point[0][0]) / 2, (pre_pre_point[0][1] + pre_point[0][1]) / 2), 1)
        else:
            temp_point = None
        
        #Convert and store the contour into the command buffer
        for point in adjusted_contour:
            if point[1] == 0 and pre_point[1] == 1:
                pass
            elif point[1] == 1 and pre_point[1] == 1:
                buffer.draw_line(pre_point[0][0], pre_point[0][1], point[0][0], point[0][1])
            elif point[1] == 1 and pre_point[1] == 0:
                if pre_pre_point[1] == 1:
                    buffer.draw_quadratic_bezier(pre_pre_point[0][0], pre_pre_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 point[0][0], point[0][1])
                elif pre_pre_point[1] == 0:
                    buffer.draw_quadratic_bezier(temp_point[0][0], temp_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 point[0][0], point[0][1])
            elif point[1] == 0 and pre_point[1] == 0:
                pre_temp_point = temp_point
                temp_point = (((pre_point[0][0] + point[0][0]) / 2, (pre_point[0][1] + point[0][1]) / 2), 1)
                if pre_pre_point[1] == 1:
                    buffer.draw_quadratic_bezier(pre_pre_point[0][0], pre_pre_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 temp_point[0][0], temp_point[0][1])
                elif pre_pre_point[1] == 0:
                    buffer.draw_quadratic_bezier(pre_temp_point[0][0], pre_temp_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 temp_point[0][0], temp_point[0][1])
            else:
                print("Error while processing contours.")
                pre_pre_point = ((pre_pre_point[0][0] - starting_pos[0]) * line_height / font_size, (pre_pre_point[0][1] - starting_pos[1]) * line_height / font_size, pre_pre_point[1])
                pre_point = ((pre_point[0][0] - starting_pos[0]) * line_height / font_size, (pre_point[0][1] - starting_pos[1]) * line_height / font_size, pre_point[1])
                point = ((point[0][0] - starting_pos[0]) * line_height / font_size, (point[0][1] - starting_pos[1]) * line_height / font_size, point[1])
                print("Pre_pre_point: " + str(pre_pre_point))
                print("Pre_point: " + str(pre_point))
                print("Point: " + str(point))
                return
            
            pre_pre_point = pre_point
            pre_point = point
    
    return (adjusted_contours[-1][-1], width * font_size / line_height)
    
    
    
def main():
    
    """
    Text selection
    """
    
    os.system('cls' if os.name == 'nt' else 'clear')
    print("Select upload mode:")
    print("1. Select .txt file")
    print("2. Directly input")
    mode = input("Enter mode: ")
    os.system('cls' if os.name == 'nt' else 'clear')
    if mode == "1" or mode == "1.":
        selected = False
        while not selected:
            #print all .txt files in the current directory
            print("Files in the directory:")
            current_dir = os.path.join(os.getcwd())
            for file in os.listdir(os.path.join(current_dir, "txt_files")):
                if file.endswith(".txt"):
                    print(file, end = " ")
            print()
            filename = input("Enter filename: ")
            if filename.endswith(".txt"):
                filename = filename[:-4]
            file_dir = os.path.join(current_dir, "txt_files", filename + ".txt")
            try:
                with open(file_dir, 'r', encoding = 'utf-8') as file:
                    text = file.read()
                selected = True
            
            except FileNotFoundError:
                os.system('cls' if os.name == 'nt' else 'clear')
                print("File not found, please try again.")
    elif mode == "2" or mode == "2.":
        text = input("Enter text: ")
    #testing mode
    elif mode == "123":
        text = "abc"
        current_dir = os.path.join(os.getcwd())
        font_dir = os.path.join(current_dir, "Python", "fonts", "Arial.ttf")
        font_size = 12
        convert_text_to_commands(text, font_dir, font_size)
        return
    else:
        print("Invalid mode")
        return
    
    print()
    
    """
    Font and size selection
    """
    
    font_list = ["Arial.ttf", "Times.ttf", "Consolas.ttf"]
    font_selected = False
    while not font_selected:
        print("Select a font:")
        for i in range(len(font_list)):
            print(str(i + 1) + ". " + font_list[i])
        font_num = input("Enter font number: ")
        
        try:
            font_num = int(font_num) - 1
            if font_num >= 0 and font_num - 1 < len(font_list):
                font = font_list[font_num]
                font_selected = True
            else:
                print("Invalid font number, please try again.")
        except ValueError:
            print("Invalid font number, please try again.")
        font_dir = os.path.join(current_dir, "Python", "fonts", font)
        
    print()
    
    font_size_selected = False
    while not font_size_selected:
        font_size = input("Enter font size (between 12 and 64): ")
        try:
            font_size = int(font_size)
            if font_size >= 12 and font_size <= 64:
                font_size_selected = True
            else:
                print("Invalid font size, please try again.")
        except ValueError:
            print("Invalid font size, please try again.")
    
    """
    Final Confirmation
    """
    
    os.system('cls' if os.name == 'nt' else 'clear')
    if len(text) >= 50:
        print("Text: " + text[:50] + "...\nFont: " + font + "\nSize: " + str(font_size))
    else:
        print("Text: " + text + "\nFont: " + font + "\nSize: " + str(font_size))
    confirm = input("Confirm? (Y/N): ")
    if confirm.lower() != 'y':
        return
    
    print("Processing and uploading text...")
    convert_text_to_commands(text, font_dir, font_size)
    com.submit_command_buffer(buffer)
    
    print("Writing text...")
    com.on_pc_control_mode_end(lambda: print("Writing pulsed."))
    com.begin()
    return
    
if __name__ == '__main__':
    main()