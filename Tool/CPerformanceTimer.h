// CTerrain.h에 추가
#include <WTypesbase.h>
class CPerformanceTimer
{
private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_endTime;

public:
	CPerformanceTimer() : m_frequency(), m_startTime(), m_endTime()
    {
        QueryPerformanceFrequency(&m_frequency);
    }

    void Start() {
        QueryPerformanceCounter(&m_startTime);
    }

    double End() {
        QueryPerformanceCounter(&m_endTime);
        return (double)(m_endTime.QuadPart - m_startTime.QuadPart) / m_frequency.QuadPart * 1000.0; // ms 단위
    }
};