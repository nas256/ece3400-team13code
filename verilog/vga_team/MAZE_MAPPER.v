module MAZE_MAPPER(PIXEL_X, PIXEL_Y, COLOR_OUT);

input [9:0] PIXEL_X;
input [9:0] PIXEL_Y;

output reg [7:0] COLOR_OUT;

reg [7:0] grid_array [4:0][3:0];

always @ (*) begin 
  grid_array[1][0] = 8'b111_111_11;
  grid_array[3][0] = 8'b111_111_11;
  
  grid_array[0][2] = 8'b111_111_11;
  grid_array[1][3] = 8'b111_111_11;
  grid_array[2][3] = 8'b111_111_11;
  grid_array[3][3] = 8'b111_111_11;
  grid_array[4][2] = 8'b111_111_11;
 
  
  if ( PIXEL_X >= 9'd500 || PIXEL_Y >= 9'd400 ) begin
    COLOR_OUT = 8'b000_000_00;
  end else begin
	 COLOR_OUT = grid_array[PIXEL_X / 9'd100][PIXEL_Y / 9'd100];
  end
  
end


endmodule