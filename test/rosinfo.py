import rospy
import rosnode
import time
from serial import Serial
from pySerialTransfer import pySerialTransfer as txfer

DEVICE='/dev/ttyUSB0'
BAUDRATE=57600
ROS_ALIVE_ID=70

def publish_message():
    comm = Serial('/dev/ttyUSB0', baudrate=57600, timeout=2)
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rospy.init_node('touchscreen_publisher', anonymous=True)
    rate = rospy.Rate(100) # 100hz
    twist = Twist()
    while not rospy.is_shutdown():
    	# Read touchscreen coords over USB
        coords = comm.readline()
        try:
            decoded_bytes = coords[0:len(coords)-2].decode("utf-8") # remove newline
            strbytes = decoded_bytes.split(',')
            try:
            	# Convert coordinates to cmd_vel format ("Twist" - linear + angular speed)
                x = float(strbytes[0])
                y = float(strbytes[1])
                twist.linear.x = ((-(y - Y_ZERO_SPEED))/Y_SPEED_NORMALIZE)*WHEELE_MAX_LINEAR
                twist.angular.z = ((-(x - X_ZERO_SPEED))/X_SPEED_NORMALIZE)*WHEELE_MAX_ANGULAR

                if(twist.linear.x < 0):
                    twist.angular.z = -twist.angular.z

                # Publish!
                pub.publish(twist)
            except:
                pass
        except rospy.ROSInterruptException:
            print("error")
            pass
        #rospy.loginfo(hello_str)
        rate.sleep()

def rosinfo_server():
    #datalink = Serial(DEVICE, baudrate=BAUDRATE, timeout=2)
    link = txfer.SerialTransfer('/dev/ttyACM0')
    link.open()

    while not rospy.is_shutdown():
        #command_string = datalink.readline()
        #cmd_string = input("command: ")
        #cmd = cmd_string[0:len(cmd_string)-2].decode()
        time.sleep(2)
        dummy = 1;
        size = link.tx_obj(int(dummy), val_type_override='i')
        link.send(size,ROS_ALIVE_ID)

        #if cmd_string[0] == '1':
        #    nodes = rosnode.get_node_names()
        #    for node in nodes:
        #        print(node)
            #datalink.writeline(nodes)


if __name__ == '__main__':
    rosinfo_server()
