// Silent SPI slave implemented in verilog
module SPI_SLAVE(clk, reset, sck, mosi, cs, input_shiftreg, done);

input clk;    // System Clock
input reset;  // System Reset

input sck;    // SPI slave clock
input mosi;   // Data input
input cs;     // Chip select

output [15:0] input_shiftreg;
output        done;

reg       current_state = 0; // 0 - ignore, 1 - read_transaction

reg [2:0] sck_record; // store 3 previous states of SCK
always @(posedge clk)
  sck_record <= { sck_record[1:0], sck };
  
reg [2:0] cs_record; // store 3 previous states of CS
always @(posedge clk)
  cs_record <= { cs_record[1:0], cs };
  
reg [15:0] input_shiftreg;
reg [4 :0] bitcounter = 0;

wire done = (bitcounter == 16);

// Sample mosi at each sclk rising edge
//  and Handle start/stop of messages
always @(posedge clk) begin
   if (cs_record[2:1] == 2'b01) begin // rising edge (end)
	   current_state <= 0; // stop listening
		bitcounter <= 0;
		
   end else if (cs_record[2:1] == 2'b10) begin // falling edge (start)
		bitcounter <= 0; // reset bit counter
		current_state <= 1; // start listening
		
	end else if (current_state == 1'b1 && sck_record[2:1] == 2'b01) begin
		input_shiftreg = { input_shiftreg[14:0], mosi };
		bitcounter <= bitcounter + 1;
		if (bitcounter == 16)
			current_state <= 0;
			
	end else begin
		input_shiftreg <= input_shiftreg;
	end
end


endmodule