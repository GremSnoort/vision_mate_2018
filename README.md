# Nyasha_0X4_vision

cv_detect.h & cv_detect.cpp: files with functions that are common for both modules.
                            They contain declaration of static variables COLOR and TYPE that are used for keeping current color and type of recognised object.
                            Also, there are declared and realized static functions for selecting areas (static Mat MakeSelection), 
                                                                                       searching colors (static Mat ColorFilter),
                                                                                       checking frame if it contains large color areas (static bool tmp_detector),
                                                                                       debug selection (static Mat debug).

cv_colorfiguredetect.h & cv_colorfiguredetect.cpp: files of color figure detecting module.
cv_colorfiguredetect.h: there are two static functions (Detect and Debug) declared.
cv_colorfiguredetect.cpp: consists of Detect and Debug functions' realization. 
                          Detect function is the main function of this module for final detection.
                          It uses auxiliary functions that are described in cv_detect.h & cv_detect.cpp.
                          To get output frame with recognised objects user needs to call Detect function and give it camera frame as an argument.
                          Detect function returns output final frame.
                          Debug function is used for debug mode.
                          
cv_textdetect.h & cv_textdetect.cpp: files of text detecting module.
                                    They contain declaration of static array of letter templates and describtion of two static functions:
                                          static Mat Detect - the main function of this module (it also uses auxiliary functions that are described in cv_detect.h & cv_detect.cpp)
                                          and static Mat PrintAns - for printing the output on the frame.

cv_settingswidget.h & cv_settingswidget.cpp: files for settings options. This module describes a separate widget for changing the program options.

cv_datatypes.h: single header file with describtion of new datatypes used in detection process.
                enum type - for keeping type of detect object,
                enum figureColor - color indexes,
                struct color - for keeping separate ranges of RGB model,
                struct default_color - for keeping color in RGB model,
                struct on_off - for checking which colors (from three available colors) are activated for recognition.
                
RovCameraWidget.h & RovCameraWidget.cpp: describtion of connection with camera and processing data flow from it.                
