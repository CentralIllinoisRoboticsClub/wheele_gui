import rospy
from serial import Serial
from geometry_msgs.msg import Twist
from std_msgs.msg import String

WHEELE_MAX_LINEAR = .2 #1
WHEELE_MAX_ANGULAR = .52 #1

X_MIN_SPEED = 122
X_MAX_SPEED = 305
X_ZERO_SPEED = 217
X_SPEED_NORMALIZE = abs((X_MAX_SPEED-X_MIN_SPEED)/2.0)

Y_MIN_SPEED = 229
Y_MAX_SPEED = 41
Y_ZERO_SPEED = 135
Y_SPEED_NORMALIZE = abs((Y_MAX_SPEED-Y_MIN_SPEED)/2.0)

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

if __name__ == '__main__':

    try:
        publish_message()
    except rospy.ROSInterruptException:
        pass
