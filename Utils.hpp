#pragma once

#include <version>  // For C++20 feature checking
#define BYTE unsigned char

class Utils {
public:
	static Utils* Get( ) {
		static Utils instance;
		return &instance;
	}

	ULONG_PTR FindSignatureInDriver( PDRIVER_OBJECT driverObject, const BYTE* signature, ULONG signatureSize ) {
		ULONG_PTR baseAddress = ( ULONG_PTR )driverObject->DriverStart;
		ULONG_PTR endAddress = baseAddress + driverObject->DriverSize - signatureSize;

		for ( ULONG_PTR address = baseAddress; address < endAddress; ++address ) {
			BOOLEAN match = TRUE;

			for ( ULONG offset = 0; offset < signatureSize; ++offset ) {
				UCHAR byte = *( ( UCHAR* )( address + offset ) );

				if ( signature [ offset ] != byte ) {
					match = FALSE;
					break;
				}
			}

			if ( match ) {
				return address;
			}
		}

		return 0x0001;  // Signature not found
	}

	ULONG_PTR DrvBase( const char* szModuleName ) {
		PVOID pModuleBase = NULL;
		PSYSTEM_MODULE_INFORMATION pSystemInfoBuffer = NULL;

		ULONG SystemInfoBufferSize = 0;

		NTSTATUS status = ZwQuerySystemInformation( SystemModuleInformation,
			&SystemInfoBufferSize,
			0,
			&SystemInfoBufferSize );

		if ( !SystemInfoBufferSize )
		{
			return NULL;
		}

		pSystemInfoBuffer = ( PSYSTEM_MODULE_INFORMATION )ExAllocatePoolWithTag( NonPagedPool, SystemInfoBufferSize * 2, '1gaT' );

		if ( !pSystemInfoBuffer )
		{
			return NULL;
		}

		memset( pSystemInfoBuffer, 0, SystemInfoBufferSize * 2 );

		status = ZwQuerySystemInformation( SystemModuleInformation,
			pSystemInfoBuffer,
			SystemInfoBufferSize * 2,
			&SystemInfoBufferSize );

		if ( NT_SUCCESS( status ) )
		{
			for ( int ModuleCount = 0; ModuleCount < pSystemInfoBuffer->Count; ModuleCount++ )
			{

				char* ModuleFileName = ( char* )pSystemInfoBuffer->Module [ ModuleCount ].FullPathName;

				int l = strlen( ModuleFileName );

				for ( int i = l; i != 0; i-- )
				{
					if ( ModuleFileName [ i ] == '\\' )
					{
						ModuleFileName = ModuleFileName + i + 1;
						break;
					}
				}

				if ( _stricmp( szModuleName, ModuleFileName ) == 0 )
				{
					pModuleBase = pSystemInfoBuffer->Module [ ModuleCount ].ImageBase;

					break;
				}

			}
		}

		ExFreePoolWithTag( pSystemInfoBuffer, 'qbWn' );
		return STATUS_SUCCESS; // Driver found
	} 
};