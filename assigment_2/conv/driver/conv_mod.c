
void kernel_left_sobel (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){

	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*1 + c_image[row-1][col][z]*0 + c_image[row-1][col+1][z]*-1 + 
						c_image[row]  [col-1][z]*2 + c_image[row]  [col][z]*0  + c_image[row]  [col+1][z]*-2 + 
						c_image[row+1][col-1][z]*1 + c_image[row+1][col][z]*0 + c_image[row+1][col+1][z]*-1);
}

void kernel_identity (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){

	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0 + c_image[row-1][col][z]*0 + c_image[row-1][col+1][z]*0 + 
						c_image[row]  [col-1][z]*0 + c_image[row]  [col][z]*1  + c_image[row]  [col+1][z]*0 + 
						c_image[row+1][col-1][z]*0 + c_image[row+1][col][z]*0 + c_image[row+1][col+1][z]*0);
}

void kernel_outline (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*-1 + c_image[row-1][col][z]*-1 + c_image[row-1][col+1][z]*-1 + 
						c_image[row]  [col-1][z]*-1 + c_image[row]  [col][z]*8  + c_image[row]  [col+1][z]*-1 + 
						c_image[row+1][col-1][z]*-1 + c_image[row+1][col][z]*-1 + c_image[row+1][col+1][z]*-1);
}

void kernel_blur (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0.0625 + c_image[row-1][col][z]*0.125 + c_image[row-1][col+1][z]*0.0625 + 
						c_image[row]  [col-1][z]*0.125 + c_image[row]  [col][z]*0.25  + c_image[row]  [col+1][z]*0.125 + 
						c_image[row+1][col-1][z]*0.0625 + c_image[row+1][col][z]*0.125 + c_image[row+1][col+1][z]*0.0625);
}

void kernel_sharpen (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0 + c_image[row-1][col][z]*-1 + c_image[row-1][col+1][z]*0 + 
						c_image[row]  [col-1][z]*-1 + c_image[row]  [col][z]*5  + c_image[row]  [col+1][z]*-1 + 
						c_image[row+1][col-1][z]*0 + c_image[row+1][col][z]*-1 + c_image[row+1][col+1][z]*0);
}

void kernel_topsobel (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*1 + c_image[row-1][col][z]*2 + c_image[row-1][col+1][z]*1 + 
						c_image[row]  [col-1][z]*0 + c_image[row]  [col][z]*0  + c_image[row]  [col+1][z]*0 + 
						c_image[row+1][col-1][z]*-1 + c_image[row+1][col][z]*-2 + c_image[row+1][col+1][z]*-1);
}

