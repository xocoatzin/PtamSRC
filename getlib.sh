#cvs -z3 -d:pserver:anonymous@cvs.savannah.nongnu.org:/sources/toon   co -D "Mon May 11 16:29:26 BST 2009" TooN
#cvs -z3 -d:pserver:anonymous@cvs.savannah.nongnu.org:/sources/libcvd co -D "Mon May 11 16:29:26 BST 2009" libcvd
#cvs -z3 -d:pserver:anonymous@cvs.savannah.nongnu.org:/sources/libcvd co -D "Mon May 11 16:29:26 BST 2009" gvars3
#echo "Just hit ENTER for password"
#svn co http://seriss.com/public/fltk/fltk/branches/branch-1.3/ fltk-1.3

#cd libcvd

#echo    "TODO ---> add  #include <stddef.h>"
#gedit cvd/image.h
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <stdint.h>"
#gedit cvd/vision.h
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <stdint.h>"
#gedit cvd/draw.h
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <stdint.h>"
#gedit cvd/utility.h
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <stdio.h>"
#gedit cvd_src/convolution.cc
#read -p "Press enter to continue"

#echo    "TODO ---> change videodev.h for videodev2.h in lines 6975 and 7096"
#gedit configure
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <cstring>"
#gedit cvd/internal/convert_pixel_types.h
#read -p "Press enter to continue"

#echo    "TODO ---> change #include <linux/videodev.h>  to  #include <linux/videodev2.h>"
#gedit cvd/Linux/v4l2frame.h
#read -p "Press enter to continue"

#echo    "TODO ---> substitute in line 79; add"
#echo    "        > #define K26(X) X"
#echo    "        > #define K24(X)"
#gedit cvd_src/Linux/v4l2buffer.cc
#read -p "Press enter to continue"

#cd ..

#cd gvars3/

#echo    "TODO ---> add #include <stdio.h>"
#gedit gvars3/serialize.h
#read -p "Press enter to continue"
#cd ..

#git clone https://github.com/BeLioN-github/PTAM.git

#cd PTAM

#echo    "TODO ---> add  #include <TooN/lapack.h>"
#gedit HomographyInit.cc
#read -p "Press enter to continue"

#echo    "TODO ---> add  #include <unistd.h>"
#gedit Tracker.cc
#read -p "Press enter to continue"

#cd ..


