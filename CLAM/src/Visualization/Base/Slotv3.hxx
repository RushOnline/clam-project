#ifndef __SLOTV3__
#define __SLOTV3__

#include "Slot.hxx"
#include "CBL.hxx"
#include "Assert.hxx"

namespace SigSlot
{

template < typename ParmType1, typename ParmType2, typename ParmType3 >
class Slotv3 : public Slot
{
		typedef CBL::Functor3<ParmType1, ParmType2, ParmType3> WrappedFuncType;

public:

		Slotv3()
				: mIsInit( false )
		{
		}

		virtual ~Slotv3()
		{
		}

		template < class RefType, typename PtrMember >
		void Wrap( RefType thisRef, PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor3<ParmType1,ParmType2,ParmType3>*)0, *thisRef, pMember );
				mIsInit = true;
		}

		template <  typename PtrMember >
		void Wrap( PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor3<ParmType1,ParmType2,ParmType3>*)0, pMember );
				mIsInit = true;
		}


		const WrappedFuncType& GetMethod() const
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				return mFunctor;
		}

		void operator()( ParmType1 parm1, ParmType2 parm2, ParmType3 parm3 )
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				mFunctor( parm1, parm2, parm3 );
		}

private:

		WrappedFuncType   mFunctor;
		bool              mIsInit;

};

}

#endif // SlottedMethod.hxx
