#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace test 
{
	class Test
	{
	public:
		Test();
		virtual ~Test();

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnGUIRender() {}
	private:
	};

	class TestMenu : public Test
	{
	public:
		template <typename T>
		void AddTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test*& current_test;
		std::vector<std::pair<std::string, std::function<Test* ()>>> tests;

	public:
		TestMenu(Test*& current_test_ptr);
		~TestMenu();
	
		void OnGUIRender() override;
	private:
	};
}



