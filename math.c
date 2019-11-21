internal int
clamp(int min, int val, int max){
	if(val < min) return min;
	if(val > max) return max;
	return(val);
}

struct {
	union{
		struct {
			d64 x;
			d64 y;
		};
		d64	e[2];
	};
} typedef v2;
