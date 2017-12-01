/*
 * ANALOG MUX LIBRARY
 * created by Nicholas Sarkis (nas256)
 * 
 * Very simple and lightweight library for interfacing
 * with a CD4051B CMOS multiplexer
 */

#ifndef AMUX_H
#define AMUX_H

// Uncomment these lines if you wish to control
// the enable pin with the microcontroller
// (and change AMUX_EN to the appropriate pin!)
// #define AMUX_USE_ENABLE
// #define AMUX_EN 4

// Replace these with the pin numbers connected to
// pins A, B, and C on your mux!
#define AMUX_A  2
#define AMUX_B  4
#define AMUX_C  7

#define AMUX_TREASURE_1        3
#define AMUX_TREASURE_2        4
#define WALL_SENSOR_LEFT       0
#define WALL_SENSOR_FRONT      2
#define WALL_SENSOR_RIGHT      1

/*
 * Initialize the pins needed to control the analog mux
 * Also initializes the mux to use INPUT 0
 */
void initialize_amux(void);

/*
 * Change the input of the mux being forwarded to the output
 * The CD4051B has 8 inputs, so a value from 0-7 is accepted
 */
void select_input(char mux_input);

#ifdef AMUX_USE_ENABLE

/*
 * Enable the mux output
 */
void amux_enable(void);

/*
 * Disable the mux output
 */
void amux_disable(void);

#endif


#endif
