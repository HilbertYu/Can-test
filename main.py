import serial
import time

PORT = "/dev/cu.usbmodem1422301"
BAUD = 9600

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

with open("data", "r", encoding="utf-8") as f:
    lines = f.readlines()
f.close()


gr = [3.454, 1.947,1.366,0.972,0.738]
gear = 4


fid = 0


for line in lines:
    fid = fid + 1
    num = line.strip()
#    print(line)
    time.sleep(0.02)
    v_num = int(num)

    cur_rpm = int(v_num*(gr[gear - 1]/gr[3]))

    #gear = (int(num) / 1000) + 1;


    if (cur_rpm > 6000):
        gear = gear + 1
        print("up")
    elif (cur_rpm < 3300):
        gear = gear - 1
        print("down")

    gr_idx = gear - 1
    v_num = v_num*(gr[gr_idx]/gr[3])
    v_num = int(v_num)

    out_str = f"{v_num},{gear}\n";

    if num.isdigit():
        #ser.write((num + "\n").encode())
        ser.write(out_str.encode())
        print(out_str, end="")
    else:
        print("error")

# while True:
#     num = input("input num:")
#     time.sleep(0.2)
#     if num.lower() == "q":
#         break
#     if num.isdigit():
#         ser.write((num + "\n").encode())
#     else:
#         print("error")

ser.close()
