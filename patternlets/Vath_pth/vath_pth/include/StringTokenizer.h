// File StringTokenizer.C
// Create a StringTokenizer class that tokenizes a string. 
// ---------------------------------------------------------
#include <string> 
 
using namespace std; 

// -------------------------------------------------------- 
// The StringTokenizer class is used to tokenize a string. 
// Pass in the constructor the string to be tokenized. 
// To obtain the next token, call get_token(), passing 
// each time a string that contains the token delimiters.
// --------------------------------------------------------
 
class StringTokenizer 
   { 
   string S; 
   string::size_type bidx;     // begin index 
   string::size_type eidx;     // end index
 
   public: 
   StringTokenizer() 
      { 
      S = ""; 
      bidx = 0; 
      } 
 
   // Define string to be tonekized
   void SetString(string& input) { S = input; }

   // Return a token from the string. 
   string GetToken(const string &delims); 
   }; 

// ------------------------------------------- 
// Return a token from the string. Return an  
// empty string when no more tokens are found. 
// Pass the delimiters in delims. 
// ------------------------------------------

string StringTokenizer::GetToken(const string &delims) 
   { 
   // Return an empty string when there are no more 
   // tokens to return. 
   if(bidx == string::npos)
       {
       bidx = 0;
       S = "";
       return S;
       } 
 
   // Beginning at bidx, find the next delimiter. 
   eidx = S.find_first_of(delims, bidx); 
 
   // Construct a string that contains the token. 
   string tok( S.substr(bidx, eidx-bidx) ); 
 
   // Find the start of the next token. This is a  
   // character that is not a delimiter. 
   bidx = S.find_first_not_of(delims, eidx); 
 
   // Return the next token. 
   return tok; 
   } 

