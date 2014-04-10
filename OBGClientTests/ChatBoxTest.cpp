#include "stdafx.h"
#include "CppUnitTest.h"
#include "ChatBox.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::wstring ToString(const std::string &str) {
	return std::wstring(str.begin(), str.end());
}

namespace OBGClientTests
{
	TEST_CLASS(ChatBoxTest)
	{
	public:
		ChatBox * chatBox;

		void setup() {
			chatBox = new ChatBox();
		}

		void teardown() {
			delete chatBox;
		}
		TEST_METHOD(HandleMessage)
		{
			setup();

			for(int i = 0; i < 10; i ++) {
				chatBox->handleMessage("asdf");
			}
			for(int i = 0; i < 10; i ++) {
				std::string asdf = "asdf";
				Assert::AreEqual(asdf, *chatBox->getMessages()[i]);
			}

			chatBox->handleMessage("aaaa");
			std::string aaaa = "aaaa";
			Assert::AreEqual(aaaa, *chatBox->getMessages()[0]);

			teardown();
		}

		TEST_METHOD(PushCharacterSubmitTest) {
			setup();

			chatBox->enterEditMode();
			chatBox->pushCharacter('a');
			chatBox->submit();
			std::string msg = "me: a";
			Assert::AreEqual(msg, *chatBox->getMessages()[0]);

			teardown();
		}

		TEST_METHOD(PopCharacterSubmitTest) {
			setup();

			chatBox->enterEditMode();
			chatBox->pushCharacter('a');
			chatBox->popCharacter();
			chatBox->submit();

			Assert::IsNull(chatBox->getMessages()[0]);

			teardown();
		}

		TEST_METHOD(CancelTest) {
			setup();

			chatBox->enterEditMode();
			chatBox->pushCharacter('a');
			chatBox->cancel();
			chatBox->enterEditMode();
			chatBox->submit();

			Assert::IsNull(chatBox->getMessages()[0]);

			teardown();
		}

	};
}