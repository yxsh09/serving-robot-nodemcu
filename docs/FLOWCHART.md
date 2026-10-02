# Software Flowchart

```text
                 +-----------+
                 |   START   |
                 +-----+-----+
                       |
                       v
              +------------------+
              | Initialize MCU,  |
              | sensors, LCD,    |
              | RFID and motors  |
              +--------+---------+
                       |
                       v
              +------------------+
              | Select table     |
              | using keypad     |
              +--------+---------+
                       |
                       v
              +------------------+
              | Start delivery   |
              +--------+---------+
                       |
                       v
              +------------------+
              | Follow line with |
              | IR sensors       |
              +--------+---------+
                       |
              +--------+---------+
              | Obstacle?        |
              +----+--------+----+
                   | YES    | NO
                   v        |
             +-----------+  |
             | Stop +    |  |
             | buzzer    |  |
             +-----+-----+  |
                   |        |
             path clear?   |
                   |        |
                   +--------+
                       |
                       v
              +------------------+
              | Read RFID tag    |
              +--------+---------+
                       |
                +------+------+
                | Match table?|
                +---+-----+---+
                    | YES | NO
                    v     |
              +---------+ |
              | Stop    | |
              | Deliver | |
              +----+----+ |
                   |      |
                   v      |
              +---------+ |
              |  IDLE   |<+
              +---------+
```
