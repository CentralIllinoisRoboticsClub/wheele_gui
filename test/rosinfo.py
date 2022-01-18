import rospy
import rosnode
import time
from serial import Serial
from pySerialTransfer import pySerialTransfer as txfer
from geometry_msgs.msg import Twist

DEVICE='/dev/ttyUSB0'
BAUDRATE=57600
ROS_ALIVE_ID=70
CMD_VEL_ID=80

class struct(object):
    x = 0.00
    y = 0.00
    z = 0.00
    yaw = 0.00
    pitch = 0.00
    roll = 0.00

cmd_vel = struct

def callback(data):
    cmd_vel.x = data.linear.x
    cmd_vel.y = data.linear.y
    cmd_vel.z = data.linear.z
    cmd_vel.yaw = data.angular.x
    cmd_vel.pitch = data.angular.y
    cmd_vel.roll = data.angular.z

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

        size = 0
        size = link.tx_obj(cmd_vel.x,start_pos=size)
        size = link.tx_obj(cmd_vel.y,start_pos=size)
        size = link.tx_obj(cmd_vel.z,start_pos=size)
        size = link.tx_obj(cmd_vel.yaw,start_pos=size)
        size = link.tx_obj(cmd_vel.pitch,start_pos=size)
        size = link.tx_obj(cmd_vel.roll,start_pos=size)
        link.send(size,CMD_VEL_ID)

        #if cmd_string[0] == '1':
        #    nodes = rosnode.get_node_names()
        #    for node in nodes:
        #        print(node)
            #datalink.writeline(nodes)


if __name__ == '__main__':
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber('cmd_vel', Twist, callback)
    rosinfo_server()
