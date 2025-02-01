#pragma once
class ConsoleWindow
{
public:
    static void Create() {
        // 콘솔 윈도우 생성
        AllocConsole();

        // 표준 입출력 스트림을 콘솔로 리다이렉트
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);

        FILE* pCin;
        freopen_s(&pCin, "CONIN$", "r", stdin);

        // 표준 에러 스트림도 콘솔로 리다이렉트
        FILE* pCerr;
        freopen_s(&pCerr, "CONOUT$", "w", stderr);

        // C++ 스트림을 동기화
        std::ios::sync_with_stdio();
    }

    static void Destroy() {
        // 스트림 닫기
        fclose(stdout);
        fclose(stdin);
        fclose(stderr);

        // 콘솔 윈도우 해제
        FreeConsole();
    }
};

