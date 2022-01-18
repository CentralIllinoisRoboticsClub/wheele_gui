import time
from pySerialTransfer import pySerialTransfer as txfer

class struct(object):
    x = 1.02
    y = 0.21
    z = 0.0
    yaw = 2.10
    pitch = 0.00
    roll = 1.1

if __name__ == '__main__':
    try:
        link = txfer.SerialTransfer('/dev/ttyACM0')

        link.open()

        while True:
            id = int(input("TEST ID: "))
            send_size = 0
            cmd_vel = struct

            if id == 10:
                ###################################################################
                # Send mode
                ###################################################################
                mode = input("WHEELE Mode(0-2): ")
                size = link.tx_obj(int(mode), val_type_override='i')
                send_size += size

            if id == 20:
                ###################################################################
                # Send NAV STATE
                ###################################################################
                navstate = input("WHEELE NAV STATE(-2 - 3): ")
                size = link.tx_obj(int(navstate), val_type_override='i')
                send_size += size

            if id == 70:
                dummy = 1;
                size = link.tx_obj(int(dummy), val_type_override='i')
                send_size += size

            if id == 80:
                cmd_vel.x = float(input("x vel:"))
                size = 0
                size = link.tx_obj(cmd_vel.x,start_pos=size)
                size = link.tx_obj(cmd_vel.y,start_pos=size)
                size = link.tx_obj(cmd_vel.z,start_pos=size)
                size = link.tx_obj(cmd_vel.yaw,start_pos=size)
                size = link.tx_obj(cmd_vel.pitch,start_pos=size)
                size = link.tx_obj(cmd_vel.roll,start_pos=size)
                send_size += size

            if id == 90:
                size = link.tx_obj('Hello')
                send_size += size

            ###################################################################
            # Transmit all the data to send in a single packet
            ###################################################################
            if send_size != 0:
                link.send(send_size,id)


    except KeyboardInterrupt:
        try:
            link.close()
        except:
            pass

    except:
        import traceback
        traceback.print_exc()

        try:
            link.close()
        except:
            pass
