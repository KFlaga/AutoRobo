# AutoRobo
__Mobile robot controller__

Code for controller of mobile platform I built back in 2015.
Controller on Stm32f401RE Nucleo board with Cortex M4.

Robot's PCB consist of:
- power source
- MCU
- LCD screen
- keyboard - six buttons / 2 switches + multiplexer (in negative logic)
- cutom encoders based on phototransistors - turned out to have poor precision so I got rid of them
- some day maybe I'll add accelerometer/gyro for better steering and Wi-Fi module for communication
Keyboard and LCD screen allows for programming of robot's path.'

Project itself is made of low-level peripherals handlers (motor steering, timers, lcd etc.), steering computation and high-level `Module`s which are regularly updated in timer's interrupt and are responsible for performing one of tasks:
- ButtonsModule : reading button states, which are then globally accessible
- LcdModule : writing on LCD
- MenuModule : provide user interface to program robot's path
- RoboModule : executes `RoboProgram`s (which defines path for robot)

`RoboProgram` is made of `RoboInstructions` which are executed sequentially. Each instruction may define path for a robot or other action (like waiting) and is parameterized. 

Simple interfaces for `Module`, `RoboInstruction` etc. provides simple way to extend controller capabilities, like adding sensors, new paths, movement algorithms.
