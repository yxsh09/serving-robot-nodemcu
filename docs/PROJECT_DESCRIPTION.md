# Project Description

## Title
Serving Robot for Restaurant Using NodeMCU

## Objective
Develop a compact mobile robot that can carry food from the kitchen and deliver it to a selected restaurant table using line following and table identification.

## Working principle

1. The operator selects a destination table.
2. NodeMCU starts the robot.
3. IR sensors detect the floor line.
4. Motor speed and direction are changed to keep the robot on the line.
5. The ultrasonic sensor checks the path for obstacles.
6. The RFID reader identifies the table checkpoint.
7. When the RFID UID matches the selected table, the robot stops.
8. LCD and buzzer provide status feedback.
9. The system returns to idle for the next order.

## Extension ideas

- Add a web/mobile ordering system.
- Send order data over Wi-Fi.
- Add multiple order queues.
- Add wheel encoders for better distance control.
- Add PID line following.
- Add battery voltage monitoring.
- Add an emergency-stop input.
- Add automatic charging/docking.
