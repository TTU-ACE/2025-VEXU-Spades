#region VEXcode Generated Robot Configuration
from vex import *
import urandom

# Brain should be defined by default
brain=Brain()

# Robot configuration code
# AI Classification Competition Element IDs
class GameElements:
    MOBILE_GOAL = 0
    RED_RING = 1
    BLUE_RING = 2
controller_1 = Controller(PRIMARY)
motor_16 = Motor(Ports.PORT16, GearSetting.RATIO_18_1, False)
# AI Vision Color Descriptions
ai_vision_1__BLUEBOX = Colordesc(1, 49, 92, 130, 10, 0.2)
ai_vision_1__REDBOX = Colordesc(2, 124, 17, 49, 10, 0.2)
# AI Vision Code Descriptions
ai_vision_1 = AiVision(Ports.PORT1, ai_vision_1__BLUEBOX, ai_vision_1__REDBOX, AiVision.ALL_AIOBJS)


# wait for rotation sensor to fully initialize
wait(30, MSEC)


# Make random actually random
def initializeRandomSeed():
    wait(100, MSEC)
    random = brain.battery.voltage(MV) + brain.battery.current(CurrentUnits.AMP) * 100 + brain.timer.system_high_res()
    urandom.seed(int(random))
      
# Set random seed 
initializeRandomSeed()


def play_vexcode_sound(sound_name):
    # Helper to make playing sounds from the V5 in VEXcode easier and
    # keeps the code cleaner by making it clear what is happening.
    print("VEXPlaySound:" + sound_name)
    wait(5, MSEC)

# add a small delay to make sure we don't print in the middle of the REPL header
wait(200, MSEC)
# clear the console to make sure we don't have the REPL in the console
print("\033[2J")



# define variables used for controlling motors based on controller inputs
controller_1_up_down_buttons_control_motors_stopped = True

# define a task that will handle monitoring inputs from controller_1
def rc_auto_loop_function_controller_1():
    global controller_1_up_down_buttons_control_motors_stopped, remote_control_code_enabled
    # process the controller input every 20 milliseconds
    # update the motors based on the input values
    while True:
        if remote_control_code_enabled:
            # check the buttonUp/buttonDown status
            # to control motor_16
            if controller_1.buttonUp.pressing():
                motor_16.spin(FORWARD)
                controller_1_up_down_buttons_control_motors_stopped = False
            elif controller_1.buttonDown.pressing():
                motor_16.spin(REVERSE)
                controller_1_up_down_buttons_control_motors_stopped = False
            elif not controller_1_up_down_buttons_control_motors_stopped:
                motor_16.stop()
                # set the toggle so that we don't constantly tell the motor to stop when
                # the buttons are released
                controller_1_up_down_buttons_control_motors_stopped = True
        # wait before repeating the process
        wait(20, MSEC)

# define variable for remote controller enable/disable
remote_control_code_enabled = True

rc_auto_loop_thread_controller_1 = Thread(rc_auto_loop_function_controller_1)

#endregion VEXcode Generated Robot Configuration

#initialize intake motor
intake = Motor(Ports.PORT17, GearSetting.RATIO_18_1, False)
intake.set_velocity(100, PERCENT)

#intialize hang motors
hang_motor_a = Motor(Ports.PORT15, GearSetting.RATIO_18_1, True)
hang_motor_b = Motor(Ports.PORT5, GearSetting.RATIO_18_1, False)
hang = MotorGroup(hang_motor_a, hang_motor_b)


#initialize drive train motor
left_motor_a = Motor(Ports.PORT10, GearSetting.RATIO_18_1, False)
left_motor_b = Motor(Ports.PORT7, GearSetting.RATIO_18_1, False)
left_drive_smart = MotorGroup(left_motor_a, left_motor_b)

right_motor_a = Motor(Ports.PORT20, GearSetting.RATIO_18_1, True)
right_motor_b = Motor(Ports.PORT18, GearSetting.RATIO_18_1, True)
right_drive_smart = MotorGroup(right_motor_a, right_motor_b)

drivetrain = DriveTrain(left_drive_smart, right_drive_smart, 299.24, 368.29999999999995, 368.29999999999995, MM, 1)

#initialize hook intake motor
hook_intake = Motor(Ports.PORT6, GearSetting.RATIO_18_1, False)
hook_intake.set_max_torque(100,PERCENT)
hook_intake.set_velocity(100,PERCENT)

