import os

def upload_text(text: str):
    pass

def main():
    os.system('cls' if os.name == 'nt' else 'clear')
    print("Select upload mode:")
    print("1. Select .txt file")
    print("2. Directly input")
    mode = int(input("Enter mode: "))
    os.system('cls' if os.name == 'nt' else 'clear')
    if mode == 1:
        selected = False
        while not selected:
            #print all .txt files in the current directory
            print("Files in the directory:")
            current_dir = os.path.join(os.getcwd(), 'Python')
            for file in os.listdir(current_dir):
                if file.endswith(".txt"):
                    print(file, end = " ")
            print()
            filename = os.path.join(current_dir, input("Enter filename: ") + ".txt")
            try:
                with open(filename, 'r', encoding = 'utf-8') as file:
                    text = file.read()
                selected = True
            
            except FileNotFoundError:
                os.system('cls' if os.name == 'nt' else 'clear')
                print("File not found, please try again.")
    elif mode == 2:
        text = input("Enter text: ")
    else:
        print("Invalid mode")
        return
    
    print("Text: " + text)
    upload_text(text)
    print("Text uploaded successfully")
    
if __name__ == '__main__':
    main()