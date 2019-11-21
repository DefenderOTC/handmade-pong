
internal void
clear_screen(u32 color){
	u32* pixel	= render_buffer.pixels;
	
	for(int y = 0; y < render_buffer.height; y++){
		for(int x = 0; x < render_buffer.width; x++){
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color){
	x0 = clamp(0, x0, render_buffer.width);
	x1 = clamp(0, x1, render_buffer.width);
	y0 = clamp(0, y0, render_buffer.height);
	y1 = clamp(0, y1, render_buffer.height);
	for( int y = y0; y < y1; y++){
		u32* 	pixel = render_buffer.pixels + x0 + render_buffer.width * y;
		for(int x = x0; x < x1; x++){
			*pixel++ = color;
		}
	}
}

internal void
draw_rect(v2 p, v2 half_size, u32 color){
	d64 aspect_multiplier 	= (d64) render_buffer.height;
	if((d64) render_buffer.width / render_buffer.height < 1.77){
		aspect_multiplier =	(d64)render_buffer.width * 1.77;
	}
	d64 scale	= .01;
	half_size.x	*= aspect_multiplier * scale;
	half_size.y	*= aspect_multiplier * scale;

	p.x	*= aspect_multiplier * scale;
	p.y	*= aspect_multiplier * scale;

	p.x	+= (d64)render_buffer.width * .5;
	p.y	+= aspect_multiplier * .5;

	int x0	= (int)(p.x - half_size.x);
	int y0	= (int)(p.y - half_size.y);
	int x1	= (int)(p.x + half_size.x);
	int y1	= (int)(p.y + half_size.y);

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}