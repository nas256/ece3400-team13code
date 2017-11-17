`define WALL_THICKNESS 9'd3

module MAZE_MAPPER(CLK, PIXEL_X, PIXEL_Y, COLOR_OUT, DATA_IN, DATA_VAL, GPIO_1_D);

input        CLK;

input [9:0]  PIXEL_X;
input [9:0]  PIXEL_Y;
input [15:0] DATA_IN;
input        DATA_VAL;

output [33:0] GPIO_1_D;

output reg [7:0] COLOR_OUT;

reg [7:0] grid_array [3:0][4:0];

reg play_sound;

always @ (posedge CLK) begin
	if (DATA_VAL) begin
		grid_array[ 2'h3 & (DATA_IN >> 11) ][ 3'h7 & (DATA_IN >> 13) ] <= DATA_IN[7:0];
		play_sound <= DATA_IN[0];
	end
end

reg [4:0] tilex;
reg [3:0] tiley;
reg [9:0] tilex_pixel;
reg [9:0] tiley_pixel;

always @ (*) begin   
  tilex       = PIXEL_X / 9'd100;
  tiley       = PIXEL_Y / 9'd100;
  tilex_pixel = tilex   * 9'd100;
  tiley_pixel = tiley   * 9'd100;
  
  if ( PIXEL_X >= 9'd500 || PIXEL_Y >= 9'd400 ) begin
    COLOR_OUT = 8'b000_000_00;
  end else begin
	 //COLOR_OUT = grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100];
	 
	 // Draw the tiles themselves
	 if (grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100] & (16'd1 << 1)) begin
		COLOR_OUT = 8'b111_100_00; // yellow
	 end else if (grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100] & (16'd1 << 2)) begin
		COLOR_OUT = 8'b111_111_11; // white -> previously explored
	 end else begin
		COLOR_OUT = 8'b010_010_01; //grey -> unexplored
	 end
  end
  

  // Draw walls
  // Protocol: [bit, wall]: [3,  top] [5, bottom] [6, left] [4, right]
  if ( PIXEL_X <= 9'd500 && PIXEL_Y <= 9'd400 ) begin
	  if ( PIXEL_X <= tilex_pixel + `WALL_THICKNESS ) begin // left wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 6) ) begin
			COLOR_OUT = 8'b101_00_000; // walls are pink
		 end
	  end else if ( PIXEL_X >= tilex_pixel + 9'd100 - `WALL_THICKNESS ) begin // right wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 4) ) begin
			COLOR_OUT = 8'b101_00_000; // walls are pink
		 end
	  end else if ( PIXEL_Y <= tiley_pixel + `WALL_THICKNESS ) begin // top wall
		 if ( grid_array[tiley][tilex] & (16'd1 << 3) ) begin
			COLOR_OUT = 8'b101_00_000; // walls are pink
		 end
	  end else if ( PIXEL_Y >= tiley_pixel + 9'd100 - `WALL_THICKNESS ) begin // bottom wall
		  if ( grid_array[tiley][tilex] & (16'd1 << 5) ) begin
			COLOR_OUT = 8'b101_00_000; // walls are pink
		 end
	  end
  end
  
  // Draw the done signal if done
  if (PIXEL_X >= 10'd550 && PIXEL_X <= 10'd600) begin
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