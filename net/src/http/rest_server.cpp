#include "rest_server.h"

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <thread>
#include <nlohmann/json.hpp>


using namespace std;
 
namespace lm {
 
	RestServer* RestServer::getInstance() {
		static RestServer* INSTANCE = new RestServer();
		return INSTANCE;
	}

	RestServer::RestServer() {

		CROW_ROUTE(app, "/") ( []() {
			return "/ alive"; 
			});

 
		std::function<string()> aStatusProvider = [&]() {
			std::stringstream ss;
			ss << "endpoint /status alive" << endl;
			return ss.str();
			};

		CROW_ROUTE(app, "/status/") (aStatusProvider);


		CROW_ROUTE(app, "/image/last/<int>")
			([&](int lastN) {
			std::stringstream ss;
			int counter = 0;

			return ss.str();
		});

 
		CROW_ROUTE(app, "/image/start/<int>/end/<int>")
			([&](double birthTime, int idx) {
				std::stringstream ss;
				return ss.str();
			});

	}

	std::thread* RestServer::start() {
		auto t = new thread([&]() {
			app.port(port).multithreaded().run();
			});
		t->detach();
		return t;
		//app.port(18080).multithreaded().run();
	}

	RestServer::~RestServer()
	{
	}
 
};

