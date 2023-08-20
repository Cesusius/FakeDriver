#pragma once

#include <version>  // For C++20 feature checking
#include "IDA_Defs.hpp" 

class Atapi {
public:
	static Atapi* Get( ) {
		static Atapi instance;
		return &instance;
	}

	char __fastcall EmulateHwBuildIoRoutine( __int64 a1, __int64 a2 )
	{
        if ( ( *( _DWORD* )( a1 + 136 ) & 8 ) != 0 && ( *( _DWORD* )( a2 + 24 ) & 3 ) != 0 )
        {
            if ( ( *( _WORD* )a2 & 0x200 ) != 0 )
                *( _DWORD* )( a2 + 16 ) &= ~2u;
            else
                *( _BYTE* )( a2 + 2 ) = 4;
        }
        if ( ( *( _DWORD* )( a1 + 136 ) & 0x10 ) != 0
            && *( _DWORD* )( a2 + 24 ) > 0x2000u
            && ( *( _DWORD* )( a2 + 24 ) & 0x1FFFu ) - 1 <= 0x1FE )
        {
            if ( ( *( _WORD* )a2 & 0x200 ) != 0 )
                *( _DWORD* )( a2 + 16 ) &= ~2u;
            else
                *( _BYTE* )( a2 + 2 ) = 4;
        }
        return 1;
	}
};