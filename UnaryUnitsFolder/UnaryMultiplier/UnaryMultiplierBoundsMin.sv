module UnaryMultiplier
#(
    parameter INPUT_WIDTH = 32,
    parameter COUNT_WIDTH = $clog2(INPUT_WIDTH + 1),
    parameter EPSILON = 0
)
(
    input logic a,
    input logic b,
    input logic [1:0] ready,
    input logic clk,
    input logic reset,
    output logic valid,
    output logic y
);

    logic [(COUNT_WIDTH - 1):0] a_ones;
    logic [(COUNT_WIDTH - 1):0] b_ones;
    logic [(COUNT_WIDTH - 1):0] y_ones;
    logic [(COUNT_WIDTH - 1):0] a_count;
    logic [(COUNT_WIDTH - 1):0] b_count;
    logic [(COUNT_WIDTH - 1):0] y_count;

    logic [(COUNT_WIDTH - 1):0] a_upper_ones;
    logic [(COUNT_WIDTH - 1):0] b_upper_ones;
    logic [(COUNT_WIDTH - 1):0] a_lower_ones;
    logic [(COUNT_WIDTH - 1):0] b_lower_ones;
    logic [COUNT_WIDTH:0] y_upper_ones;
    logic [COUNT_WIDTH:0] y_lower_ones;
    logic [COUNT_WIDTH:0] y_midpoint_ones;
    logic [COUNT_WIDTH:0] y_distance_upper;
    logic [COUNT_WIDTH:0] y_distance_lower;
    logic [2*(COUNT_WIDTH - 1):0] y_prod_upper_ones;
    logic [2*(COUNT_WIDTH - 1):0] y_prod_lower_ones;
    
    always_comb begin
        a_upper_ones = INPUT_WIDTH - a_count + a_ones;
        a_lower_ones = a_ones;
        b_upper_ones = INPUT_WIDTH - b_count + b_ones;
        b_lower_ones = b_ones;
        y_prod_upper_ones = a_upper_ones * b_upper_ones;
        y_prod_lower_ones = a_lower_ones * b_lower_ones;
        y_upper_ones = y_prod_upper_ones >> (COUNT_WIDTH - 2);
        y_lower_ones = y_prod_lower_ones >> (COUNT_WIDTH - 2);
        y_midpoint_ones = (INPUT_WIDTH - y_count + (y_ones << 1'b1));
        y_distance_upper = y_upper_ones - y_midpoint_ones;
        y_distance_lower = y_midpoint_ones - y_lower_ones;
    end
    
    always_ff @(posedge clk or negedge reset) begin
        if (!reset) begin
            a_ones <= 0;
            b_ones <= 0;
            y_ones <= 0;
            a_count <= 0;
            b_count <= 0;
            y_count <= 0;
            y <= 0;
            valid <= 0;
        end else begin
            if (y_count == INPUT_WIDTH) begin
                y <= 1'b0;
                valid <= 1'b0;
            end else begin
                if (ready[0]) begin
                    a_ones <= a_ones + a;
                    a_count <= a_count + 1'b1;
                end
                if (ready[1]) begin
                    b_ones <= b_ones + b;
                    b_count <= b_count + 1'b1;
                end
                if (|a_count || |b_count) begin
                    if (y_midpoint_ones <= y_lower_ones) begin
                        y <= 1'b1;
                        y_ones <= y_ones + 1'b1;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else if (y_midpoint_ones >= y_upper_ones) begin
                        y <= 1'b0;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else begin
                        valid <= 1'b0;
                    end
                end
            end
        end
    end
endmodule