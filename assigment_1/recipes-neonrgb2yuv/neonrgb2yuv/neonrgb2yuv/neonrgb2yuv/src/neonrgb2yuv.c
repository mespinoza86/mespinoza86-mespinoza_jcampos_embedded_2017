

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getopt
#include <math.h>
#include <sys/time.h>
#include <arm_neon.h>
#include <errno.h>

void rgb2yuv (char *srcfile, char *outfile);
void print_autor();
void print_help();
uint8_t *open_image(char *src_file, uint8_t *file_array, int x, int y, int z);

int main(int argc, char **argv)
{

	int c;
	char *src_image = NULL;
	char *out_image = NULL;

//This while is used to read the arguments added to through the command line to the application
	while ((c = getopt (argc, argv, "i:o:ha")) != -1){
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

	int time_diff, time1, time2;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	rgb2yuv(src_image, out_image);
	gettimeofday(&end, NULL);
	printf("Execution time in micro seconds is: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));


	return 0;

}
uint8_t *open_image(char *src_file, uint8_t *file_array, int w, int h, int depth){

	FILE *f = NULL;
	
	f= fopen(src_file, "rb");
	uint8_t*	result;
	int size = h*w*depth;
	

	result = (uint8_t*) malloc (sizeof(uint8_t)*size);
        memset( file_array, 0, size);	
	fread(file_array, 1, size, f);
	return result;

	
}

void rgb2yuv (char *srcfile, char *outfile){

//Generating the required variables
	uint8_t *org_image;
	FILE* fw = NULL;
	FILE* f  = NULL;
	//Required to calculate Y
//	uint8x8_t rtoy = vdup_n_u8 (66);

	asm("mov r1, #66\n"
	    "vdup.8 d1, r1");

//	uint8x8_t gtoy = vdup_n_u8 (129);

	asm("mov r1, #129\n"
	    "vdup.8 d2, r1");

//	uint8x8_t btoy = vdup_n_u8 (25);

	asm("mov r1, #25\n"
	    "vdup.8 d3, r1");

	//Required to calculate V

//	uint8x8_t rtou = vdup_n_u8 (38);

	asm("mov r1, #38\n"
	    "vdup.8 d4, r1");

//	uint8x8_t gtou = vdup_n_u8 (74);

	asm("mov r1, #74\n"
	    "vdup.8 d5, r1");

//	uint8x8_t btou = vdup_n_u8 (112);
	asm("mov r1, #112\n"
	    "vdup.8 d6, r1");

	//Required to calculate U

//	uint8x8_t rtov = vdup_n_u8 (112);
	asm("mov r1, #112\n"
	    "vdup.8 d7, r1");

//	uint8x8_t gtov = vdup_n_u8 (94);
	asm("mov r1, #94\n"
	    "vdup.8 d8, r1");

//	uint8x8_t btov = vdup_n_u8 (18);
	asm("mov r1, #18\n"
	    "vdup.8 d9, r1");

	//Required to do the operations to get the final YUV result

//	uint16x8_t set128pq = vdupq_n_u16(128);
	asm("mov r1, #128\n"
	    "vdup.16 q1, r1");

//	uint8x8_t set16   = vdup_n_u8(16);
	asm("mov r1, #16\n"
	    "vdup.8 d10, r1");

//	uint8x8_t set128   = vdup_n_u8(128);
	asm("mov r1, #128\n"
	    "vdup.8 d11, r1");

  //      uint8x8_t temp; //d15
  //	uint16x8_t  var1; //q2

	//Generating the Buffers for Y, U, V and YUV
	uint8_t* y;
	uint8_t* u;
	uint8_t* v;
	uint8_t* yuv;

/*
	//jecampos
	int x2 = 5;
	int y2;
	
	asm("mov %[result],%[value]" : [result] "=r" (y2) : [value] "r" (x2));
	printf("-I- y2 value is :     %i\n", y2);

	asm("mov r3, #77");
	asm("mov r4, #151");
	asm("mov r5, #28");
	asm("mov r10, r3");
	asm("mov r11, r4");
	asm("mov r12, r5");
	asm("vld3.8 {d28-d30}, [r1]!");*/

//Variables initialization

	int w=640;
	int h=480;
	int depth = 3;
	int size = w*h*depth;
	int i=0;

	asm ("mov r2, #640\n" //int w=640;
	     "mov r3, #480\n" //int h=480;
	     "mov r4, #3\n"   // int depth=3;
	     "mov r5, #0\n"   // int i = 0;
	     "mul r2, r2,r3\n"); // int size = w*h;
	org_image = (uint8_t*) malloc (sizeof(uint8_t)*size);
	open_image(srcfile, org_image, w,h,depth);
	fw = fopen( outfile,"wb" );
	


  	y = (uint8_t*) malloc (sizeof(uint8_t)* 8);
	memset(y, 0, 8 );

	u = (uint8_t*) malloc (sizeof(uint8_t)* 8);
	memset(u, 0, 8 );

	v = (uint8_t*) malloc (sizeof(uint8_t)* 8);
	memset(v, 0, 8 );

	yuv = (uint8_t*) malloc (sizeof(uint8_t)* 24);
	memset( yuv, 0, 24 );
	size = size/depth;
        int size2;
	size/=8;
        asm("mov r2, %[size]" :"=r"(size2) :[size]  "r"(size) : "r2");
        asm("lsr r2, r2, #3\n"); // size/8

//Formula to calculate the YUV from RGB:
// Y = ((66xR  + 129xG + 25xB  + 128) >> 8) + 16
// U = ((-38xR - 74xG  + 112xB + 128) >> 8) + 128
// V = ((112xR - 94xG  - 18xB  + 128) >> 8) + 128

	printf("------- Image Information -------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);		
	printf("---------------------------------\n");

	printf("-I- The formula used to calculate the YUV from RGB is the following:\n");
	printf("-I-  Y = ((66xR  + 129xG + 25xB  + 128) >> 8) + 16  \n");
	printf("-I-  U = ((-38xR - 74xG  + 112xB + 128) >> 8) + 128 \n");
	printf("-I-  V = ((112xR - 94xG  - 18xB  + 128) >> 8) + 128 \n");


  for (i=0; i<(size); i++){

   asm(".loop:\n");

//    uint8x8x3_t rgb  = vld3_u8 (org_image);
    asm("mov r6, %[img]" ::[img]  "r"(org_image) : "r6");
    asm("mov r7, %[bufy]"::[bufy] "r"(y)         : "r7");
    asm("mov r8, %[bufg]"::[bufg] "r" (u)        : "r8");
    asm("mov r9, %[bufv]"::[bufv] "r" (v)        : "r9");
    asm("vld3.8      {d12-d14}, [r6]!\n"); //r0 should have org_imag
 
///Calculating Y

//    var1 = vmull_u8 (rgb.val[0],      rtoy);
    asm("VMULL.u8 q2, d12,d1\n");

//    var1 = vmlal_u8 (var1,rgb.val[1], gtoy);
    asm("vmlal.u8 q2,d13,d2\n");

//    var1 = vmlal_u8 (var1,rgb.val[2], btoy);
    asm("vmlal.u8 q2,d14,d3\n");

//    var1 = vaddq_u16(var1, set128pq);
    asm("VADD.I16 q2,q2,q1\n");

 //   temp = vshrn_n_u16 (var1, 8);
    asm("vshrn.u16   d15, q2, #8\n");

 //   temp = vadd_u8(temp, set16);
    asm("VADD.I8      d15, d10\n");
//    vst1_u8 (y, temp);
    asm("vst1.8 {d15}, [r7]!\n");  //r1 should have y buffer


    ///Calculating U

//    var1 = vmull_u8 (rgb.val[2], btou);
    asm("VMULL.u8 q2, d14, d6\n");

//    var1 = vmlsl_u8 (var1, rgb.val[0], rtou);
    asm("VMLSL.U8 q2, d12, d4\n");

//    var1 = vmlsl_u8 (var1, rgb.val[1], gtou);
    asm("VMLSL.U8 q2, d13, d5\n");

//    var1 = vaddq_u16(var1, set128pq);
    asm("VADD.I16 q2,q2,q1\n");

//    temp = vshrn_n_u16 (var1, 8);
    asm("vshrn.u16   d15, q2, #8\n");

//    temp = vadd_u8(temp, set128);
    asm("VADD.I8      d15, d11\n");

//    vst1_u8 (u, temp);
    asm("vst1.8 {d15}, [r8]!\n");  //r2 should have u buffer

    //Calculating V
//    var1 = vmull_u8 (rgb.val[0],      rtov);
    asm("VMULL.u8 q2, d12, d7\n");

//    var1 = vmlsl_u8 (var1,rgb.val[1], gtov);
    asm("VMLSL.U8 q2, d13, d8\n");

//    var1 = vmlsl_u8 (var1,rgb.val[2], btov);
    asm("VMLSL.U8 q2, d14, d9\n");

//    var1 = vaddq_u16(var1, set128pq);
    asm("VADD.I16 q2,q2,q1\n");

//    temp = vshrn_n_u16 (var1, 8);
    asm("vshrn.u16   d15, q2, #8\n");

//    temp = vadd_u8(temp, set128);
    asm("VADD.I8      d15, d11\n");

//    vst1_u8 (v, temp);
    asm("vst1.8 {d15}, [r9]!\n");  //r3 should have u buffer

  
    //Final result
   
    yuv[0] = y[0]; 
    yuv[1] = u[0]; 
    yuv[2] = v[0];
    yuv[3] = y[1]; 
    yuv[4] = u[1];
    yuv[5] = v[1];
    yuv[6] = y[2]; 
    yuv[7] = u[2]; 
    yuv[8] = v[2];
    yuv[9] = y[3]; 
    yuv[10] = u[3]; 
    yuv[11] = v[3];
    yuv[12] = y[4]; 
    yuv[13] = u[4]; 
    yuv[14] = v[4];
    yuv[15] = y[5]; 
/*    yuv[16] = u[5]; 
    yuv[17] = v[5];
    yuv[18] = y[6]; 
    yuv[19] = u[6]; 
    yuv[20] = v[6];
    yuv[21] = y[7]; 
    yuv[22] = u[7]; 
    yuv[23] = v[7];*/
 
    //Storing the image
    fwrite(yuv, 1, 16, fw);


//    org_image  += 8*3;
    asm("vst1.8      {d6}, [r6]!\n"
        "subs        r2, r2, #1\n");
 //       "bne         .loop\n");

  
  }
	printf("-I- Finished conversion of RGB to YUV\n");	


}


//This method will print the autors information
void print_autor(){
	printf("#############################################\n");
	printf("#           Conver tool                #\n");
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
	printf("#                               Conver tool                                   #\n");
	printf("####################################################################################\n");
	printf("# Valid arguments:                                                                 #\n");
	printf("#   -a: It shows the program autor information                                     #\n");
	printf("#   -h: It shows this help menu                                                    #\n");
	printf("#   -i: It receive the source image to apply the yuv conversion                    #\n");
	printf("#   -o: It receive the name for the output image with the yuv image result         #\n");
	printf("#                                                                                  #\n");
	printf("# Command line examples                                                            #\n");
	printf("#  ./conv -i foto1.rgb  -o result.rgb                                              #\n");
	printf("####################################################################################\n");
	exit(0);
	
}

