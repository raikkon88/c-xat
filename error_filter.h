#ifndef ERROR_FILTER_H_
#define ERROR_FILTER_H_

/********************************************************
 * ERROR FILTER HEADER FILE
 ********************************************************/

 struct error{
   char message[200];
   int code;
 };


/**
 *
 */
struct error parseResult(int result);

#endif
