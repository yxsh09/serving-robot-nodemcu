# Testing Procedure

## Test 1 - Power
1. Keep robot wheels lifted.
2. Power the system.
3. Confirm red LED indicates power.
4. LCD should show `Serving Robot / NodeMCU Ready`.

## Test 2 - Motors
1. Test left motor direction.
2. Test right motor direction.
3. If a motor runs backward, reverse its two motor wires or invert the software direction.

## Test 3 - IR line following
1. Place the robot over a black line on a light surface.
2. Observe left, center and right sensor states.
3. Adjust sensor height and potentiometers.
4. Tune `BASE_SPEED` and `TURN_SPEED`.

## Test 4 - Obstacle detection
1. Place an object in front of the robot.
2. The robot should stop.
3. The buzzer should sound.
4. Remove the obstacle.
5. The robot should resume.

## Test 5 - RFID
1. Read each tag UID through Serial Monitor.
2. Put the UID into `config.h`.
3. Select the corresponding table.
4. Place the tag at the table checkpoint.
5. Robot should stop and display `DELIVERED`.

## Test 6 - Full delivery
1. Select table 1.
2. Place the robot on the starting line.
3. Confirm line following.
4. Confirm obstacle stop if an obstacle is inserted.
5. Confirm RFID table detection.
6. Confirm delivery indication.
7. Repeat for tables 2-4.

## Safety

Do not test the robot near people or food until the drive system is mechanically stable. Add an accessible emergency-stop switch for a physical prototype.
