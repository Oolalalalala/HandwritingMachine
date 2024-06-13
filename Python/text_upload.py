import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph
import numpy as np
import matplotlib.pyplot as plt
from CommandBuffer import *
Buffer = CommandBuffer()
natural_size = 1000

class Plotter:
    def __init__(self):
        self.contours_plot = plt.figure(1)
        self.smooth_plot = plt.figure(2)
        self.contours_plot.gca().set_aspect('equal')
        self.smooth_plot.gca().set_aspect('equal')
        

def upload_text_as_commands(text: str, font_dir: str, font_size: int):
    for char in text:
        convert_char_to_commands(char, font_dir, font_size)

#Not the final version, just for testing
def convert_char_to_commands(char: str, font_dir: str, font_size: float):
    font = describe.openFont(font_dir)
    g = glyph.Glyph(glyphquery.glyphName(font, char))
    contours = g.calculateContours(font)
    
    #Convert the contour to the correct size
    adjusted_contours = []
    for contour in contours:
        adjusted_contour = []
        for point in contour:
            adjusted_contour.append((point[0] * font_size / natural_size, point[1] * font_size / natural_size))
        adjusted_contours.append(adjusted_contour)
    
    print("Number of contours: " + str(len(contours)) + "\n")
    
    for contour in adjusted_contours:
        #Print the original contour (for debugging purposes)
        print(contour)
        print()
        
        
        
        #Creating some temporary points
        pre_pre_point = contour[-2]
        pre_point = contour[-1]
        
        pre_temp_point = None
        if pre_point[1] == 0 and pre_pre_point[1] == 0:
            temp_point = (((pre_pre_point[0][0] + pre_point[0][0]) / 2, (pre_pre_point[0][1] + pre_point[0][1]) / 2), 1)
        else:
            temp_point = None
        
        #Convert and store the contour into the command buffer
        for point in contour:
            if point[1] == 0 and pre_point[1] == 1:
                pass
            elif point[1] == 1 and pre_point[1] == 1:
                Buffer.draw_line(pre_point[0][0], pre_point[0][1], point[0][0], point[0][1])
            elif point[1] == 1 and pre_point[1] == 0:
                if pre_pre_point[1] == 1:
                    Buffer.draw_quadratic_bezier(pre_pre_point[0][0], pre_pre_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 point[0][0], point[0][1])
                elif pre_pre_point[1] == 0:
                    Buffer.draw_quadratic_bezier(temp_point[0][0], temp_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 point[0][0], point[0][1])
            elif point[1] == 0 and pre_point[1] == 0:
                pre_temp_point = temp_point
                temp_point = (((pre_point[0][0] + point[0][0]) / 2, (pre_point[0][1] + point[0][1]) / 2), 1)
                if pre_pre_point[1] == 1:
                    Buffer.draw_quadratic_bezier(pre_pre_point[0][0], pre_pre_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 temp_point[0][0], temp_point[0][1])
                elif pre_pre_point[1] == 0:
                    Buffer.draw_quadratic_bezier(pre_temp_point[0][0], pre_temp_point[0][1],
                                                 pre_point[0][0], pre_point[0][1],
                                                 temp_point[0][0], temp_point[0][1])
            else:
                print("Error while processing contours.")
                print("Pre_pre_point: " + str(pre_pre_point))
                print("Pre_point: " + str(pre_point))
                print("Point: " + str(point))
                return
            
            pre_pre_point = pre_point
            pre_point = point
            
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
        text = "a"
        current_dir = os.path.join(os.getcwd())
        font_dir = os.path.join(current_dir, "Python", "fonts", "Arial.ttf")
        font_size = 12
        upload_text_as_commands(text, font_dir, font_size)
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
    
    print("Uploading text...")
    upload_text_as_commands(text, font_dir, font_size)
    print("Text uploaded successfully")
    
if __name__ == '__main__':
    main()