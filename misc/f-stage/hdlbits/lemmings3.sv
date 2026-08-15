module top_module (
    input  clk,
    input  areset,
    input  bump_left,
    input  bump_right,
    input  dig,
    input  ground,
    output walk_left,
    output walk_right,
    output aaah,
    output digging
);

    enum {
        DIGGING,
        FALLING,
        WALKING
    }
        state_reg, state_nxt;
    always @(posedge clk or posedge areset) begin
        if (areset) begin
            state_reg <= WALKING;
        end else begin
            state_reg <= state_nxt;
        end
    end
    always_comb begin
        state_nxt = state_reg;
        case (state_reg)
            DIGGING: begin
                if (!ground) state_nxt = FALLING;
            end
            FALLING: begin
                if (ground) state_nxt = WALKING;
            end
            WALKING: begin
                if (!ground) state_nxt = FALLING;
                else if (dig) state_nxt = DIGGING;
            end
        endcase
    end

    logic is_and_will_walking;
    assign is_and_will_walking = (state_reg == WALKING) && (state_nxt == WALKING);
    enum {
        LEFT,
        RIGHT
    } direction_reg;
    always @(posedge clk, posedge areset) begin
        if (areset) begin
            direction_reg <= LEFT;
        end else if (is_and_will_walking && bump_left && bump_right) begin
            direction_reg <= direction_reg == LEFT ? RIGHT : LEFT;  // change the direction
        end else if (is_and_will_walking && bump_left) begin
            direction_reg <= RIGHT;
        end else if (is_and_will_walking && bump_right) begin
            direction_reg <= LEFT;
        end
    end

    assign walk_left = (state_reg == WALKING) && (direction_reg == LEFT);
    assign walk_right = (state_reg == WALKING) && (direction_reg == RIGHT);
    assign aaah = (state_reg == FALLING);
    assign digging = (state_reg == DIGGING);

endmodule
