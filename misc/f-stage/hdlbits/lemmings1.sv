module top_module (
    input clk,
    input areset,  // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    output walk_left,
    output walk_right
);

    localparam LEFT = 0;
    localparam RIGHT = 1;
    reg state, next_state;

    always @(*) begin
        if (bump_left && bump_right) begin
            next_state = ~state;  // change direction
        end else if (bump_left) begin
            next_state = RIGHT;
        end else if (bump_right) begin
            next_state = LEFT;
        end else begin
            next_state = state;
        end
    end

    always @(posedge clk or posedge areset) begin
        if (areset) begin
            state <= LEFT;
        end else begin
            state <= next_state;
        end
    end

    // Output logic
    assign walk_left  = (state == LEFT);
    assign walk_right = (state == RIGHT);

endmodule
