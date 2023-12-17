#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include <memory>

namespace engine
{
	// on macOS the operating system calls the application instead of the other way around, so we'll adopt this
	// approach for our framework as well.
	class IApplicationDelegate
	{
	public:
		virtual void applicationDidFinishLaunching();
	};

	class Application final
	{
	public:
		explicit Application();
		~Application();

		void run();

		IApplicationDelegate* getDelegate(); // change to shared pointer?

		void setDelegate(IApplicationDelegate* delegate);

		struct Implementation;

	private:
		std::unique_ptr<Implementation> pImplementation;

		IApplicationDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_APPLICATION_H