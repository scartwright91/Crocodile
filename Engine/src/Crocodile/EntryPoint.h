#pragma once

// #ifdef CC_PLATFORM_WINDOWS

extern Crocodile::Application *Crocodile::CreateApplication();

int main(int argc, char *argv[])
{
	printf("----- Crocodile Engine -----\n");
	auto app = Crocodile::CreateApplication();
	app->run();
	delete app;
}

// #endif
