module top_module (
    input  clk,
    input  areset,
    input  bump_left,
    input  bump_right,
    input  ground,
    output walk_left,
    output walk_right,
    output aaah
);

    enum {
        FALLING,
        GROUNDING
    } ground_or_fall;
    always @(posedge clk or posedge areset) begin
        if (areset) begin
            ground_or_fall <= GROUNDING;
        end else begin
            ground_or_fall <= ground ? GROUNDING : FALLING;
        end
    end

    wire is_and_will_ground = ground_or_fall == GROUNDING && ground;
    enum {
        LEFT,
        RIGHT
    } direction;
    always @(posedge clk or posedge areset) begin
        if (areset) begin
            direction <= LEFT;
        end else if (is_and_will_ground && bump_left && bump_right) begin
            direction <= direction == LEFT ? RIGHT : LEFT;  // change the direction
        end else if (is_and_will_ground && bump_left) begin
            direction <= RIGHT;
        end else if (is_and_will_ground && bump_right) begin
            direction <= LEFT;
        end
    end

    assign walk_left = (direction == LEFT) && (ground_or_fall == GROUNDING);
    assign walk_right = (direction == RIGHT) && (ground_or_fall == GROUNDING);
    assign aaah = (ground_or_fall == FALLING);

endmodule
