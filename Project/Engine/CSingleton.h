#pragma once

typedef void(*EXIT)(void);

template<typename T>
class CSingleton
{
private:
	static T* m_Inst; // ���ø� �μ��� ���� Ŭ�󽺸� ��¡�ϴ� ������ ���� -> �� Ŭ�� �����ʹ� ������ ������ ����ȴ�.

public:
	static T* GetInst(); // ��ü ���� ��ȯ �� �� �־�� �ϱ� ������ ���� �ɹ� �Լ� ���� �Ѵ�.
	static void Destroy();

public:
	CSingleton() {}
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr; // Static �̱� ������ �ʱ�ȭ ���ش�.

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = new T;
		atexit((EXIT) &CSingleton<T>::Destroy); // �Լ� �����͸� ���ڷ� �޾Ƽ� ���� �Լ��� ���� �� ȣ���Ѵ�.
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
