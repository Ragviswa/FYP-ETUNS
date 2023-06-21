module UnarySquareRoot (
    input logic a,
    input logic ready,
    input logic clk,
    input logic reset,
    output logic valid,
    output logic y
);
    parameter INPUT_WIDTH = 32;
    parameter COUNT_WIDTH = $clog2(INPUT_WIDTH + 1);

    logic [(COUNT_WIDTH - 1):0] a_ones;
    logic [(COUNT_WIDTH - 1):0] y_ones;
    logic [(COUNT_WIDTH - 1):0] a_count;
    logic [(COUNT_WIDTH - 1):0] y_count;

    logic [(COUNT_WIDTH - 1):0] a_upper_ones;
    logic [(COUNT_WIDTH - 1):0] a_lower_ones;
    logic [(COUNT_WIDTH - 1):0] y_upper_pre_ones;
    logic [(COUNT_WIDTH - 1):0] y_lower_pre_ones;
    logic [(COUNT_WIDTH - 1):0] y_upper_post_ones;
    logic [(COUNT_WIDTH - 1):0] y_lower_post_ones;
    logic [(COUNT_WIDTH - 1):0] y_upper_count;
    logic [(COUNT_WIDTH - 1):0] y_lower_count;
    
    always_comb begin
        a_upper_ones = y_upper_ones * y_upper_ones;
        a_lower_ones = y_lower_ones * y_lower_ones;
        y_lower_post_ones = y_lower_pre_ones * y_lower_count;
        y_upper_post_ones = y_upper_pre_ones * y_upper_count;
    end
    
    always_ff @(posedge clk or negedge reset) begin
        if (!reset) begin
            a_ones <= 8'b0;
            y_ones <= 8'b0;
            a_count <= 8'b0;
            y_count <= 8'b0;
            y_lower_pre_ones <= 8'b0;
            y_upper_pre_ones <= 8'b0;
            y_lower_pre_count <= 8'b0;
            y_upper_pre_count <= 8'b0;
            y <= 1'b0;
            valid <= 1'b0;
        end else begin
            if (ready) begin
                a_ones <= a_ones + a;
                a_count <= a_count + 1'b1;
                if (a_ones - a_lower_ones > a_upper_ones - a_ones) begin
                    y_lower_pre_ones <= y_lower_pre_ones + 1'b1;
                    y_upper_pre_ones <= y_upper_pre_ones + 1'b1;
                end
                if (y_ones <= y_lower_ones) begin
                    y <= 1'b1;
                    y_ones <= y_ones + 1'b1;
                    valid <= 1'b1;
                    y_count <= y_count + 1'b1;
                end else if (y_ones >= y_upper_ones) begin
                    y <= 1'b0;
                    y_ones <= y_ones + 1'b1;
                    valid <= 1'b1;
                    y_count <= y_count + 1'b1;
                end else begin
                    valid <= 1'b0;
                end
            end
        end
    end
endmodule