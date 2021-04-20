#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>

int lll(int argc, wchar_t* argv[])
{
	LPGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	NET_API_STATUS nStatus;

	if (argc != 3)
	{
		fwprintf(stderr, L"Usage: %s \\\\ServerName UserName\n", argv[0]);
		exit(1);
	}

	nStatus = NetUserGetGroups(argv[1],
		argv[2],
		dwLevel,
		(LPBYTE*)&pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries);

	if (nStatus == NERR_Success)
	{
		LPGROUP_USERS_INFO_0 pTmpBuf;
		DWORD i;
		DWORD dwTotalCount = 0;

		if ((pTmpBuf = pBuf) != NULL)
		{
			fprintf(stderr, "\nGlobal group(s):\n");

			for (i = 0; i < dwEntriesRead; i++)
			{
				assert(pTmpBuf != NULL);

				if (pTmpBuf == NULL)
				{
					fprintf(stderr, "An access violation has occurred\n");
					break;
				}

				wprintf(L"\t-- %s\n", pTmpBuf->grui0_name);

				pTmpBuf++;
				dwTotalCount++;
			}
		}

		if (dwEntriesRead < dwTotalEntries)
			fprintf(stderr, "\nTotal entries: %d", dwTotalEntries);

		printf("\nEntries enumerated: %d\n", dwTotalCount);
	}
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}