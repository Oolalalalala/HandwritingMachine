from CommunicationInterface import CommunicationInterface
from PIL import Image
import time


def on_image_recieve(image):
    image.show()
    filepath = "./Images/Image" + str(time.time()) + ".png"

    try:
        image.save(filepath)
        print("Image saved to " + filepath)
    except:
        print("Failed to save image to file")


com = CommunicationInterface("192.168.225.42", 8000)
com.on_image_recieve(on_image_recieve)
com.begin()