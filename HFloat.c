h_float16 float2half(float x){
	long *x_bytes; //uint32_t
	
	unsigned char e; //uint8_t 
	long m; //uint32_t
	
	h_float16 y;
	
	y=0;
	x_bytes=&x;  // (cast float into bit array "int32")
	e=*x_bytes>>23;//b30-23
	m=*x_bytes&0x7FFFFFL;//23 bit
	
	if (*x_bytes&(1L<<31)) y=0x8000 ;//sig bit
	
	
	if (e>0x8E){
		y+=0x7C00;//inf (1F<<10)
		if (e==255 && m) y++; //NaN
	}else{
		if(e>0x70) y+=(h_float16)(e-0x70)<<10;//exponent
		y+=(m)>>13;//mantis
		if (m & (1<<12)) y++;//round 
	}
	return y;
}

float half2float(h_float16 x){
	long x_bytes; //uint32_t
	float *f_pointer;
	
	char e; //uint8_t 
	short m; //uint16_t
	
	x_bytes=0;
	e=(x>>10)&0x1F;//b14-10
	m=x&0x03FF;//10 bit Mask (b9-0)
	
	if (x&1<<15) x_bytes=1L<<31;//sig bit
	
	if (e==0||e==0x1F){
		x_bytes+=(long)(e)<<23;// zero/inf
		if(m) x_bytes++; //NaN
		}else{
		x_bytes+=(long)(0x70+e)<<23;//exponent
		x_bytes+=(long)(m)<<13;//mantis
	}
	f_pointer=&x_bytes;
	return *f_pointer;
}