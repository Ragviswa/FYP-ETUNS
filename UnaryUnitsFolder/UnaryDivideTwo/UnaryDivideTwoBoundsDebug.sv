module UnaryDivideTwo 
#(
    parameter INPUT_WIDTH = 32,
    parameter COUNT_WIDTH = $clog2(INPUT_WIDTH + 1),
    parameter EPSILON = 0
)
(
    input logic a,
    input logic ready,
    input logic clk,
    input logic reset,
    output logic [(COUNT_WIDTH - 1):0] a_ones, y_ones, a_count, y_count,
    output logic [(COUNT_WIDTH - 1):0] a_upper_ones, a_lower_ones,
    output logic [COUNT_WIDTH:0] y_upper_ones, y_lower_ones, y_midpoint_ones, y_ones_p_epsilon, y_ones_m_epsilon,
    output logic valid,
    output logic y
);

    // logic [(COUNT_WIDTH - 1):0] a_ones;
    // logic [(COUNT_WIDTH - 1):0] y_ones;
    // logic [(COUNT_WIDTH - 1):0] a_count;
    // logic [(COUNT_WIDTH - 1):0] y_count;

    // logic [(COUNT_WIDTH - 1):0] a_upper_ones;
    // logic [(COUNT_WIDTH - 1):0] a_lower_ones;
    // logic [COUNT_WIDTH:0] y_upper_ones;
    // logic [COUNT_WIDTH:0] y_lower_ones;
    // logic [COUNT_WIDTH:0] y_midpoint_ones;
    // logic [COUNT_WIDTH:0] y_ones_p_epsilon;
    // logic [COUNT_WIDTH:0] y_ones_m_epsilon;
    logic [COUNT_WIDTH:0] y_distance_upper;
    logic [COUNT_WIDTH:0] y_distance_lower;
    
    always_comb begin
        a_upper_ones = INPUT_WIDTH - a_count + a_ones;
        a_lower_ones = a_ones;
        y_upper_ones = a_upper_ones;
        y_lower_ones = a_lower_ones;
        y_midpoint_ones = (INPUT_WIDTH - y_count + (y_ones << 1'b1));
        y_ones_p_epsilon = y_midpoint_ones + EPSILON >= (INPUT_WIDTH << 1'b1) ? (INPUT_WIDTH << 1'b1) : y_midpoint_ones + EPSILON;
        y_ones_m_epsilon = EPSILON >= y_midpoint_ones ? 0 : y_midpoint_ones - EPSILON;
        y_distance_upper = y_upper_ones - y_midpoint_ones;
        y_distance_lower = y_midpoint_ones - y_lower_ones;
    end
    
    always_ff @(posedge clk or negedge reset) begin
        if (!reset) begin
            a_ones <= 0;
            y_ones <= 0;
            a_count <= 0;
            y_count <= 0;
            y <= 0;
            valid <= 0;
        end else begin
            if (y_count == INPUT_WIDTH) begin
                y <= 1'b0;
                valid <= 1'b0;
            end else begin
                if (ready) begin
                    a_ones <= a_ones + a;
                    a_count <= a_count + 1'b1;
                end
                if (|a_count) begin
                    if (y_midpoint_ones <= y_lower_ones) begin
                        y <= 1'b1;
                        y_ones <= y_ones + 1'b1;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else if (y_midpoint_ones >= y_upper_ones) begin
                        y <= 1'b0;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1; 
                    end else if (y_ones_m_epsilon <= y_lower_ones && y_ones_p_epsilon < y_upper_ones) begin
                        y <= 1'b1;
                        y_ones <= y_ones + 1'b1;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else if (y_ones_p_epsilon >= y_upper_ones && y_ones_m_epsilon > y_lower_ones) begin
                        y <= 1'b0;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else if (y_ones_m_epsilon <= y_lower_ones && y_ones_p_epsilon >= y_upper_ones && y_distance_lower <= y_distance_upper) begin
                        y <= 1'b1;
                        y_ones <= y_ones + 1'b1;
                        valid <= 1'b1;
                        y_count <= y_count + 1'b1;
                    end else if (y_ones_m_epsilon <= y_lower_ones && y_ones_p_epsilon >= y_upper_ones && y_distance_lower >= y_distance_upper) begin
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