# initialize clamp motor (run to position and hold)
clamp = Motor(Ports.PORT9, GearSetting.RATIO_18_1, False)
clamp.set_max_torque(100,PERCENT)
clamp.set_stopping(HOLD)
isClamped = False
CLAMP_DOWN_ANGLE=-217
CLAMP_UP_ANGLE=0


#initialize tilt motor (run to position and hold)
tilt = Motor(Ports.PORT8, GearSetting.RATIO_18_1, False)
# tilt.reset_position()
tilt.set_max_torque(100,PERCENT)
tilt.set_stopping(HOLD)
# UP and INIT should be the same for the intake tilt motor
TILT_UP_ANGLE=180
TILT_DOWN_ANGLE=0
TILT_INIT_ANGLE=180

# initialize lady brown servos (should move in sync)
ladyBrownLeft = Motor(Ports.PORT19, GearSetting.RATIO_18_1, False)
ladyBrownLeft.set_max_torque(100,PERCENT)
ladyBrownLeft.set_stopping(HOLD)
LB_LEFT_UP_ANGLE=300
LB_LEFT_DOWN_ANGLE=0
LB_LEFT_ABUTTON_ANGLE=67.25
LB_LEFT_INIT_ANGLE=180

ladyBrownRight = Motor(Ports.PORT16, GearSetting.RATIO_18_1, False)
ladyBrownRight.set_max_torque(100,PERCENT)
ladyBrownRight.set_stopping(HOLD)
LB_RIGHT_UP_ANGLE=-300
LB_RIGHT_DOWN_ANGLE=0
LB_RIGHT_ABUTTON_ANGLE=-67.25
LB_RIGHT_INIT_ANGLE=-180


# define variables used for controlling motors based on controller inputs
controller_1_left_shoulder_control_motors_stopped = True
controller_1_right_shoulder_control_motors_stopped = True
controller_1_up_down_buttons_control_motors_stopped = True
controller_1_x_b_buttons_control_motors_stopped = True
drivetrain_l_needs_to_be_stopped_controller_1 = False
drivetrain_r_needs_to_be_stopped_controller_1 = False
teleop_started = False
detect_blue_mode = False
abort_detection = False

class states:
    NOT_BEGUN = 1
    PRESSED = 2
    FINISHED = 3
isYPressed = False
currentYState = states.NOT_BEGUN

def status(buttonState):
    global isYPressed
    global currentYState

    if (buttonState == 0):
         _buttonState = False
    else:
         _buttonState = True

    if (_buttonState and currentYState == states.NOT_BEGUN):
        currentYState = states.PRESSED
        isYPressed = False
    elif ( not _buttonState and (currentYState == states.PRESSED)):
        currentYState = states.FINISHED
        isYPressed = True
    elif (currentYState == states.FINISHED):
        currentYState = states.NOT_BEGUN;
        isYPressed = False;
    return isYPressed;


def clampIt():
    global isClamped
    global currentYState
    if not isClamped:
       clamp.spin_to_position(CLAMP_DOWN_ANGLE, DEGREES)
       isClamped = True
    else:
       clamp.spin_to_position(CLAMP_UP_ANGLE, DEGREES)
       isClamped = False

def on_left_button_pressed():
   ladyBrownLeft.spin_to_position(LB_LEFT_DOWN_ANGLE, DEGREES, wait=False)
   ladyBrownRight.spin_to_position(LB_RIGHT_DOWN_ANGLE, DEGREES, wait=True)
   
def on_b_button_pressed():
   ladyBrownLeft.spin_to_position(LB_LEFT_INIT_ANGLE, DEGREES, wait=False)
   ladyBrownRight.spin_to_position(LB_RIGHT_INIT_ANGLE, DEGREES, wait=True)


def on_a_button_pressed():
   ladyBrownLeft.spin_to_position(LB_LEFT_ABUTTON_ANGLE, DEGREES, wait=False)
   ladyBrownRight.spin_to_position(LB_RIGHT_ABUTTON_ANGLE, DEGREES, wait=True)

def on_x_button_pressed():
    global detect_blue_mode, abort_detection
    if detect_blue_mode:
        # Abort detection mode if already active
        abort_detection = True
        detect_blue_mode = False
    else:
        # Start detection mode
        detect_blue_mode = True
        abort_detection = False


