import os
from ttfquery import describe
from ttfquery import glyphquery
import ttfquery.glyph as glyph
import numpy as np
import matplotlib.pyplot as plt
from CommandInterface import *


def upload_text_as_commands(text: str, font_dir: str, font_size: int):
    # for char in text:
    #     convert_text_to_commands(char, font_url, font_size)
    convert_text_to_commands(text[0], font_dir, font_size)

#Not the final version, just for testing
def convert_text_to_commands(character: str, font_dir: str, font_size: int):
    char = character
    font = describe.openFont(font_dir)
    g = glyph.Glyph(glyphquery.glyphName(font, char))
    contours = g.calculateContours(font)
    
    
    print("Number of contours: " + str(len(contours)))
    print()
    
    #Print and plot the contours
    for coutour in contours:
        print(coutour)
        print()
        x = []
        y = []
        for point in coutour:
            x.append(point[0][0])
            y.append(point[0][1])
        plt.plot(x, y)
    plt.show()
    
    
    
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