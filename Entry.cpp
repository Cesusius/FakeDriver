#include "Imports.hpp"
char( __fastcall * o_AtapiHwBuildIo )( ... );
char __fastcall hk_AtapiHwBuildIo( __int64 a1, __int64 a2 )
{
	/* Execute anything..*/
	return Atapi::Get( )->EmulateHwBuildIoRoutine( a1, a2 );
} 

NTSTATUS DriverEntry( PDRIVER_OBJECT Arg1, PUNICODE_STRING Arg2 )
{
	/* Locate the vulnerable driver's base address
	   Could also be obtained from PEB->ImageBaseAddress but CBA :/ */
	const auto AtapiBaseAddress = Utils::Get( )->DrvBase( "atapi.sys" );
	if ( !AtapiBaseAddress )
		return 2;

	/* Function called upon driver initialization routine */
	const auto AtapiHwBuildIo = Utils::Get( )->FindSignatureInDriver( reinterpret_cast< PDRIVER_OBJECT >( AtapiBaseAddress ), "48 89 4A 10 48 8D 0D ? ? ? ?", 0x1000 );
	if ( !AtapiBaseAddress )
		return 3;

	o_AtapiHwBuildIo = ( char( * )( ... ) )AtapiHwBuildIo;
	o_AtapiHwBuildIo = ( char( * )( ... ) )InterlockedExchangePointer( ( PVOID* )&AtapiHwBuildIo, ( PVOID )hk_AtapiHwBuildIo );

	/* Swap it */ 
	InterlockedExchangePointer( ( PVOID* )&AtapiHwBuildIo, o_AtapiHwBuildIo );

	return STATUS_SUCCESS;
}