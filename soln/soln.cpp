#include "crow/crow.h"
#include <cstdlib>

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")([]() {
		return "Hello world";
	});

	auto port_str = std::getenv("PORT");
	int port = 80;
	if (port_str != nullptr) {
		port = std::atoi(port_str);
	}

	app.port(port).multithreaded().run();
}
