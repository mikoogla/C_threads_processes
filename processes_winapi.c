#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>
#define BUFSIZE 1024
#define VARNAME "varname"
HANDLE Mutex;

struct Arguments
{
    int _argc;
    int p;
    char _argv[64];
} a;

void _tmain(int argc, char **argv)
{
    int flag = 0;
    LPSTR nr;
    HANDLE file = CreateFileA("parent_info", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        //puts("Child");
    }
    else
    {
        a._argc = argc;
        a.p = 1;
        for (int i = 1; i < argc; i++)
        {
            a._argv[i] = *argv[i];
        }
        flag = 1;
        //puts("Parent");
        Mutex = CreateMutex(NULL, FALSE, NULL);
        WaitForSingleObject(Mutex, INFINITE);
        FILE *fptr = fopen("sth", "wb");
        fwrite(&a, sizeof(struct Arguments), 1, fptr);
        fclose(fptr);
        ReleaseMutex(Mutex);
        SetEnvironmentVariable(VARNAME, "1");
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE sth;
    int k = GetCurrentProcessId();
    sth = &k;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (flag == 1 && argc <= 1)
    {
        puts("Brak argumentow");
        goto label;
    }
    if (flag == 1)
    {
        for (int i = 1; i < argc; i++)
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            if (!CreateProcess(argv[0], // No module name (use command line)
                               NULL,    // Command line
                               NULL,    // Process handle not inheritable
                               NULL,    // Thread handle not inheritable
                               TRUE,    // Set handle inheritance to FALSE
                               0,       // No creation flags
                               NULL,    // Use parent's environment block
                               NULL,    // Use parent's starting directory
                               &si,     // Pointer to STARTUPINFO structure
                               &pi)     // Pointer to PROCESS_INFORMATION structure
            )
            {
                printf("CreateProcess failed (%d).\n", GetLastError());
                return;
            }
            else
            {
                // puts("process created");
            }
        }
    }
    else
    {
        // puts("Child process running: ");
        WaitForSingleObject(Mutex, INFINITE);
        struct Arguments b;
        FILE *fptr_1 = fopen("sth", "rb");
        fread(&b, sizeof(struct Arguments), 1, fptr_1);
        fclose(fptr_1);
        DeleteFileA("sth");

        printf("Argument: %c\n", b._argv[b.p]);

        b.p++;

        FILE *fptr_2 = fopen("sth", "wb");
        fwrite(&b, sizeof(struct Arguments), 1, fptr_2);
        fclose(fptr_2);
        ReleaseMutex(Mutex);
    }

label:

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(file);
    DeleteFileA("parent_info");
    if (flag == 1)
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DeleteFileA("sth");
    }
}
