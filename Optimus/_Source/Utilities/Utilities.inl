bool isNaN ( const float val )
{

	volatile float value= val;
	return value != value;
}

bool isCloseEnough( float a, float b, float epsilon )
{
    float diff = fabs(a - b);

    float largest = ( fabs(a) > fabs(b) ) ? fabs(a) : fabs(b);
 
    if ( diff <= largest * epsilon )
        return true;
    return false;
}



#if defined _DEBUG
void printToConsole( int level, const char* fMt , ... )
{
	const int str_length = 128;

	char str[str_length];

	va_list args;

	va_start ( args,fMt);
	
	if( level >0 )
	vsprintf_s( str, str_length, fMt, args);

	va_end( args );

	if( level > 1 )
	OutputDebugStringA( str );
}

#endif