def pre_autonomous():
    # actions to do when the program starts
    brain.screen.clear_screen()
    brain.screen.print("Initializing ACE Bot\n")
    print("initializing ACE Bot")
    initializeRandomSeed()

    # define variables used for controlling motors based on controller inputs
    controller_1_left_shoulder_control_motors_stopped = True
    controller_1_right_shoulder_control_motors_stopped = True
    controller_1_up_down_buttons_control_motors_stopped = True
    controller_1_x_b_buttons_control_motors_stopped = True
    drivetrain_l_needs_to_be_stopped_controller_1 = False
    drivetrain_r_needs_to_be_stopped_controller_1 = False
    teleop_started=False
 

    # set the servo's init positions and move to starting positions
    print("initialize servos")
    tilt.set_position(TILT_DOWN_ANGLE, DEGREES)
    tilt.spin_to_position(TILT_INIT_ANGLE, DEGREES, wait=True)
    wait(750, MSEC)
    clamp.set_position(CLAMP_UP_ANGLE, DEGREES)
    clamp.spin_to_position(CLAMP_UP_ANGLE, DEGREES, wait=True)
    wait(500, MSEC)
    ladyBrownLeft.spin_to_position(LB_LEFT_DOWN_ANGLE, DEGREES, wait=False)
    ladyBrownRight.spin_to_position(LB_RIGHT_DOWN_ANGLE, DEGREES, wait=True)
    wait(750,MSEC)

    print("initialize callbacks")
    # register callbacks for buttons
    
    controller_1.buttonX.pressed(on_x_button_pressed)
    controller_1.buttonLeft.pressed(on_left_button_pressed)
    controller_1.buttonA.pressed(on_a_button_pressed)
    controller_1.buttonB.pressed(on_b_button_pressed)

    brain.screen.print("Hello ACE, Wreck Em")

# autonomous procedure
def autonomous():
    #brain.screen.clear_screen()
    brain.screen.print("Starting Autonomous")
    # place automonous code here
    print("Starting Autonomous")

