module UnaryTestbench;

    logic a;
    logic b;
    logic [1:0] ready;
    logic clk;
    logic reset;
    logic valid;
    logic y;
    logic [7:0] y_count;
    logic [7:0] a_count;
    logic [7:0] b_count;
    logic [7:0] a_ones;
    logic [7:0] b_ones;

    // Instantiate design under test
    UnaryAdder DUT(
        .a(a),
      	.b(b),
      	.ready(ready),
      	.clk(clk),
      	.reset(reset),
      	.valid(valid),
      	.y(y),
        .y_count(y_count),
        .a_count(a_count),
        .b_count(b_count),
        .a_ones(a_ones),
        .b_ones(b_ones)
    );

    initial begin 
        $display("Release reset.");
        clk = 0;
        reset = 0;
        #10
        reset = 1;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b1; b = 1'b0; ready = 2'b11;
        display;
        #10 a = 1'b0; b = 1'b0; ready = 2'b00;
        display;
        #10
        display;
        #10
        display;
        #10
        display;
        #10
        display;
        #10
        display;
        $stop; 
    end

    always #5 clk = ~clk;

    task display;
        $display("a:%0h, b:%0h, valid:%0h, y:%0h, clk:%0h", a, b, valid, y, clk);
        $display("y_count:%0h, a_count:%0h, b_count:%0h, a_ones:%0h, b_ones", y_count, a_count, b_count, a_ones, b_ones);
    endtask

endmodule