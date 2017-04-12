
#define PNGSUITE_PRIMARY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DEFINE
#include "stb.h"

#include <string.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include "../driver/chardev.h"

void write_image (char *srcfile, int file_desc);
void print_autor();
void print_help();
int open_device();
void ioctl_set_mode(int file_desc, int value);
void read_image(int file_desc, char *outfile);
int w,h, depth;

int main(int argc, char **argv)
{

	int c;
	char *src_image = NULL;
	char *out_image = NULL;
	int kernel = -1;
	int file;

//This while is used to read the arguments added to through the command line to the application
	while ((c = getopt (argc, argv, "i:o:k:ha")) != -1){
/*
The following switch will identify the arguments read, and it will call the proper method
and set the proper variables in order to execute the required methods
*/

		switch (c) {
			case 'a':
			//'a' means the autor information will be printed
				print_autor();
				break;
			case 'i':
			//'b' It will have the path for the binary file to be executed
				src_image = optarg;
				break;			
			case 'h':
			//'h' is used to print the help menu
				print_help();
				break;
			case 'o':
			//'w' has the address which will be monitored during the test execution
				out_image = optarg;
				break;
			case 'k':
			//'s' will execute the binary file step by step
				kernel = atoi(optarg);
				break;

			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				        return 1;
			default:
			//In case the argument read is not supported
				printf("Option '-%i' not recognized.\n",c);
			}
	}

	/*
	-i argument is required for the program to apply the kernel to the src image. 
	-o argument is required for the program to store the result from the convolution between the src image and the kernel selected.
	-k argument is required for the program to know the kernel to apply. 
	If one of those arguments is not added, the program will fail due to there is not a required argument to have the result succesfully. 
	The following "if" will check if those argument were added in the command line, and if one of the 
	argument was not added, the program will fail.	
	*/

	if(src_image==NULL){
		fprintf(stderr, "-Error- Argument -i <source_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will use the image %s as the source image\n", src_image);
	}

	if(out_image==NULL){
		fprintf(stderr, "-Error- Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will store the image resultant in the file %s\n", out_image);
	}

	if(kernel==-1){
		fprintf(stderr, "-Error- Argument -k <kernel desired> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will apply the kernel number %i to the src image\n", kernel);
	}

	file =open_device();
	write_image(src_image, file);
	file =open_device();
	ioctl_set_mode(file, kernel);
	ioctl_start_mode(file);
	read_image(file, out_image);

//	run_kernel(src_image, out_image, kernel);

	return 0;

}

/* open_device(): This method is used to open the device 
                  In case of error the method open will return a value different to 0
*/

int open_device(){
	int ret_val;
	static int file_desc;
	file_desc = open(DEVICE_FILE_NAME,2);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}
	return file_desc;

}

/* Method used to send through IOCTL the kernel choosen from command line */

