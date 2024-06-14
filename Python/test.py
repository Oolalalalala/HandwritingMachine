from matplotlib import pyplot as plt
import numpy as np

#draw a quadratic bezier curve
def draw_quadratic_bezier_curve(p0, p1, p2):
    t = np.linspace(0, 1, 51)
    x = ((1 - t) ** 2 * p0[0] + 2 * (1 - t) * t * p1[0] + t ** 2 * p2[0])
    y = ((1 - t) ** 2 * p0[1] + 2 * (1 - t) * t * p1[1] + t ** 2 * p2[1])
    plt.plot(x, y, 'black')

#draw a random curve
def draw_random_curve():
    t = np.linspace(0, 1, 51)
    x = t
    y = t
    plt.plot(x, y, 'green')

  
def main():
    # Define the control points
    p0 = (0, 0)
    p1 = (2, 3)
    p2 = (4, 0)
    
    # Draw the quadratic bezier curve
    draw_quadratic_bezier_curve(p0, p1, p2)
    draw_random_curve()
    
    # Show the plot
    plt.show()
    
if __name__ == "__main__":
    main()