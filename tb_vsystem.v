`timescale 1ns/1ps
module tb_vsystem (/*AUTOARG*/);

   integer status;

   initial begin 
      status = $vsystem("ls -lart ");
      $display("-I- status = %d", status);
      
   end

endmodule  
