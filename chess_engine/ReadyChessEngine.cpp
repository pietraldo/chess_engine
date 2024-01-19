#include "ReadyChessEngine.h"


wstring ReadyChessEngine::engine_path_exe = L"stockfish/stockfish.exe";

string ReadyChessEngine::GiveAnswer(const char* command)
{

    // Create pipes for communication
    HANDLE hChildStdoutRd, hChildStdoutWr;
    HANDLE hChildStdinRd, hChildStdinWr;

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0) ||
        !CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
        std::cerr << "CreatePipe failed" << std::endl;
        return "";
    }

    // Set up members of the PROCESS_INFORMATION structure
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = hChildStdoutWr;
    siStartInfo.hStdOutput = hChildStdoutWr;
    siStartInfo.hStdInput = hChildStdinRd; // Set the standard input of the child process
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process
    if (!CreateProcess(NULL,
        const_cast<LPWSTR>(engine_path_exe.c_str()),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &siStartInfo,
        &piProcInfo)) {
        std::cerr << "CreateProcess failed" << std::endl;
        return "";
    }

    // Close handles to the child process and its primary thread
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    // Close the write end of the pipes
    CloseHandle(hChildStdoutWr);
    CloseHandle(hChildStdinRd);

    // Write input to the child process
    DWORD bytesWritten;
    WriteFile(hChildStdinWr, command, strlen(command), &bytesWritten, NULL);

    // Close the write end of the pipe
    CloseHandle(hChildStdinWr);

    // Read the output from the child process
    const int bufferSize = 4096;
    char buffer[bufferSize];
    std::string output;

    DWORD bytesRead;
    while (ReadFile(hChildStdoutRd, buffer, bufferSize, &bytesRead, NULL) && bytesRead != 0) {
        output.append(buffer, bytesRead);
    }

    return output;
}


