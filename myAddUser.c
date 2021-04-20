#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>

int wmain(int argc, wchar_t* argv[])
{
    USER_INFO_1 ui;
    DWORD dwLevel = 1;
    DWORD dwError = 0;
    NET_API_STATUS nStatus;

    if (argc != 4)
    {
        fwprintf(stderr, L"Usage:%s <username> <password> <localgroup>\n",argv[0]);
        exit(1);
    }

    ui.usri1_name = argv[1];
    ui.usri1_password = argv[2];
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_home_dir = NULL;
    ui.usri1_comment = NULL;
    ui.usri1_flags = UF_SCRIPT;
    ui.usri1_script_path = NULL;

    nStatus = NetUserAdd(NULL,
        dwLevel,
        (LPBYTE)&ui,
        &dwError);

    if (nStatus == NERR_Success)
        fwprintf(stderr, L"User %s has been successfully added\n", argv[1]);

    else
        fprintf(stderr, "A system error has occurred: %d\n", nStatus);

    LOCALGROUP_MEMBERS_INFO_3 account;
    account.lgrmi3_domainandname = argv[1];

    NET_API_STATUS Status = NetLocalGroupAddMembers(
        NULL,
        argv[3], 
        3, 
        (LPBYTE)&account,
        1);

    if (Status == NERR_Success || Status == ERROR_MEMBER_IN_ALIAS) {
        fwprintf(stderr, L"%s group added Successfully!\n", argv[3]);
    }
    else {
        fwprintf(stderr, L"%s group added Failed!\n", argv[3]);
    }
    return 0;
}