# teleop procedure
def user_control():
    #brain.screen.clear_screen()
    brain.screen.print("Starting teleop")
    print("Starting teleop")
    global drivetrain_l_needs_to_be_stopped_controller_1, drivetrain_r_needs_to_be_stopped_controller_1, controller_1_left_shoulder_control_motors_stopped, controller_1_right_shoulder_control_motors_stopped, controller_1_up_down_buttons_control_motors_stopped, controller_1_x_b_buttons_control_motors_stopped, remote_control_code_enabled
    global controller_1_ladybrown_stopped, teleop_started
    global detect_blue_mode, abort_detection
    global a_vision_1
    # place driver control in this while loop

    while True:       
        # items to run only once when teleop starts
        if not teleop_started:
            tilt.spin_to_position(TILT_DOWN_ANGLE, DEGREES, wait=True)
            wait(200,MSEC)
            ladyBrownLeft.spin_to_position(LB_LEFT_INIT_ANGLE, DEGREES, wait=False)
            ladyBrownRight.spin_to_position(LB_RIGHT_INIT_ANGLE, DEGREES, wait=True)
            teleop_started = True

        # single press of Y button toggles clamp
        if status(controller_1.buttonY.pressing()):
            clampIt()

        ########################################################
        # ring detection 
        if detect_blue_mode and not abort_detection:
            blue_objects = ai_vision_1.take_snapshot(ai_vision_1__BLUEBOX)
            red_objects = ai_vision_1.take_snapshot(ai_vision_1__REDBOX)
            brain.screen.clear_screen()
            brain.screen.set_cursor(1, 1)

            # Print the amount of objects detected that matched
            # the "dark_blue" Signature using the len command.
            red_ring_width = 0
            blue_ring_width = 0
            if (blue_objects and len(blue_objects) > 0) or (red_objects and len(red_objects) > 0):
                if len(blue_objects) > 0:
                    print("BLUE RING DETECTED!")
                    for ring in blue_objects:
                        if (ring.width > blue_ring_width):
                            blue_ring_width = ring.width
                if len(red_objects) > 0:
                    print("RED RING DETECTED!")
                    for ring in red_objects:
                        if (ring.width > red_ring_width):
                            red_ring_width = ring.width
                print ("Red ring width = ", red_ring_width, "Blue ring width = ", blue_ring_width)
                if (blue_ring_width > GAME_ELEMENT_THRESHOLD) or (red_ring_width > GAME_ELEMENT_THRESHOLD):
                    print("detected RING")
                    wait(1500, MSEC)
                    hook_intake.stop()
                    ladyBrownLeft.spin_to_position(LB_LEFT_UP_ANGLE, DEGREES, wait=False)
                    ladyBrownRight.spin_to_position(LB_RIGHT_UP_ANGLE, DEGREES, wait=False)
                    detect_blue_mode = False
        ########################################################
        
        # calculate the drivetrain motor velocities from the controller joystick axies
        # left = axis3 + axis1
        # right = axis3 - axis1
        drivetrain_left_side_speed = controller_1.axis3.position() + controller_1.axis1.position()
        drivetrain_right_side_speed = controller_1.axis3.position() - controller_1.axis1.position()         

        # check if the value is inside of the deadband range
        if drivetrain_left_side_speed < 5 and drivetrain_left_side_speed > -5:
            # check if the left motor has already been stopped
            if drivetrain_l_needs_to_be_stopped_controller_1:
                # stop the left drive motor
                left_drive_smart.stop()
                # tell the code that the left motor has been stopped
                drivetrain_l_needs_to_be_stopped_controller_1 = False
        else:
            # reset the toggle so that the deadband code knows to stop the left motor next
            # time the input is in the deadband range
            drivetrain_l_needs_to_be_stopped_controller_1 = True
        # check if the value is inside of the deadband range
        if drivetrain_right_side_speed < 5 and drivetrain_right_side_speed > -5:
            # check if the right motor has already been stopped
            if drivetrain_r_needs_to_be_stopped_controller_1:
                # stop the right drive motor
                right_drive_smart.stop()
                # tell the code that the right motor has been stopped
                drivetrain_r_needs_to_be_stopped_controller_1 = False
        else:
            # reset the toggle so that the deadband code knows to stop the right motor next
            # time the input is in the deadband range
            drivetrain_r_needs_to_be_stopped_controller_1 = True

        # only tell the left drive motor to spin if the values are not in the deadband range
        if drivetrain_l_needs_to_be_stopped_controller_1:
            left_drive_smart.set_velocity(drivetrain_left_side_speed, PERCENT)
            left_drive_smart.spin(FORWARD)
        # only tell the right drive motor to spin if the values are not in the deadband range
        if drivetrain_r_needs_to_be_stopped_controller_1:
            right_drive_smart.set_velocity(drivetrain_right_side_speed, PERCENT)
            right_drive_smart.spin(FORWARD)

        if controller_1.buttonL1.pressing():
            intake.spin(FORWARD)
            controller_1_left_shoulder_control_motors_stopped = False
        elif controller_1.buttonL2.pressing():
            intake.spin(REVERSE)
            controller_1_left_shoulder_control_motors_stopped = False
        elif not controller_1_left_shoulder_control_motors_stopped:
            intake.stop()
            controller_1_left_shoulder_control_motors_stopped = True

        # set the toggle so that we don't constantly tell the motor to stop when
        # the buttons are released
        

        # check the buttonR1/buttonR2 status
        # to control hook_intake
        if controller_1.buttonR1.pressing():
            hook_intake.spin(FORWARD)
            controller_1_right_shoulder_control_motors_stopped = False
        elif controller_1.buttonR2.pressing():
            hook_intake.spin(REVERSE)
            controller_1_right_shoulder_control_motors_stopped = False
        elif not controller_1_right_shoulder_control_motors_stopped:
            hook_intake.stop()
            controller_1_right_shoulder_control_motors_stopped = True
    
        # set the toggle so that we don't constantly tell the motor to stop when
        # the buttons are released
     
        if controller_1.buttonUp.pressing():
            hang.spin(FORWARD)
            hang.set_max_torque(100,PERCENT)
            hang.set_velocity(100,PERCENT)
            controller_1_up_down_buttons_control_motors_stopped = False
        elif controller_1.buttonDown.pressing():
            hang.spin(REVERSE)
            hang.set_max_torque(100,PERCENT)
            hang.set_velocity(-100,PERCENT)
            controller_1_up_down_buttons_control_motors_stopped = False
        elif not controller_1_up_down_buttons_control_motors_stopped:
            hang.stop()
            # set the toggle so that we don't constantly tell the motor to stop when
            # the buttons are released
            controller_1_up_down_buttons_control_motors_stopped = True
    
    
        # wait before repeating the process
        wait(20, MSEC)

# create competition instance
pre_autonomous()
# add the line below back in for comp and remove the similartion lines
#comp = Competition(user_control, autonomous)

# use the lines below to similate start of auto and then user control
autonomous()
wait(500, MSEC)
user_control()