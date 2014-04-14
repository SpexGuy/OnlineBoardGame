#include "stdafx.h"
#include "CppUnitTest.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionShapeInflater.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{
	TEST_CLASS(CollisionShapeInflaterTest)
	{
	public:
		
		TEST_METHOD(SphereInflaterTest)
		{
			 SphereInflater *inflater = new SphereInflater(2.0);
			 btSphereShape *sphere = inflater->inflate();

			 Assert::AreEqual((double) sphere->getRadius(), 2.0);

			 delete inflater;
			 delete sphere;
		}
		
		TEST_METHOD(BoxInflaterTest)
		{
			btVector3 param(1,2,3);
			BoxInflater *inflater = new BoxInflater(param);
			btBoxShape *box = inflater->inflate();
			btVector3 halfExtents = box->getHalfExtentsWithMargin();
			
			Assert::AreEqual(param, halfExtents);
			delete inflater;
			delete box;
		}

		TEST_METHOD(CylinderInflaterTest)
		{
			btVector3 param(1,2,3);
			CylinderInflater *inflater = new CylinderInflater(param);
			btCylinderShape *cylinder = inflater->inflate();
			btVector3 halfExtents = cylinder->getHalfExtentsWithMargin();
			
			Assert::AreEqual(param, halfExtents);

			delete inflater;
			delete cylinder;
		}

		TEST_METHOD(ConeInflaterTest)
		{
			 ConeInflater *inflater = new ConeInflater(2.0, 1.0);
			 btConeShape *cone = inflater->inflate();

			 Assert::AreEqual((double) cone->getRadius(), 2.0);
			 Assert::AreEqual((double) cone->getHeight(), 1.0);

			 delete inflater;
			 delete cone;
		}
	};
}
