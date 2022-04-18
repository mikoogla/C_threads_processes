#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>
#define VARNAME "SO2"
#define BUFSIZE 4096

void _tmain(int argc, char **argv)
{
    
    SECURITY_ATTRIBUTES sec;
        sec.nLength = sizeof(SECURITY_ATTRIBUTES);
        sec.lpSecurityDescriptor = NULL;
        sec.bInheritHandle = TRUE;

    HANDLE sem;
    DWORD dwRet, dwErr;
    LPTSTR pszOldVal;
    DWORD dwFlags = 0;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL fExist, fSuccess;
    pszOldVal = (LPTSTR)malloc(BUFSIZE * sizeof(TCHAR));


    if (NULL == pszOldVal)
    {
        printf("Out of memory\n");
        return;
    }

    dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, BUFSIZE);

    if (0 == dwRet)
    {
        dwErr = GetLastError();
        if (ERROR_ENVVAR_NOT_FOUND == dwErr)
        {
            fExist = FALSE;
        }
    }
    else if (BUFSIZE < dwRet)
    {
        pszOldVal = (LPTSTR)realloc(pszOldVal, dwRet * sizeof(TCHAR));
        if (NULL == pszOldVal)
        {
            printf("Out of memory\n");
            return;
        }
        dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, dwRet);
        if (!dwRet)
        {
            printf("GetEnvironmentVariable failed (%d)\n", GetLastError());
            return;
        }
        else
            fExist = TRUE;
    }
    else
        fExist = TRUE;

    if (fExist == TRUE && strcmp(pszOldVal, "NEW") == 0)
    {
        puts("SO2=NEW");

        HANDLE file = CreateFileA("parent_info", GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE){
            sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "semaphore");
            puts("Releasing...");
            ReleaseSemaphore(sem, 1, NULL);
            puts("Released...");
            CloseHandle(file);
            WaitForSingleObject(sem, INFINITE);
            return;
        }
        sem = CreateSemaphore(NULL, 0, 1, "semaphore");
        puts("Waiting...");
        WaitForSingleObject(sem, INFINITE);
        CloseHandle(sem);
        puts("Released");
        CloseHandle(file);
        return;
    }
    else
    {
        HANDLE file = CreateFileA("parent_info", GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE)
        {
            puts("Process already running");
            return;
        }
        getchar();
        CloseHandle(file);
        DeleteFileA("parent_info");
        return;
    }
}
