#ifndef CMF_APPLICATIONSTATICS_H
#define CMF_APPLICATIONSTATICS_H

class Application;

class ApplicationStatics {
public:
	static Application* getApplication() noexcept;
};

#endif //CMF_APPLICATIONSTATICS_H