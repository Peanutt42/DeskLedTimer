import math

def main():
    print("{")
    for row in range(8):
        print("\t{", end=" ")
        for column in range(8):
            led_coord_x = row - 3.5
            led_coord_y = column - 3.5
            angle_radians = math.atan2(led_coord_y, led_coord_x)
            percentage = (math.pi + angle_radians) / (2.0 * math.pi)
            print(f"{percentage}, ", end="")
        print("},")
    print("};")

if __name__ == "__main__":
    main()
