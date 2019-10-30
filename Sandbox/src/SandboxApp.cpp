#include <PepperMint.h>

#include <iostream>

class Sandbox : public PepperMint::Application {
public:
	Sandbox() = default;
	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}