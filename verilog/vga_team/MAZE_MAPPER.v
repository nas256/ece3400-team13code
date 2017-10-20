module MAZE_MAPPER(CLK, PIXEL_X, PIXEL_Y, COLOR_OUT, DATA_IN, DATA_VAL);

input        CLK;

input [9:0]  PIXEL_X;
input [9:0]  PIXEL_Y;
input [15:0] DATA_IN;
input        DATA_VAL;

output reg [7:0] COLOR_OUT;

reg [7:0] grid_array [3:0][4:0];

always @ (posedge CLK) begin
	if (DATA_VAL) begin
		grid_array[ 2'h3 & (DATA_IN >> 11) ][ 3'h7 & (DATA_IN >> 13) ] = DATA_IN[7:0];
	end
end

always @ (*) begin   
  if ( PIXEL_X >= 9'd500 || PIXEL_Y >= 9'd400 ) begin
    COLOR_OUT = 8'b000_000_00;
  end else begin
	 //COLOR_OUT = grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100];
	 
	 if (grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100] & (16'd1 << 1)) begin
		COLOR_OUT = 8'b111_100_00; //yellow
	 end else if (grid_array[PIXEL_Y / 9'd100][PIXEL_X / 9'd100] & (16'd1 << 2)) begin
		COLOR_OUT = 8'b111_111_11; // white -> previously explored
	 end else begin
		COLOR_OUT = 8'b010_010_01;//grey -> unexplored
	 end
  end
  
end


endmodule