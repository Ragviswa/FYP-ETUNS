module UnaryTestbench;

    parameter INPUT_WIDTH = 8;
    parameter INPUT_SPACE = 2 ** INPUT_WIDTH;
    parameter COUNT_WIDTH = $clog2(INPUT_WIDTH + 1);
    parameter EPSILON = 16;

    logic a;
    logic b;
    logic [1:0] ready;
    logic clk;
    logic reset;
    logic valid;
    logic y;

    int fd;
    string line;

    logic [(INPUT_WIDTH - 1):0] a_stream;
    logic [(INPUT_WIDTH - 1):0] b_stream;
    logic [(INPUT_WIDTH - 1):0] y_stream;
    logic [(INPUT_WIDTH - 1):0] y_stream_input;
    logic [(COUNT_WIDTH - 1):0] y_stream_count;
    
    // Instantiate design under test
    UnaryMultiplier #(
            .INPUT_WIDTH(INPUT_WIDTH),
            .COUNT_WIDTH(COUNT_WIDTH)
            // .EPSILON(EPSILON)
        ) DUT (
        .a(a),
      	.b(b),
      	.ready(ready),
      	.clk(clk),
      	.reset(reset),
      	.valid(valid),
      	.y(y)
    );

    initial begin 
        fd = $fopen ("./data.csv", "w");
        clk = 0;
        reset = 0;
        a_stream = 0;
        b_stream = 0;
        y_stream = 0;
        y_stream_count = 0;
        #10
        for (int ai = 0; ai < INPUT_SPACE; ai++) begin
            for (int bi = 0; bi < INPUT_SPACE; bi++) begin
                reset = 0;
                #10
                reset = 1;
                y_stream = 0;
                y_stream_count = 0;
                a = 1'b0; b = 1'b0; ready = 2'b00;
                #10
                for (int i = 0; i < INPUT_WIDTH; i++) begin
                    #10 a = a_stream[i]; b = b_stream[i]; ready = 2'b11;
                    y_stream = valid ? (y_stream | (y << y_stream_count)) : y_stream;
                    y_stream_count = valid ? y_stream_count + 1'b1 : y_stream_count;
                end
                for (int i = 0; i <= INPUT_WIDTH; i++) begin
                    #10 a = 1'b0; b = 1'b0; ready = 2'b00;
                    y_stream = valid ? (y_stream | (y << y_stream_count)) : y_stream;
                    y_stream_count = valid ? y_stream_count + 1'b1 : y_stream_count;
                end
                $fdisplay (fd, "%b,%b,%b,1,16.0", a_stream, b_stream, y_stream);

                b_stream = b_stream + 1'b1;
            end
            a_stream = a_stream + 1'b1;
        end
        $fclose(fd);
        $finish;
    end

    always #5 clk = ~clk;

endmodule