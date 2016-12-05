/*-----------------------------------------

//Update History:
//2016/06/13 	V1.1	by Lee	add support for burst mode

--------------------------------------*/


#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "arducam_arch_raspberrypi.h"
#define OV2640_CHIPID_HIGH 	0x0A
#define OV2640_CHIPID_LOW 	0x0B
#define BUF_SIZE (384*1024)
#define BUF_LEN 4096
#define CAM1_CS 0
#define CAM2_CS 4
#define CAM3_CS 3
#define CAM4_CS 5
uint8_t buffer[BUF_SIZE] = {0xFF};

ArduCAM myCAM1(OV2640,CAM1_CS);
ArduCAM myCAM2(OV2640,CAM2_CS);
ArduCAM myCAM3(OV2640,CAM3_CS);
ArduCAM myCAM4(OV2640,CAM4_CS);

void setup()
{
    uint8_t vid,pid;
    uint8_t temp;
    
     // Check if the ArduCAM SPI1 bus is OK
    myCAM1.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM1.read_reg(ARDUCHIP_TEST1);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI1 interface error!\n");
        exit(EXIT_FAILURE);
    } 	
    // Check if the ArduCAM SPI2 bus is OK
    myCAM2.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM2.read_reg(ARDUCHIP_TEST1);
    //printf("temp=%x\n",temp);      //debug
    if(temp != 0x55) {
        printf("SPI2 interface error!\n");
        exit(EXIT_FAILURE);
    }   	
    // Check if the ArduCAM SPI3 bus is OK
    myCAM3.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM3.read_reg(ARDUCHIP_TEST1);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI3 interface error!\n");
        exit(EXIT_FAILURE);
    } 
     // Check if the ArduCAM SPI4 bus is OK
    myCAM4.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM4.read_reg(ARDUCHIP_TEST1);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI4 interface error!\n");
        exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI4 interface OK!\n");
    	}
    
    // Change MCU mode
    myCAM1.write_reg(ARDUCHIP_MODE, 0x00); 
    myCAM2.write_reg(ARDUCHIP_MODE, 0x00); 
    myCAM3.write_reg(ARDUCHIP_MODE, 0x00); 
    myCAM4.write_reg(ARDUCHIP_MODE, 0x00); 
    // Check if the camera module type is OV2640
    myCAM1.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM1.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
        printf("Can't find OV2640 module!\n");
        exit(EXIT_FAILURE);
    } else {
        printf("OV2640 detected\n");
    }
}

