import time
from pySerialTransfer import pySerialTransfer as txfer


if __name__ == '__main__':
    try:
        link = txfer.SerialTransfer('/dev/ttyACM1')

        link.open()

        while True:
            id = int(input("TEST ID: "))
            send_size = 0

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
