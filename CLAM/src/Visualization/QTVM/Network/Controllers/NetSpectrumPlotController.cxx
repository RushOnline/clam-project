#include "SpecTypeFlags.hxx"
#include "NetSpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlotController::NetSpectrumPlotController()
		{
			SetvRange(TData(-150.0),TData(0.0));
		}

		NetSpectrumPlotController::~NetSpectrumPlotController()
		{	
		}

		void NetSpectrumPlotController::SetData(const Spectrum& spec)
		{
			_spec = spec;
			CacheData();
			FullView();
		}

		void NetSpectrumPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetSpectrumPlotController::Draw()
		{
			_renderer.Render();
		}

		void NetSpectrumPlotController::FullView()	
		{
		    _view.left = TData(0.0);
		    _view.right = TData(GetnSamples());
		    _view.top = GetvMax();
		    _view.bottom = GetvMin();
		    emit sendView(_view);
		}

		void NetSpectrumPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			_spec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				// check for conversions
				if(spFlags.bComplex)
				{
					_spec.Complex2MagPhase();
				}
				else if(spFlags.bPolar)
				{
					_spec.Polar2MagPhase();
				}
				else if(spFlags.bMagPhaseBPF)
				{
					_spec.BPF2MagPhase();
				}
			}
			// convert to dB
			_spec.ToDB();
		}

		void NetSpectrumPlotController::CacheData()
		{
			AdaptSpectralData();
			TSize specSize = _spec.GetMagBuffer().Size();
			_renderer.SetDataPtr(_spec.GetMagBuffer().GetPtr(), specSize, NormalMode);
			SetnSamples(specSize);
		}

	}
}

// END