int main(int argc, char *argv[])
{   
    if (argc == 1) {
        printf("Usage: %s [-s <resolution>] | [-c <filename]", argv[0]);
        printf(" -s <resolution> Set resolution, valid resolutions are:\n");
        printf("                   160x120\n");
        printf("                   176x144\n");
        printf("                   320x240\n");
        printf("                   352x288\n");
        printf("                   640x480\n");
        printf("                   800x600\n");
        printf("                   1024x768\n");
        printf("                   1280x960\n");
        printf("                   1600x1200\n");
        printf(" -c <filename>   Capture image\n");
        exit(EXIT_SUCCESS);
    }
    
    if (strcmp(argv[1], "-c") == 0 && argc == 7) {
        setup();
        myCAM1.set_format(JPEG);
        myCAM1.InitCAM();  
        // Change to JPEG capture mode and initialize the OV2640 module   
	      if (strcmp(argv[3], "160x120") == 0) myCAM1.OV2640_set_JPEG_size(OV2640_160x120);
	      else if (strcmp(argv[3], "176x144")  == 0) myCAM1.OV2640_set_JPEG_size(OV2640_176x144);
	      else if (strcmp(argv[3], "320x240")  == 0) myCAM1.OV2640_set_JPEG_size(OV2640_320x240);
	      else if (strcmp(argv[3], "352x288")  == 0) myCAM1.OV2640_set_JPEG_size(OV2640_352x288);
	      else if (strcmp(argv[3], "640x480")  == 0) myCAM1.OV2640_set_JPEG_size(OV2640_640x480);
	      else if (strcmp(argv[3], "800x600")  == 0) myCAM1.OV2640_set_JPEG_size(OV2640_800x600);
	      else if (strcmp(argv[3], "1024x768") == 0) myCAM1.OV2640_set_JPEG_size(OV2640_1024x768);
	      else if (strcmp(argv[3], "1280x960") == 0) myCAM1.OV2640_set_JPEG_size(OV2640_1280x1024);
	      else if (strcmp(argv[3], "1600x1200")== 0) myCAM1.OV2640_set_JPEG_size(OV2640_1600x1200);
	      else {
	      printf("Unknown resolution %s\n", argv[3]);
	      exit(EXIT_FAILURE);
	      }
        sleep(1); // Let auto exposure do it's thing after changing image settings
        printf("Changed resolution1 to %s\n", argv[6]);
        // Flush the FIFO
        myCAM1.flush_fifo();    
        // Clear the capture done flag
        myCAM1.clear_fifo_flag();
        // Start capture
        printf("CAM1 start capture\n");  
        myCAM1.start_capture();
        while (!(myCAM1.read_reg(ARDUCHIP_TRIG) & CAP_DONE_MASK)) ;
         printf("CAM1 Capture Done\n");
        
        // Flush the FIFO
        myCAM2.flush_fifo();    
        // Clear the capture done flag
        myCAM2.clear_fifo_flag();
        // Start capture
        printf("CAM2 start capture\n");  
        myCAM2.start_capture();
        while (!(myCAM2.read_reg(ARDUCHIP_TRIG) & CAP_DONE_MASK)) ;
        printf("CAM2 Capture Done\n");
        
         // Flush the FIFO
        myCAM3.flush_fifo();    
        // Clear the capture done flag
        myCAM3.clear_fifo_flag();
        // Start capture
        printf("CAM3 start capture\n");  
        myCAM3.start_capture();
        while (!(myCAM3.read_reg(ARDUCHIP_TRIG) & CAP_DONE_MASK)) ;
        printf("CAM3 Capture Done\n");
        
         // Flush the FIFO
        myCAM4.flush_fifo();    
        // Clear the capture done flag
        myCAM4.clear_fifo_flag();
        // Start capture
        printf("CAM4 start capture\n");  
        myCAM4.start_capture();
        while (!(myCAM4.read_reg(ARDUCHIP_TRIG) & CAP_DONE_MASK)) ;
        printf("CAM4 Capture Done\n");
         
            
         // Open the new file
        FILE *fp1 = fopen(argv[2], "w+");
         FILE *fp2 = fopen(argv[3], "w+");
          FILE *fp3 = fopen(argv[4], "w+");
           FILE *fp4 = fopen(argv[5], "w+");
        
        if (!fp1) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
        if (!fp2) {
            printf("Error: could not open %s\n", argv[3]);
            exit(EXIT_FAILURE);
        }
          if (!fp3) {
            printf("Error: could not open %s\n", argv[4]);
            exit(EXIT_FAILURE);
        }
        if (!fp4) {
            printf("Error: could not open %s\n", argv[5]);
            exit(EXIT_FAILURE);
        } 
         printf("Reading FIFO and saving IMG\n");  
        
        size_t len1 = myCAM1.read_fifo_length();
        size_t len2 = myCAM2.read_fifo_length();
        size_t len3 = myCAM3.read_fifo_length();
        size_t len4 = myCAM4.read_fifo_length();
      if ((len1>=MAX_FIFO_SIZE)||(len2>=MAX_FIFO_SIZE)||(len3>=MAX_FIFO_SIZE)||(len4>=MAX_FIFO_SIZE)){
		   printf("Over size.");
		    exit(EXIT_FAILURE);
		  }
		  if (len1 == 0 ) printf("Size1 is 0."); 
		  if (len2 == 0 ) printf("Size2 is 0."); 
		  if (len3 == 0 ) printf("Size3 is 0."); 
		  if (len4 == 0 ) printf("Size4 is 0.");  			
      int32_t i=0; 
      myCAM1.CS_LOW();  //Set CS low          
      myCAM1.set_fifo_burst();
      myCAM1.transfers(buffer,1);//dummy read  
      while(len1>BUF_LEN)
      {	 
      	myCAM1.transfers(&buffer[i],BUF_LEN);
      	len1 -= BUF_LEN;
      	i += BUF_LEN;
      }
       myCAM1.transfers(&buffer[i],len1); 
       myCAM1.CS_HIGH();//Set CS HIGH
       i=0;
       myCAM2.CS_LOW(); //Set CS2 low       
       myCAM2.set_fifo_burst();
       myCAM2.transfers(buffer,1);//dummy read  
      while(len2>BUF_LEN)
      {	 
      	myCAM2.transfers(&buffer[i],BUF_LEN);
      	len2 -= BUF_LEN;
      	i += BUF_LEN;
      }
       myCAM2.transfers(&buffer[i],len2); 
       myCAM2.CS_HIGH();//Set CS HIGH
       i=0;
      
       myCAM3.CS_LOW(); //Set CS3 low       
       myCAM3.set_fifo_burst();
       myCAM3.transfers(buffer,1);//dummy read  
      while(len3>BUF_LEN)
      {	 
      	myCAM3.transfers(&buffer[i],BUF_LEN);
      	len3 -= BUF_LEN;
      	i += BUF_LEN;
      }
       myCAM3.transfers(&buffer[i],len3); 
       myCAM3.CS_HIGH();//Set CS HIGH
       i=0;
      
      myCAM4.CS_LOW(); //Set CS4 low       
      myCAM4.set_fifo_burst();
      myCAM4.transfers(buffer,1);//dummy read  
      while(len4>BUF_LEN)
      {	 
      	myCAM4.transfers(&buffer[i],BUF_LEN);
      	len4 -= BUF_LEN;
      	i += BUF_LEN;
      }
      myCAM4.transfers(&buffer[i],len4); 
      myCAM4.CS_HIGH();//Set CS HIGH
      i=0;
      
      // Close the file
      fclose(fp1);
      fclose(fp2);
      fclose(fp3);
      fclose(fp4);
      // Clear the capture done flag
      myCAM1.clear_fifo_flag();
	    printf("IMG1 save OK !\n");
      myCAM2.clear_fifo_flag();
	    printf("IMG2 save OK !\n");
      myCAM3.clear_fifo_flag();
	    printf("IMG3 save OK !\n");
      myCAM4.clear_fifo_flag();
	    printf("IMG4 save OK !\n");
      
    } else {
        printf("Error: unknown or missing argument.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}