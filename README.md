# Engine-Temperature-Monitoring-on-ARM-Cortex-M4-
The project consists of two microcontrollers (both are tiva c), they are connected using CAN. First one is for data gathering from temperature sensor and the other to communicate with PC.

***System	Overview***

The objective of this system is to monitor the temperature of the motor and act upon any faults in the system.
2 ECUs will be communicating through CAN, ECU1 is responsible for reporting the system health to the PC and ECU2 is responsible for monitoring the motor and reporting to ECU1.
ECU1 will send a keep alive message(check CAN communication) and will wait for ECU2 response.
ECU2 is responsible for monitoring the motor and reporting to ECU1.
If ECU1 doesn’t get “normal” temperature readings from ECU2 for 3 seconds, ECU1 will send an ADC health check remote frame.
If either ECU doesn’t get the Keep alive message the ECU goes to communication,lost mode(Blinking Blue led for 10 seconds waiting on communication to be restored).
DTCs are expected and will be further explained.

***UART to PC connections***
• ECU1 shall be connected to PC through UART and the messages sent to PC should indicate which state ECU2 is running along with indicating any error message sent from ECU2 
• UART DEMO read from PC connected to ECU1: 
      ✓ ECU1: System in fault state(explained further)
      ✓ ECU2: Error Communication Lost 
      ✓ ECU2: Average temperature 
      ✓ Motor Overheated(explained further)

***Fault checks and reactions***
• ECU2 is connected to both a known voltage and a temperature sensor to simulate the motor control environment.
• If ECU1 thinks the temperature is very high or not within the “normal” expected temperatures for 3 seconds ECU1 will send a remote frame to ECU2 to check the readings of the “known voltage”, since this voltage is known to the system, if ECU2 sends anything outside the expected voltage, ECU1 will put the system in fault state(blink red led on both ECUs and stop communication until DTC is cleared), sending over the UART that the system is in Fault state
• If ECU2 sends the known voltage correctly then the fault is in the Motor and the motor should be shut down immediately, ECU1 will send the UART that motor is overheated, and both ECUs will blink white LED for 10 seconds unless DTC is raised.
• Under normal conditions ECU2 should have the green LED on as long as the temperature readings are within the expected range(since this is a simulation to motor environment the normal temps can be from 10 degree Celsius – 25 degree Celsius)
• If either ECU doesn’t get the keep alive communication message for 5 seconds, the ECU goes to communication lost mode(Blinking Blue led for 10 seconds waiting on communication to be restored)
• NOTE:
Only ECU1 gets to decide if the temperature readings are within “normal” or not.
***Fault checks and their saving in non-volatile memory***
• If ECU1 detects Overheat 3 times(3 consecutive times) in the system a DTC will be saved in non-volatile memory and both ECUs will stay in “overheat” until SW1 is pressed on both ECUs(simulation for check engine).
• If system goes to Communication lost state 3 times(keep alive check fails 3 consecutive times), a DTC will be saved in nonvolatile memory and both ECUs will stay in “Communication lost state” until SW2 is pressed on both ECUs(simulation for check engine).
• If system detects fault state, a DTC will be saved and will stay in fault state until SW1 &2 are pressed on both ECUs(simulation for check engine).

***Functional Requirements***
Application and SWCs requirements:
-  ECU 1 has the following functionality:
        a. Report system data to PC over UART each 1 second  
        b. ECU1 sends a CAN keep alive message each 100 ms
        c. ECU1 will perform ADC health check when needed
        d. ECU1 will check the ADC readings and determine if the temperature readings are within normal or not

-  ECU 2 has the following functionality:
        a. Monitoring motor temperature
        b. Process the ADC readings and send them to ECU1 over CAN each 500ms.
        c. ADC readings should be averaged before sending
   
Any timings that are not mentioned shall be defined and OS shall
act accordingly

