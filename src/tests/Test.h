#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "../system/GameManager.h"

namespace test 
{
	class Test
	{
	public:
		GameManager* testGM;

		Test(GameManager* GM);
		virtual ~Test();

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnGUIRender() {}
	private:
	};

	class TestMenu : public Test
	{
	public:
		GameManager* TestMenuGM;

		template <typename T>
		void AddTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			tests.push_back(std::make_pair(name, [](GameManager* GM) { return new T(GM); }));
		}
	private:
		Test*& current_test;
		std::vector<std::pair<std::string, std::function<Test* (GameManager* GM)>>> tests;

	public:
		TestMenu(GameManager* GM, Test*& current_test_ptr);
		~TestMenu();
	
		void OnGUIRender() override;
	private:
	};
}



