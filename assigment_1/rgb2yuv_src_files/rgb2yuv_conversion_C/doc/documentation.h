/**
 * @file documentation.h
 * @authors Marco Espinoza/
 *			Jose Campos/
 * @brief Assignment 1: Meta-Tec with RGB2YUV with C compilation! 
 * @section USAGE
 * This program receive an rgb image as argument and it will convert the image to yuv using C methods. 
 * 
 * Usage: 
 *
 *		rgb2yuv -i <source_image> -o <output_image> 
 *
 *		-i source_image  	specify the RGB input image to convert to yuv
 *
 *		-o dest_image  		specify the resulting image	
 *		-h 					print help 
 *		-a 					print author information
 *
 * @subsection Methods Description
 *  
 *		1. rgb2yuv (char *srcfile, char *outfile);
*		2. void print_autor();
*		3. void print_help();
*		4. unsigned char *open_image(char *src_file, int x, int y, int z);

 *
 * @subsection Example Example 
 * Example input image:
 * @image html image.rgb
 *
 * Example output image:
 * @image html image.yuv
 */

/**
 * @brief rgb2yuv is used to do the image convertions from rgb to yuv
 * @param srcfile Source image
 * @param outfile Output image
 */
void rgb2yuv (char *srcfile, char *outfile);

/**
 * @brief print_autor() is used to print the autor information 
 */
void print_autor();
/**
 * @brief print_help
 *        It will print the help related with the program 
 */

void print_help();


