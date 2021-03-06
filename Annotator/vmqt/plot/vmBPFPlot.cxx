/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "vmPlotCanvas.hxx"
#include "vmBPFEditor.hxx"
#include "vmBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFPlot::BPFPlot(QWidget* parent)
			: MultiBPFPlot(parent)
		{
			InitBPFPlot();
		}

		BPFPlot::~BPFPlot()
		{
		}

		void BPFPlot::SetData(BPF* bpf)
		{
			AddBPF("bpf_editor",bpf);
		}

		void BPFPlot::SetFlags(int f)
		{
			SetFlags("bpf_editor",f);
		}

		void BPFPlot::backgroundWhite()
		{
			MultiBPFPlot::backgroundWhite();
			SetColors("bpf_editor",QColor(0,0,0),QColor(0,0,255));
		}

		void BPFPlot::backgroundBlack()
		{
			MultiBPFPlot::backgroundBlack();
			SetColors("bpf_editor",QColor(255,255,255),QColor(255,0,0));
		}

		void BPFPlot::xvalue_changed(QString key, unsigned index, double value)
		{
			emit xValueChanged(index,value);
		}

		void BPFPlot::yvalue_changed(QString key, unsigned index, double value)
		{
			emit yValueChanged(index,value);
		}

		void BPFPlot::element_added(QString key, unsigned index, double xvalue, double yvalue)
		{
			emit elementAdded(index,xvalue,yvalue);
		}

		void BPFPlot::element_removed(QString key, unsigned index)
		{
			emit elementRemoved(index);
		}

		void BPFPlot::setCurrentBPF(QString key)
		{
			MultiBPFPlot::setCurrentBPF(key);
		}

		void BPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			MultiBPFPlot::AddBPF(key,bpf);
		}

		void BPFPlot::SetColors(const QString& key, const QColor& cline, const QColor& chandler)
		{
			MultiBPFPlot::SetColors(key,cline,chandler);
		}

		void BPFPlot::SetFlags(const QString& key, int flags)
		{
			MultiBPFPlot::SetFlags(key,flags);
		}

		void BPFPlot::InitBPFPlot()
		{
			AddBPF("bpf_editor",0);
			setCurrentBPF("bpf_editor");
			connect(mBpfEditors[0], SIGNAL(xValueChanged(QString,unsigned,double)),
				this, SLOT(xvalue_changed(QString,unsigned,double)));
			connect(mBpfEditors[0], SIGNAL(yValueChanged(QString,unsigned,double)),
				this, SLOT(yvalue_changed(QString,unsigned,double)));
			connect(mBpfEditors[0],	SIGNAL(elementAdded(QString,unsigned,double,double)),
				this, SLOT(element_added(QString,unsigned,double,double)));
			connect(mBpfEditors[0], SIGNAL(elementRemoved(QString,unsigned)),
				this, SLOT(element_removed(QString,unsigned)));
		}
	}
}

// END

