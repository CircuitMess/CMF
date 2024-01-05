#ifndef CMF_APPLICATIONSTATICS_H
#define CMF_APPLICATIONSTATICS_H

class Application;

class ApplicationStatics {
public:
	static Application* GetApplication() noexcept;
};

#endif //CMF_APPLICATIONSTATICS_H