Purpose:
Program to test GPIO modules

Running mode:
* Compile, Flash the program and reset.

Note:																									  
The example sets up few pins for GPIO input event and interrupt test.
Various kinds of trigger conditions have been configured to various pins. Check code comments for the description of each pin.

Output:
Based on the input interrupt received on the GPIO ports, the interrupt edge(rising/falling edge or level) 
is detected and the respective register(SIENR-Set Pin Interrupt Enable (Rising) register,SIENF-Set Pin Interrupt Enable Falling Edge) is set.
