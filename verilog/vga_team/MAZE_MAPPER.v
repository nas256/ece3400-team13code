`define WALL_THICKNESS 10'd3  // in px
`define TILE_SIZE      10'd70 // in px
`define IR_BUFFER      10'd20 // in px
`define MAZE_X_SIZE    10'd4
`define MAZE_Y_SIZE    10'd5

// Define colors
`define COLOR_7kHz        8'b111_000_00 // red
`define COLOR_12kHz       8'b000_111_00 // green
`define COLOR_17kHz       8'b000_000_11 // blue

`define COLOR_EXPLORED    8'b111_111_11 // white
`define COLOR_UNEXPLORED  8'b010_010_01 // grey
`define COLOR_CURRENT     8'b111_100_00 // yellow

`define COLOR_WALLS       8'b101_00_000 // red

module MAZE_MAPPER(CLK, PIXEL_X, PIXEL_Y_IN, COLOR_OUT, DATA_IN, DATA_VAL, GPIO_1_D);

input        CLK;

input [9:0]  PIXEL_X;
input [9:0]  PIXEL_Y_IN;
input [15:0] DATA_IN;
input        DATA_VAL;

output [33:0] GPIO_1_D;

output reg [7:0] COLOR_OUT;

reg [11:0] grid_array [4:0][3:0];

reg [9:0] PIXEL_Y;
 
reg play_sound;

always @ (posedge CLK) begin
	if (DATA_VAL) begin
		grid_array[ 3'h7 & (DATA_IN >> 11) ][ 3'h3 & (DATA_IN >> 14) ] <= DATA_IN[11:0];
		play_sound <= DATA_IN[0];
	end
end

reg [4:0] tilex;
reg [3:0] tiley;
reg [9:0] tilex_pixel;
reg [9:0] tiley_pixel;

always @ (*) begin   
  PIXEL_Y = 10'd480 - PIXEL_Y_IN;
  //PIXEL_Y = PIXEL_Y_IN;

  tilex       = PIXEL_X / `TILE_SIZE;
  tiley       = PIXEL_Y / `TILE_SIZE;
  tilex_pixel = tilex   * `TILE_SIZE;
  tiley_pixel = tiley   * `TILE_SIZE;
  
  if ( PIXEL_X >= (`MAZE_X_SIZE*`TILE_SIZE) || PIXEL_Y >= (`MAZE_Y_SIZE*`TILE_SIZE) ) begin
    COLOR_OUT = 8'b000_000_00;
  end else begin
	 //COLOR_OUT = grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100];
	 
	 // Draw the tiles themselves
	 if (grid_array[PIXEL_Y / `TILE_SIZE][PIXEL_X / `TILE_SIZE] & (16'd1 << 1)) begin
		COLOR_OUT = `COLOR_CURRENT;
	 end else if (grid_array[PIXEL_Y / `TILE_SIZE][PIXEL_X / `TILE_SIZE] & (16'd1 << 2)) begin
		COLOR_OUT = `COLOR_EXPLORED;
	 end else begin
		COLOR_OUT = `COLOR_UNEXPLORED;
	 end
  end
  

  // Draw walls
  // Protocol: [bit, wall]: [3,  top] [5, bottom] [6, left] [4, right]
  if ( PIXEL_X <= (`MAZE_X_SIZE*`TILE_SIZE) && PIXEL_Y <= (`MAZE_Y_SIZE*`TILE_SIZE) ) begin
	  if ( PIXEL_X <= tilex_pixel + `WALL_THICKNESS ) begin // left wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 6) ) begin
			COLOR_OUT = `COLOR_WALLS; // walls are pink
		 end
	  end else if ( PIXEL_X >= tilex_pixel + `TILE_SIZE - `WALL_THICKNESS ) begin // right wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 4) ) begin
			COLOR_OUT = `COLOR_WALLS; // walls are pink
		 end
	  end else if ( PIXEL_Y <= tiley_pixel + `WALL_THICKNESS ) begin // top wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 5) ) begin
			COLOR_OUT = `COLOR_WALLS; // walls are pink
		 end
	  end else if ( PIXEL_Y >= tiley_pixel + `TILE_SIZE - `WALL_THICKNESS ) begin // bottom wall
		  if ( grid_array[tiley][tilex] & (16'd1 << 3) ) begin
			COLOR_OUT = `COLOR_WALLS; // walls are pink
		 end
	  end
  end
  
  // Draw the IR beacons
  if ( PIXEL_X <= (`MAZE_X_SIZE*`TILE_SIZE) && PIXEL_Y <= (`MAZE_Y_SIZE*`TILE_SIZE) ) begin
    if ( PIXEL_X >= tilex_pixel + `IR_BUFFER && PIXEL_X <= tilex_pixel + `TILE_SIZE - `IR_BUFFER ) begin
	   if ( PIXEL_Y >= tiley_pixel + `IR_BUFFER && PIXEL_Y <= tiley_pixel + `TILE_SIZE - `IR_BUFFER  ) begin
		  // In the draw zone for IR, choose color based on IR reading
		  case ( (grid_array[tiley][tilex] >> 9 ) & 2'd3 )
		    2'b01:   COLOR_OUT = `COLOR_7kHz;
		    2'b10:   COLOR_OUT = `COLOR_12kHz;
			 2'b11:   COLOR_OUT = `COLOR_17kHz;
			 default: COLOR_OUT =  COLOR_OUT;
		  endcase
		end
	 end
  end
  
  
  // Draw the done signal if done
  if (PIXEL_X >= 10'd500 && PIXEL_X <= 10'd550) begin
    if (play_sound) COLOR_OUT = 8'b000_11_000; 
  end
end

// Audio integration
AUDIO audio(
  .CLOCK_50(CLK),
  .ENABLE( play_sound ),
  .GPIO_1_D( GPIO_1_D ),
);


endmodule