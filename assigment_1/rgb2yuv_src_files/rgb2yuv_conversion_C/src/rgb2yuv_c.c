#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getopt

//#define PNGSUITE_PRIMARY

void rgb2yuv (char *srcfile, char *outfile);
void print_autor();
void print_help();
unsigned char *open_image(char *src_file, int x, int y, int z);
int main(int argc, char **argv)
{

	int c;
	char *src_image = NULL;
	char *out_image = NULL;

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
	-i argument is required for the program to do the rgb conversion from this src image. 
	-o argument is required for the program to store the result from the conversion.
	If one of those arguments is not added, the program will fail due to there is not a required argument to have the result succesfully. 
	The following "if" will check if those argument were added in the command line, and if one of the 
	argument was not added, the program will fail.	
	*/

	if(src_image==NULL){
		fprintf(stderr, "-Error- Argument -i <source_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Conversion program will use the image %s as the source image\n", src_image);
	}

	if(out_image==NULL){
		fprintf(stderr, "-Error- Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Conversion program will store the image resultant in the file %s\n", out_image);
	}


	rgb2yuv(src_image, out_image);

	return 0;

}

unsigned char *open_image(char *src_file, int x, int y, int z1){

	FILE *f = fopen(src_file, "rb");

	unsigned char*	result;
	unsigned char	temp;

	int h,w,depth;
	h=y;
	w=x;
	depth=z1;

	int size = w*h*depth;
 	int row,col,z;

	result = (unsigned char*)malloc(size);	
	if (f){
                while(!feof(f)){
                        fread(result, size, 1, f);
			for (row = 0; row < h; row++) {
				for (col = 0; col < w; col++) {
					for (z = 0; z < depth; z++) {
						temp = result[(row * w + col) * depth + z];
						result[(row * w + col) * depth + z] = result[((h - row - 1) * w + col) * depth + z];
						result[((h - row - 1) * w + col) * depth + z] = temp;
					}
				}
			}

                }

        }

	
	return result;

	
}

void rgb2yuv (char *srcfile, char *outfile){

	unsigned char *org_image, *result;
	int w=640;
	int h=480;
	int depth=3;
	int size = w*h*depth;

	org_image = open_image(srcfile,w,h,depth);
	result = open_image(srcfile,w,h,depth);

	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);		
	unsigned char c_image[h][w][depth];

	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			for (int z = 0; z < depth; z++) {
				c_image[row][col][z] = org_image[(row * w + col) * depth + z];
				result[(row * w + col) * depth + z] = -1;
			}
		}
	}

	printf("-I- Starting conversion of RGB to YUV\n");		


	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
		// Y =  0.299R + 0.587G + 0.114B
		//  U = -0.147R - 0.289G + 0.436B
		// V =  0.615R - 0.515G - 0.100B

			for (int z = 0; z < depth; z++) {
				if(z==0){
					result[(row * w + col) * depth + z] = (0.299*c_image[row][col][2] + 0.587*c_image[row][col][1] + 0.114*c_image[row][col][0]);
				}

				if(z==1){
					result[(row * w + col) * depth + z] = (-0.147*c_image[row][col][2] - 0.289*c_image[row][col][1] + 0.436*c_image[row][col][0]);
				}

				if(z==2){
					result[(row * w + col) * depth + z] = (0.615*c_image[row][col][2] - 0.515*c_image[row][col][1] - 0.100*c_image[row][col][0]);
				}
			}
		}	
	}


	printf("-I- Finished conversion of RGB to YUV\n");	

	FILE *fw = fopen(outfile, "wb");
	fwrite(result, size, sizeof(size), fw);
	exit(0);
}


//This method will print the autors information
void print_autor(){
	printf("#############################################\n");
	printf("#           Conversion tool                #\n");
	printf("#############################################\n");
	printf("# Autores: Marco Espinoza Murillo           #\n");
	printf("#          Jose Campos Murillo              #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


//This method will print the help menu

void print_help(){
	printf("####################################################################################\n");
	printf("#                               Conversion tool                                   #\n");
	printf("####################################################################################\n");
	printf("# Valid arguments:                                                                 #\n");
	printf("#   -a: It shows the program autor information                                     #\n");
	printf("#   -h: It shows this help menu                                                    #\n");
	printf("#   -i: It receive the source image to apply the yuv conversion                    #\n");
	printf("#   -o: It receive the name for the output image with the yuv image result         #\n");
	printf("#                                                                                  #\n");
	printf("# Command line examples                                                            #\n");
	printf("#  ./conv -i foto1.rgb   -o result.rgb                                             #\n");
	printf("####################################################################################\n");
	exit(0);
	
}

