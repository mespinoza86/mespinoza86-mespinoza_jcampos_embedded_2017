#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getopt
#include <sys/time.h>

//#include <iostream>

//#define PNGSUITE_PRIMARY

void rgb2yuv (char *srcfile, char *outfile);
void print_autor();
void print_help();
unsigned char *open_image(char *src_file, char *file_array, int x, int y, int z);
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

	struct timeval start, end;
	gettimeofday(&start, NULL);
	rgb2yuv(src_image, out_image);
	gettimeofday(&end, NULL);
	printf("Time executed in microseconds is %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	return 0;

}

unsigned char *open_image(char *src_file, char *file_array, int x, int y, int z1){

	FILE *f;
	if (!(f = fopen(src_file, "rb"))){
		printf("-E- The file %s does not exist in the path\n", src_file);
		perror("-E- Issues opening the file provided\n");
		exit(-1);
	}
	
	unsigned char*	result;
	unsigned char	temp;

	int h,w,depth;
	h=y;
	w=x;
	depth=z1;

	int size = w*h*depth;
 	int row,col,z;

	result = (unsigned char*)malloc(size);	
	int status = fread(file_array, 1, size, f);	
	
	return result;

	
}

void rgb2yuv (char *srcfile, char *outfile){
	FILE *fw = fopen(outfile, "wb");
	unsigned char *org_image, *result;
	int w=640;
	int h=480;
	int depth=3;
	int size = w*h*depth;
	short int pixel_data = 0;
	unsigned char *y;
	unsigned char *u;
	unsigned char *v;
	unsigned char *yuv;
	unsigned char c_image[h][w][depth];

	y = (char*) malloc (sizeof(char)*8);
	memset(y, 0, 8);
	u = (char*) malloc (sizeof(char)*8);
	memset(u, 0, 8);

	v = (char*) malloc (sizeof(char)*8);
	memset(v, 0, 8);

	yuv = (char*) malloc (sizeof(char)*16);
	memset(yuv, 0, 16);

	org_image = (unsigned char*)malloc(sizeof(char)*size);
	open_image(srcfile,org_image,w,h,depth);

	result = (unsigned char*)malloc(sizeof(char)*size);
	open_image(srcfile,result,w,h,depth);

	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);		

	printf("-I- Starting conversion of RGB to YUV\n");		

	for (int i=0; i < size/3; i++){
	//Formula to calculate the YUV from RGB:
	// Y = ((66xR  + 129xG + 25xB  + 128) >> 8) + 16
	// U = ((-38xR - 74xG  + 112xB + 128) >> 8) + 128
	// V = ((112xR - 94xG  - 18xB  + 128) >> 8) + 128
		int r = *org_image++;
		int g = *org_image++;
		int b = *org_image++;									
	 	y[0] = ((66*r + 129*g + 25*b  + 128) >> 8) +  16;
		u[0] = ((-38*r - 74*g + 112*b + 128) >> 8) + 128;
		v[0] = ((112*r - 94*g - 18*b  + 128) >> 8) + 128;

	//Storing image
		fwrite(y, 1,1 , fw);
		if(i%2 == 0)
			fwrite(u, 1,1 , fw);
		else
			fwrite(v, 1,1 , fw);
}
	printf("-I- Finished conversion of RGB to YUV\n");	
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