void ioctl_set_mode(int file_desc, int value)
{
	int ret_val;
	ret_val = ioctl(file_desc,IOCTL_SET_MODE, value);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void ioctl_start_mode(int file_desc){
	int ret_val;
	ret_val = ioctl(file_desc,IOCTL_START_CONV, 0);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void read_image (int file_desc, char *outfile){
	uint8 *result;
	int image_size = w*h*depth;
	result = (uint8 *) malloc(sizeof(uint8)*image_size);
	memset( result, 0, image_size);
	read(file_desc, result, image_size);
	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);

	stbi_write_bmp(outfile,w,h,depth,result);
	close(file_desc);

}

void write_image (char *srcfile, int file_desc){

	uint8 *org_image;
	int *x; int *y; int req_comp;
//	char tu[5]="A1223";
	org_image = stbi_load(srcfile, &w, &h, &depth, req_comp);
	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);
	int image_size = w*h*depth;
	
	write(file_desc, org_image, image_size);
	close(file_desc);
//	write(file_desc, tu, sizeof(char));

/*	result = stbi_load(srcfile, &w, &h, &depth, req_comp);

	uint8 temp_c_image[h][w][3];
	
	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);		

	uint8 c_image[h][w][depth];


	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			for (int z = 0; z < depth; z++) {
				c_image[row][col][z] = org_image[(row * w + col) * depth + z];
				result[(row * w + col) * depth + z] = -1;
			}
		}
	}
*/
/*
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			for (int z = 0; z < depth; z++) {

				switch (_kernel) {
					case 1:
						kernel_left_sobel(result, h, row, w, col, depth, z, c_image);	
					break;
					
					case 2:
						kernel_identity(result, h, row, w, col, depth, z, c_image);							
					break;			
				
					case 3:
						kernel_outline(result, h, row, w, col, depth, z, c_image);		
					break;
	
					case 4:
						kernel_blur(result, h, row, w, col, depth, z, c_image);		
					break;

					case 5:
						kernel_sharpen(result, h, row, w, col, depth, z, c_image);		
					break;

					case 6:
						kernel_topsobel(result, h, row, w, col, depth, z, c_image);		
					break;	
					default:
						printf("-Error- Kernel %i is not found, exiting from the program ....\n", _kernel);
						exit(0);
				}
			}
		}
	}
*/	
/*	if (strstr(srcfile,".bmp")){
		printf("-Info- Storing the image as bmp\n");
		stbi_write_bmp(outfile,w,h,depth,result);
		printf("-Info- Image was stored as %s\n", outfile); 
	}else if(strstr(srcfile,".png") != NULL){
		printf("-Info- Storing the image as png\n");
		stbi_write_png(outfile,w,h,depth,result, w*3);
		printf("-Info- Image was stored as %s\n", outfile);
	}else if(strstr(srcfile,".tga") != NULL){
		printf("-Info- Storing the image as tga\n");
     		stbi_write_tga(outfile,w,h,depth,result);
		printf("-Info- Image was stored as %s\n", outfile);
	}else if(strstr(srcfile,".jpg") != NULL){
		printf("-Warning- Input file has jpg format but it will be stored as bmp\n");
		stbi_write_bmp(outfile,w,h,depth,result);
		printf("-Info- Image was stored as %s\n", outfile); 

	}else{
		printf("-Error-The image format added is not supported by the program\n-Error- Format compatible are: jpg/png/bmp/tga\n-Error- Exiting ....\n");
		exit(0);
	}
*/
	
}

//This method will print the autors information
void print_autor(){
	printf("#############################################\n");
	printf("#           Convolution tool                #\n");
	printf("#############################################\n");
	printf("# Autores: Marco Espinoza Murillo           #\n");
	printf("#          Jose Campos Murillo              #\n");
	printf("#	   Freddy Zeledon Jarquin           #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


//This method will print the help menu

void print_help(){
	printf("####################################################################################\n");
	printf("#                               Convolution tool                                   #\n");
	printf("####################################################################################\n");
	printf("# Valid arguments:                                                                 #\n");
	printf("#   -a: It shows the program autor information                                     #\n");
	printf("#   -h: It shows this help menu                                                    #\n");
	printf("#   -i: It receive the source image to apply the kernel                            #\n");
	printf("#   -k: It is the kernel to apply to the image, one of the follows can be applied: #\n");
	printf("#            1: kernel_left_sobel                                                  #\n");
	printf("#            2: kernel_identity                                                    #\n");
	printf("#            3: kernel_outline                                                     #\n");
	printf("#            4: kernel_blur                                                        #\n");
	printf("#            5: kernel_sharpen                                                     #\n");
	printf("#            6: kernel_topsobel                                                    #\n");
	printf("#   -o: It receive the name for the output image with the kernel applied           #\n");
	printf("#                                                                                  #\n");
	printf("# Command line examples                                                            #\n");
	printf("#  ./conv -i foto1.jpg   -o result.bmp -k 5                                        #\n");
	printf("#  ./conv -i foto2.jpg   -o result.bmp -k 2                                        #\n");
	printf("#  ./conv -i foto3.bmp   -o result.bmp -k 6                                        #\n");
	printf("#  ./conv -i lena512.bmp -o result.bmp -k 3                                        #\n");
	printf("####################################################################################\n");
	exit(0);
	
}

