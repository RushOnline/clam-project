#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "similarityHelper.hxx"
#include "Spectrum.hxx"
#include "SpectralAnalysisExampleHelper.hxx"
#include "SpectralPeakDetectExampleHelper.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakDetect.hxx"
#include "XMLStorage.hxx"
#include "Plots.hxx"
#include <fstream>

namespace CLAMTest
{
	class SpectralPeakDetectFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDetectFunctionalTest );

	class SpectralPeakDetectFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SpectralPeakDetectFunctionalTest );
		CPPUNIT_TEST( test_ProofOfConcept );
		CPPUNIT_TEST( test_DetectsSamePeaksFromNoiseSignal );
		CPPUNIT_TEST_SUITE_END();

	protected:
		static double                            smSimilarityThreshold;
		static bool                              smBack2BackDataLoaded;
		static CLAM::SpectralPeakArray           smReferenceSpectralPeakArray;
		static CLAM::Spectrum                    smReferenceSpectrum;
		static CLAMExamples::SpectralPeaksSet    smReferenceSpectralPeaksSet;

		std::string                       mPathToTestData;

	protected: // Auxiliary methods
		
		static void loadBack2BackDataset( const std::string& pathToTestData )
		{
			CLAM::XMLStorage   storageIface;

			if ( !smBack2BackDataLoaded )
			{
				
				storageIface.Restore( smReferenceSpectralPeakArray,
						      pathToTestData + "OneSineSpectrum_RectWindow_P2_Peaks.xml" );

				storageIface.Restore( smReferenceSpectrum,
						      pathToTestData + "OneSineSpectrum_RectWindow_P2.xml" );

				storageIface.Restore( smReferenceSpectralPeaksSet,
						      pathToTestData + "DetectedPeaks_0001.xml" );

				smBack2BackDataLoaded = true;
			}
		}

	public:

		void setUp()
		{
			mPathToTestData = "../../../../../CLAM-TestData/spectralData/";
			loadBack2BackDataset( mPathToTestData );
		}

		void tearDown()
		{
		}

	private:

		void test_ProofOfConcept()
		{
			CLAM::XMLStorage storageIface;

			CLAM::SpectralPeakArray detectedPeaks;
			detectedPeaks.SetScale( CLAM::EScale::eLog );

			CLAM::SpectralPeakDetectConfig processingConfig; // default
			CLAM::SpectralPeakDetect       processing;

			smReferenceSpectrum.ToDB();

			processing.Configure( processingConfig );

			(*processing.FirstInput())->Attach( smReferenceSpectrum );
			(*processing.FirstOutput())->Attach( detectedPeaks );
			
			processing.Start();

			processing.Do();

			processing.Stop();

			CLAM::DataArray flattenedReference;
			CLAM::DataArray flattenedResult;

			flattenSpectralPeakArray( smReferenceSpectralPeakArray,
						  flattenedReference );

			flattenSpectralPeakArray( detectedPeaks,
						  flattenedResult );

			double similarity = evaluateSimilarity( flattenedReference,
								flattenedResult );

			// We dump the detected peaks if the similitude between
			// the reference dataset and the detected peaks is outside
			// our confidence interval
			if ( smSimilarityThreshold > similarity )
			{
				/*
				CLAMVM::SpectrumAndPeaksPlot refDataPlot( "plot_1" );
				refDataPlot.SetPosition( 0, 100 );
				refDataPlot.SetSize( 300, 300 );
				refDataPlot.SetYRange( -80, 500 );
				refDataPlot.SetLabel( "Reference dataset results" );
				refDataPlot.SetData( smReferenceSpectrum, 
						     smReferenceSpectralPeakArray );

				CLAMVM::SpectrumAndPeaksPlot actualDataPlot( "plot_2" );
				actualDataPlot.SetPosition( 320, 100 );
				actualDataPlot.SetSize( 300, 300 );
				actualDataPlot.SetYRange( -80, 500 );
				actualDataPlot.SetLabel( "Obtained peaks" );
				actualDataPlot.SetData( smReferenceSpectrum,
							detectedPeaks );

				CLAMVM::SystemPlots::DisplayAll();
				*/
				storageIface.Dump( detectedPeaks,
						   "PeaksFailedToPassTest",
						   "detectedPeaks_ProofOfConcept.xml" );
			}
			CPPUNIT_ASSERT( smSimilarityThreshold <= similarity );
		}

		void test_DetectsSamePeaksFromNoiseSignal()
		{
			// Load spectral analysis needed for repeating the peak detection
			// process

			CLAMExamples::SpectralAnalysis specAnalysis;
			CLAM::XMLStorage storageIFace;
			storageIFace.Restore( specAnalysis, 
					      mPathToTestData+smReferenceSpectralPeaksSet.GetSpectralAnalysis() );
			
			CLAM::SpectralPeakDetect peakDetector;
			peakDetector.Configure( smReferenceSpectralPeaksSet.GetPeakDetectionSettings() );

			CLAMExamples::SpectralPeaksSet detectedPeaks;
			CLAM::SpectralPeakArray tmpPeakArray;
			tmpPeakArray.SetScale( CLAM::EScale::eLog );

			peakDetector.Start();

			CLAMExamples::SpectralAnalysis::FramesContainer::iterator i =
				specAnalysis.GetResultingFrames().begin();

			while( i!= specAnalysis.GetResultingFrames().end() )
			{
				i->ToDB();

				peakDetector.Do( *i, tmpPeakArray );
				detectedPeaks.GetDetectedPeaks().push_back( tmpPeakArray );

				i++;
			}

			peakDetector.Stop();

			// Let's check that the detected peaks resemble the reference peaks
			std::ofstream log;

			log.open( "TestSpectralPeakDetect_DetectsSamePeaksFromNoiseSignal.log" );

			
			CLAMExamples::SpectralPeaksSet::SpectralPeaksList::iterator k =
				smReferenceSpectralPeaksSet.GetDetectedPeaks().begin();

			CLAMExamples::SpectralPeaksSet::SpectralPeaksList::iterator l =
				detectedPeaks.GetDetectedPeaks().begin();

			bool everythingWentOK = true;
			int  frameCounter = 0;

			while ( k != smReferenceSpectralPeaksSet.GetDetectedPeaks().end() 
				&& l != detectedPeaks.GetDetectedPeaks().end() )
			{
				
				CLAM::DataArray flattenedReference;
				CLAM::DataArray flattenedResult;
				
				flattenSpectralPeakArray( *k,
							  flattenedReference );
				
				flattenSpectralPeakArray( *l,
							  flattenedResult );								

				try
				{

					double similarity = evaluateSimilarity( flattenedReference,
										flattenedResult );
					
					if ( smSimilarityThreshold > similarity )
					{
						log << "Frame #" << frameCounter << ": Was dissimilar ";
						log << "similarity index was " << similarity*100. << "%" << std::endl;
						everythingWentOK = false;
					}
				}
				catch( CLAMTest::DataSizeMismatch& error )
				{
					log << "Frame #" << frameCounter << ": Could not be compared ";
					log << "revise spectral peak arrays visually" << std::endl;
					everythingWentOK = false;
				}

				frameCounter++;

				k++;
				l++;
			}

			if ( frameCounter != smReferenceSpectralPeaksSet.GetDetectedPeaks().size() )
			{
				log << "there was a difference of ";
				log << smReferenceSpectralPeaksSet.GetDetectedPeaks().size() - frameCounter;
				log << " between reference peaks in frame list and the actual" << std::endl;
			}

			log.close();

			CPPUNIT_ASSERT_EQUAL( true, everythingWentOK );
		}
		
	};

	double SpectralPeakDetectFunctionalTest::smSimilarityThreshold = 0.9999;
	bool   SpectralPeakDetectFunctionalTest::smBack2BackDataLoaded = false;
	CLAM::Spectrum SpectralPeakDetectFunctionalTest::smReferenceSpectrum;
	CLAM::SpectralPeakArray SpectralPeakDetectFunctionalTest::smReferenceSpectralPeakArray;
	CLAMExamples::SpectralPeaksSet  SpectralPeakDetectFunctionalTest::smReferenceSpectralPeaksSet;
}
