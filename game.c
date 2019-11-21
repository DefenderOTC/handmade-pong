

v2 player_p;

internal void
simulate_game(Input * input, d64 delta_time){
	
	d64 speed = 100;					
	clear_screen(0Xff44a000);
	if(is_down(BUTTON_LEFT)) player_p.x -= speed *    delta_time;
	if(is_down(BUTTON_RIGHT)) player_p.x += speed *   delta_time;
	if(is_down(BUTTON_UP)) player_p.y += speed *      delta_time;
	if(is_down(BUTTON_DOWN)) player_p.y -= speed*     delta_time;
	
	draw_rect(player_p, (v2){1,1},0x00ff00);
}