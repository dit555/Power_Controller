## Power Allocation on an Imperial-Class Star Destroyer


An Imperial Star Destroyer has a limited source of power. As a result, it is important to allocate power in such a way that all required systems are able to get sufficient power when turned on by ship command. It is also important to not allow power to be overdrawn as that could cause many problems. The bridge has a panel of 8 switches dedicated to handling power distribution to the main functions of the ship. These functions are: life support, attack systems, engine throttle, and deflector shields. These inputs are fed to a microcontroller that diverts power to these systems according to the input received.
The microcontroller will have a period of 100ms in the final product but the period is increased to 1 s for demonstration and testing purposes. The input will be received through port A where: A0 turns off power for life support systems, A1 diverts power to the attack functions on the targeting computer, A2-A4 divert power to engine thrust, A5 increases power sent to the deflector shields, A6 turns off the engines, A7 turns off the deflector shields. In addition, the microcontroller also calculates power left and outputs the integer value to B (0 to 100) through the power manager. In the event that there is insufficient power the system will be forced to idle.







## Life Support:
By default is in the On state and drains 5% of power  
Can be turned off if absolutely necessary but risks the wellbeing of the crew if left off for long periods of time.  

## Attack Systems:
By default on idle state where power drain is negligible  
When power is supplied they will begin the attack cycle  
When detecting the target's position, 5% power is drained by the tractor beam targeting array.  
All available weapon systems are aimed at the target and doing so drains 10% power  
When firing the maximum power use of 20% is reserved for all armaments.  

## Engine Systems:
By default the engines are idling using 1% of power  
Toggling only A2 will cause the engines to fire, draining 5% power.  
Toggling A2A3 will cause the engines’ afterburners to activate draining 20% power giving a large increase in thrust but at the cost of efficiency.  
Toggling A2-A4 will cause the ship’s Class 2 Hyperdrive to activate and the ship will jump to Hyperspace draining 50% power.  
In the event of a glitch and the above inputs are not A2-A4 = 100, 110, or 111 the engines are forced to idle to prevent damage to assets and crew.  
If A6 is toggled the engines will be turned off completely draining no power regardless of the state of A2-A4.  

## Deflector Shields:
* By default the deflector shield is idle and drains 10% power.  
* Toggling A5 will switch the DS into combat mode draining 40% power  
* Toggling A7 when A5 is off will turn off the DS to drain no power.  

## Power Manager:
Calculates the percent of power available for use and outputs it to B.  


