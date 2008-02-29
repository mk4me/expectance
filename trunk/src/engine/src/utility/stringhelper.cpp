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

/**
 * Parses string and extract drom it an array of float numbers
 *
 * \param const std::string & str - String to be parsed
 * \param float* floatArray - array to which floatnumbers will be written
 * \pamar int arraySize - size of array which string should define. If string has represents an array of different size this
 *                           method failes nad returns false
 * \return bool - true if parsing successfull, false otherwise
 **/
bool StringHelper::ReadFloatArrayFromString(const string& str, float* floatArray, int arraySize)
{
    vector<string> _tokens;
    string _arrayStr(str);
	_arrayStr = StringHelper::ClearDelimiters(_arrayStr, '(',')');
	_arrayStr = StringHelper::RemoveChar(_arrayStr,' ');
	_tokens = StringHelper::Split(_arrayStr, ",");

    if ((_tokens[0] == "<KEY_NOT_FOUND>")||(_tokens.size()!=arraySize))
		return false;

	for (unsigned int i = 0; i <_tokens.size(); i++)
	{
		std::istringstream is(_tokens[i]);
		is >> floatArray[i];
		if( !is )
			return false;
	}
	return true;
}

template <typename T> T StringHelper::FromStringEx( const std::string& s, bool& bSuccess)
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

string StringHelper::itos(int i)
{
	stringstream s;
	s << i;
	return s.str();
}
