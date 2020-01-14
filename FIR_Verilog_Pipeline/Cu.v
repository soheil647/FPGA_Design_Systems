module CU 
	(
	input  clk,rst,ready,
	input signed[15:0]N,
	output reg done,write,read,init_reg,ld,clear_K,cn_N,cn_K,clear_N
    );

  reg [2:0]ps,ns;
  parameter [2:0] one=3'b000,two=3'b001,three=3'b010, four=3'b011, five = 3'b100;
  always@( ps , N  )begin
    ns=one;
    case(ps)
      one   : ns = (ready == 1'b1) ? two : one ;
      two   : ns = three  ; 
      three : ns = four  ;
      four  : ns = ((N-1)<0) ? five : four ;
      five  : ns = one ;
    endcase 
  end
  always@(ps)begin
    
		done=0;write=0;read=0;init_reg=0;ld=0;cn_N=0;cn_K=0;clear_K=0;clear_N=0;

    case (ps)
      
      one : begin clear_K = 1'b1;
      end
      two : begin write=1'b1 ; init_reg=1'b1 ;
      end
      three : begin read=1'b1 ;
      end
      four :  begin ld=1'b1; cn_K= 1'b1;read=1'b1;
      end
      five :  begin done=1'b1; cn_N=1'b1;
      end
    endcase 
  end
  
  always@(posedge clk, posedge rst ) begin 
    if(rst ) ps <= 3'b000;
    else ps <= ns;
    end
  endmodule
      
      
 

 


