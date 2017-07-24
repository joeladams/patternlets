// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
//  InputList.h  
//  -----------
//  Header file for input-output to file. Emulates the 
//  namelist facility in Fortran. See the InputList.C 
//  source for detailed documentation of the member 
//  functions
//--------------------------------------------------------

#include <string>
#include <list>

#ifndef INPUTLIST
#define INPUTLIST
////////////////

typedef enum {EMPTY=0, NI, NL, NUI, NUL, NF, ND} DataType;

// -----------------------------------------------------
// The meaning of the vStatus field in the 
// InputObj class is the following:
//
// 0 = OK
// 1 = no data
// 2 = missing data (input array too short)
// 3 = extra data on input line (input array too long)
// ----------------------------------------------------

class InputObj
   {
   public:
   //////
   std::string   vName;
   void          *vPtr;
   DataType      vType;
   int           vLen;
   int           vStatus;
   
   InputObj() {}
   InputObj(std::string myname, void *addr, DataType type, int size);
   };

class InputList
   {
   private:
   std::list<InputObj> L;
   
   int InitializeObject(std::string& S);

   public:   
   InputList() {}
   ~InputList();

   void RegisterData(std::string myname, void *addr, DataType type, int size);
   void UnregisterData(std::string myname);
   void UnregisterAllData();
   void ReportStatus();
   
   int ReadData(const char *filename);
   int ReadData(const char *filename, const char *delim);
   void PrintData(); 
   };

#endif  // INPUTLIST
