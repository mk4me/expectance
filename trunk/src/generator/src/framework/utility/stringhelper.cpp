#include "stringhelper.h"

using namespace ft;


/**
 * Splits the given String into tokens.
 *
 * \param const std::string & str - String to be split
 * \param const std::string & delimiters - tokens separators array 
 * \return std::vector<string> - an array of tokens.
 **/
vector<string> StringHelper::Split(const string& str,const string& delimiters)
{
  vector<string> tokens;
  
  string::size_type lastPos = 0, pos = 0;  
  int count = 0;
  
  if(str.length()<1)  return tokens;
  
  // skip delimiters at beginning.  
  lastPos = str.find_first_not_of(delimiters, 0);
      
  if((str.substr(0, lastPos-pos).length()) > 0)
  {  	
  	count = str.substr(0, lastPos-pos).length();  	

  	for(int i=0; i < count; i++)  	
  	 	tokens.push_back("");
  	
  	if(string::npos == lastPos)
  		tokens.push_back("");
  }

  // find first "non-delimiter".
  pos = str.find_first_of(delimiters, lastPos);
  
  while (string::npos != pos || string::npos != lastPos)
  {  	      	    
     	// found a token, add it to the vector.
     	tokens.push_back( str.substr(lastPos, pos - lastPos));
				
    	// skip delimiters.  Note the "not_of"
     	lastPos = str.find_first_not_of(delimiters, pos);   	   	    
		
		if((string::npos != pos) && (str.substr(pos, lastPos-pos).length() > 1))  		
  		{
  			count = str.substr(pos, lastPos-pos).length();

  			for(int i=0; i < count; i++)
  	 			tokens.push_back("");
		}
		
  		pos = str.find_first_of(delimiters, lastPos);
  }

	return tokens;
}


/**
 * Clears delimiters from string
 *
 * \param const std::string & str - String to be corrected
 * \param const char start_bracket - First delimiter:  <,{,[ or (
 * \param const char end_bracket - Second delimiter: >,},] or )
 * \return std::string - Modified string
 **/
string StringHelper::ClearDelimiters(const string& str, const char start_bracket, const char end_bracket)
{
	int start = str.find_first_not_of(start_bracket);
	int end = str.find_last_of(end_bracket);
	if ((start>=0)&&(end > start))
	{
		end-=1;
		return str.substr(start,end);
	}

	return str;
}

/**
 * Removes specified character from string
 *
 * \param const std::string & str - String to be corrected
 * \param const char character - char literal to by removed
 * \return std::string - Modified string
 **/
string StringHelper::RemoveChar(const string& str, const char character)
{
	std::string a;
	unsigned int j=0;
	for(unsigned int i = 0; i < str.size(); i++)
	{
		if(str[i]==character)
		{
			a += str.substr(j,i-j);
			j=i+1;
		}
	}
	a+=str.substr(j,str.length());

	return a;
}

template <typename T> static T StringHelper::FromStringEx( const std::string& s, bool& bSuccess)
// string conversion from string to typename T with a flag
// the represents success or failure.
//
// e.g: int d = FromString<int>( s );
//
// if you want to check whether the conversion was successful then you can
// use the bSuccess flag!
{
	bSuccess = true;
	std::istringstream is(s);
	T t;
	is >> t;
	if( !is )
	bSuccess = false;
	return t;
}