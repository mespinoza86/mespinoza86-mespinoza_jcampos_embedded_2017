################################
## Meta-Tec layer projec      ##
## Autors:                    ##
##        Marco Espinoza      ##
##        Jose Campos         ##
################################

This project has folder called meta-tec layer
Inside the meta-tec layer there are 3 importante recipes:
 - recipe-rgb2yuv_c
 - recipe-intrinsicrgb2yuv
 - recipe-neonrgb2yuv

The command line to compile this layer is the follwing:

bitbake meta-tec-image

The explanation for each of the recipe developed is:

*recipe-rgb2yuv:
                It has a program developed in C, and the main function is convert a 
                rgb image to yuv.
                All the methods used are from C and it is not using neon to do the 
                operations. 

                It was measured the time expended by this program to do the image conversion
                and it was: 338000 us for a image.rgb.

* recipe-intrinsicrgb2yuv:
                It has a program developed in C but using the neon intrinsic library to do  
                the image conversion from rgb to yuv.
                All the methods used were written in C language but using the intrinsic 
                vectors and operations from the neon lib.

                It was measured the time expended by this program to do the image conversion
                and it was: 156902 us for a image.rgb.

* recipe-neonrgb2yuv:
                It has a program developed in C but using the neon library to do  
                the image conversion from rgb to yuv.
                The main method to do the conversion was written using ARM neon asm
                It allowed to have an improvement in the image conversion, due to 
                it was used the registers directly in asm code. 

                It was measured the time expended by this program to do the image conversion
                and it was: 138085 us for a image.rgb.

To execute the binary files generated belonging to each recipe, the command line required is the following:

rgb2yuv C version:

The binary file is rgb2yuv_c:
Command line: rgb2yuv_c -i <rgg image> -o <output image>
The program will generate a yuv image with the name specified in the -o argument.

intrinsic rgb2yuv version:

The binary file is intrinsicrgb2yuv:
Command line: intrinsicrgb2yuv_c -i <rgg image> -o <output image>
The program will generate a yuv image with the name specified in the -o argument.

neon rgb2yuv version:

The binary file is neonrgb2yuv:
Command line: neonrgb2yuv_c -i <rgg image> -o <output image>
The program will generate a yuv image with the name specified in the -o argument.

Conclusions:
 
* It was possible to developed 3 programs to do the image conversion from rgb to yuv, 
  allowing to identify the differences between the C programming and the improvement
  aported by neon in the program execution.
* In embedded systems it is very important to develop programs with good performance due
  to the memory and battery are limited in this kind of systems, for this reason the 
  implementation of libraries which help to use appropriately the hardware architecture
  is very important.
* In this project it was demonstrated despite C programming is a low programming level, the
  performance still could be improved if the program is written with asm code and proper
  vector instruction.
* The timing measurement demonstrated the neon_rgb2yuv program obtained the better performance,
  however the intrinsic had a similar result, without program in asm code, and the rgb2yuv 
  program written in C language obtained the worst performance. 

