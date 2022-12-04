#pragma once

typedef void(*EXIT)(void);

template<typename T>
class CSingleton
{
private:
	static T* m_Inst; // 템플릿 인수로 받은 클라스를 지징하는 포인터 변수 -> 각 클라스 포인터는 데이터 영역에 저장된다.

public:
	static T* GetInst(); // 객체 없이 소환 될 수 있어야 하기 때문에 정적 맴버 함수 여야 한다.
	static void Destroy();

public:
	CSingleton() {}
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr; // Static 이기 때문에 초기화 해준다.

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = new T;
		atexit((EXIT) &CSingleton<T>::Destroy); // 함수 포인터를 인자로 받아서 메인 함수가 끝날 때 호출한다.
	}

	return m_Inst;
}

template<typename T>
inline void CSingleton<T>::Destroy()
{
	if (nullptr != m_Inst)
	{
		delete m_Inst;
		m_Inst = nullptr;
	}
}
