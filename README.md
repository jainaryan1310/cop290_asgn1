# cop290_asgn1
COP 290 | Assignment 1 | Using OpenCV for traffic density detection | Aryan Jain (2019CS10334) &amp; Avantika Agarwal (2019CS10338)

# asgn1_part1
compile using : make asgn1_sub1 <br />
Run executable using : ./asgn1_sub1 image.jpg <br />

# asgn1_part2
compile using : make <br />
Run executable using : ./asgn1_sub2 video.mp4 image.jpg <br />
To remove previously compiled executables : make clean <br />
replace image.jpg with an image that has to be taken as the background for the process <br />
replace video.mp4 with the video file <br />
Choose the corners of the rectangle in an anticlockwise manner <br />

# Subtask 3
Method 1: method1.csv contains the final results and graphs <br />
          m1base.csv contains actual results using which errors are calculated <br />
          m1file"x".csv where x is a number, contains the runtime and the output obtained on skipping x frames and m1file"x".xlsx contains the error calculation <br />
          method1.cpp can be compiled using "make method1" and run using "./method1 trafficvideo.mp4 empty.jpg" <br />
          
Method 2: method2.csv contains the final results and graphs <br />
          m2factor"x".csv where x is a number, contains the output obtained on reducing resolution by a factor of x and m1file"x".xlsx contains the error calculation <br />
          method2_runtimes.csv contains the runtimes for the different resolutions <br />
          method2.cpp can be compiled using "make method2" and run using "./method2 trafficvideo.mp4 empty.jpg" <br />

Method 3: method3.csv contains the final results and graphs <br />
          m3thread"x".csv where x is a number, contains the output obtained on using x threads and m1file"x".xlsx contains the error calculation <br />
          method2_runtimes.csv contains the runtimes for different number of threads <br />
          method3.cpp can be compiled using "make method3" and run using "./method3 trafficvideo.mp4 empty.jpg" <br />

Method 4: method4.csv contains the final results and graphs <br />
          m4file"x".csv where x is a number, contains the runtime and the output obtained on using x threads <br />
          method4.cpp can be compiled using "make method4" and run using "./method4 trafficvideo.mp4 empty.jpg" <br />
