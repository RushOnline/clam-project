#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <string>

namespace CLAMTest
{

class LadspaFactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( LadspaFactoryTest );

class LadspaFactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LadspaFactoryTest );
	CPPUNIT_TEST( testCreate_existing );
	CPPUNIT_TEST( testCreate_nonExistingKey_throws );
//	CPPUNIT_TEST( testCreate_withTwoCreators );
	CPPUNIT_TEST_SUITE_END();
protected:
	class Processing //TODO delete. it should be CLAM::Processing 
	{
	public:
		virtual ~Processing() {};
	};
	class LadspaWrapper : public Processing //TODO move to CLAM/src/Processing/Plugins (in namespace CLAM)
	{
		std::string _libraryFileName;
		unsigned _index;
	public:
		LadspaWrapper( const std::string& libraryFileName, unsigned index ) 
			: _libraryFileName( libraryFileName )
			, _index( index )
		{
		}
		const std::string& LibraryFileName() 
		{ 
			return _libraryFileName; 
		}
		unsigned Index()
		{
			return _index;
		}
	};
	class LadspaWrapperCreator  //TODO move to CLAM/src/Processing/Plugins
	{
		std::string _libraryFileName;
		unsigned _index;
	public:
		LadspaWrapperCreator( const std::string& libraryFileName, unsigned index ) 
			: _libraryFileName( libraryFileName )
			, _index( index )
		{
		}
		virtual Processing* Create()
		{
			return new LadspaWrapper(_libraryFileName, _index);
		}
		virtual ~LadspaWrapperCreator() {};
	};
	class LadspaFactory
	{
		std::string _key;
		LadspaWrapperCreator* _creator;
	public:
		LadspaFactory() : _creator(0)
		{
		}
		class NonExistingKeyError
		{
		};
		Processing* Create(const std::string& key)
		{
			if (!_creator)
				throw NonExistingKeyError();
		
			return _creator->Create();
		}
		void AddCreator( const std::string& key, LadspaWrapperCreator* creator )
		{
			_key = key;
			_creator = creator;
		}
	};

	void testCreate_existing()
	{
		LadspaFactory factory;
		factory.AddCreator( "foo", new LadspaWrapperCreator("lib.so", 1) );
		Processing* processing = factory.Create("foo");
		LadspaWrapper* wrapper = dynamic_cast<LadspaWrapper*>(processing);
		CPPUNIT_ASSERT_EQUAL( std::string("lib.so"), wrapper->LibraryFileName() );
		CPPUNIT_ASSERT_EQUAL( 1u, wrapper->Index() );
	}
	void testCreate_nonExistingKey_throws()
	{
		LadspaFactory factory;
		try
		{
			factory.Create("non-existing");
			CPPUNIT_FAIL("expected an exception");
		} catch ( LadspaFactory::NonExistingKeyError& )
		{
		}
	}
	void testCreate_withTwoCreators()
	{
//TODO
//
//  this test is still in RED. just enable-it from above
//
		LadspaFactory factory;
		factory.AddCreator( "foo", new LadspaWrapperCreator("libfoo.so", 1) );
		factory.AddCreator( "bar", new LadspaWrapperCreator("libbar.so", 2) );
		Processing* processing = factory.Create("foo");
		LadspaWrapper* wrapper = dynamic_cast<LadspaWrapper*>(processing);
		CPPUNIT_ASSERT_EQUAL( std::string("libfoo.so"), wrapper->LibraryFileName() );
		CPPUNIT_ASSERT_EQUAL( 1u, wrapper->Index() );
		
	}
	
};

} //namespace


