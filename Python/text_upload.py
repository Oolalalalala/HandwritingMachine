import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph
import numpy as np
import matplotlib.pyplot as plt
from CommandBuffer import *
from CommunicationInterface import CommunicationInterface
from Plotter import Plotter

buffer = CommandBuffer()
# com = CommunicationInterface("192.168.225.42", 8000)
pt_to_mm = 0.352778
pltr = Plotter()

def convert_text_to_commands(text: str, font_dir: str, font_size: int, pltr: Plotter):
    current_pos = [0, 0]
    starting_pos = [0, 0]
    for char in text:
        r_current_pos, r_pre_char_width = convert_char_to_commands(char, font_dir, font_size, current_pos, starting_pos, pltr)
        current_pos = r_current_pos
        starting_pos[0] += r_pre_char_width

#Not the final version, just for testing
def convert_char_to_commands(char: str, font_dir: str, font_size: float, current_pos: tuple, starting_pos: tuple, pltr: Plotter = None):
    print("Processing character: " + char)
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
            adjusted_contour.append(((starting_pos[0] + point[0][0] * font_size / line_height, starting_pos[1] + point[0][1] * font_size / line_height), point[1]))
        adjusted_contours.append(adjusted_contour)
        adjusted_contour_homes.append(adjusted_contour[0][0])
    
    # print("debug homes")
    # print(adjusted_contour_homes)
    
    pltr.draw_contours(adjusted_contours)
    
    current_contour_home = current_pos
    for i in range(len(adjusted_contours)):
        #Move to the starting point of the contour
        if adjusted_contours[i][0][0] != current_contour_home:
            # print("debug pre_home")
            # print(current_contour_home)
            buffer.move(current_contour_home[0], current_contour_home[1], adjusted_contour_homes[i][0], adjusted_contour_homes[i][1])
            current_contour_home = adjusted_contour_homes[i]
            # print("debug")
            # print(current_contour_home)
        
        #Create some temporary points
        pre_temp_point = None
        temp_point = None
        
        #Convert and store the contour into the command buffer
        points = adjusted_contours[i]
        for j in range(len(points)):
            point = points[j]
            next_point = points[(j + 1) % len(points)]
            next2_point = points[(j + 2) % len(points)]
            
            # print("Point: " + str(point))
            # print("Next_point: " + str(next_point))
            # print("Next2_point: " + str(next2_point))
            # print("Temp_point: " + str(temp_point))
            # print("Pre_temp_point: " + str(pre_temp_point))
            
            if point[1] == 1 and next_point[1] == 1:
                buffer.draw_line(point[0][0], point[0][1], next_point[0][0], next_point[0][1])
            elif point[1] == 1 and next_point[1] == 0:
                if next2_point[1] == 1:
                    buffer.draw_quadratic_bezier(point[0][0], point[0][1],
                                                 next_point[0][0], next_point[0][1],
                                                 next2_point[0][0],  next2_point[0][1])
                elif next2_point[1] == 0:
                    temp_point = (((next2_point[0][0] + next_point[0][0]) / 2, (next2_point[0][1] + next_point[0][1]) / 2), 1)
                    buffer.draw_quadratic_bezier(point[0][0], point[0][1],
                                                 next_point[0][0], next_point[0][1],
                                                 temp_point[0][0],  temp_point[0][1])
            elif point[1] == 0 and next_point[1] == 1:
                pass
            elif point[1] == 0 and next_point[1] == 0:
                if next2_point[1] == 1:
                    buffer.draw_quadratic_bezier(temp_point[0][0], temp_point[0][1],
                                                 next_point[0][0], next_point[0][1],
                                                 next2_point[0][0], next2_point[0][1])
                elif next2_point[1] == 0:
                    pre_temp_point = temp_point
                    temp_point = (((next2_point[0][0] + next_point[0][0]) / 2, (next2_point[0][1] + next_point[0][1]) / 2), 1)
                    buffer.draw_quadratic_bezier(pre_temp_point[0][0], pre_temp_point[0][1],
                                                 next_point[0][0], next_point[0][1],
                                                 temp_point[0][0], temp_point[0][1])
            else:
                print("Error while processing contours.")
                point = ((point[0][0] - starting_pos[0]) * line_height / font_size, (point[0][1] - starting_pos[1]) * line_height / font_size, point[1])
                next_point = ((next_point[0][0] - starting_pos[0]) * line_height / font_size, (next_point[0][1] - starting_pos[1]) * line_height / font_size, next_point[1])
                next2_point = ((next2_point[0][0] - starting_pos[0]) * line_height / font_size, (next2_point[0][1] - starting_pos[1]) * line_height / font_size, next2_point[1])
                print("Point: " + str(point))
                print("Next_point: " + str(next_point))
                print("Next2_point: " + str(next2_point))
                return    
    
    return (current_contour_home, font_size * width / line_height)
    
    
    
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
    
    current_dir = os.path.join(os.getcwd())
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
            if font_size >= 18 and font_size <= 64:
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
    convert_text_to_commands(text, font_dir, font_size, pltr)  
    pltr.draw_commands(buffer)
    pltr.show_plots()
    # com.submit_command_buffer(buffer)
    
    # print("Writing text...")
    # com.on_pc_control_mode_end(lambda: print("Writing pulsed."))
    # com.begin()
    return
    
if __name__ == '__main__':
    main()