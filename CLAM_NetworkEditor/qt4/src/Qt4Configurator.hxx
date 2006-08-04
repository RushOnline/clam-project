/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef Qt4Configurator_hxx
#define Qt4Configurator_hxx

#include <CLAM/ConfigurationVisitor.hxx>
#include <map>
#include <string>

#include <CLAM/Assert.hxx>
#include <CLAM/Enum.hxx>
#include <CLAM/BPF.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/DynamicType.hxx>

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <CLAM/QtEnvelopeEditor.hxx>

namespace CLAM{
	/**
	 * A generic DynamicType edition dialog for Qt.
	 * @ingroup Configurators
	 * @see DynamicType, FLTKConfigurator
	 */
	class Qt4Configurator : public QDialog {
		Q_OBJECT
		typedef QDialog super;
		typedef std::map<std::string, QWidget*> tWidgets;
	public:
		Qt4Configurator(QWidget * parent = NULL) 
			: super(parent)
			
		{
			mSetter = 0;
			mGetter = 0;
			mLayout = 0;
		}

		virtual ~Qt4Configurator();
		template <class Config>
		void SetConfig(Config & config) {
			CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
			CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
			mSetter = new ConfigurationSetter<Config,Qt4Configurator>(&config, this);
			mGetter = new ConfigurationGetter<Config,Qt4Configurator>(&config, this);

			CLAM_ASSERT(!mLayout, "Configurator: Configuration assigned twice");
			mLayout = new QVBoxLayout(this);
			mLayout->setSpacing(3);
			mLayout->setMargin(5);
			//mLayout->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
			setMinimumWidth(400);
			GetInfo();

			QFrame * frame = new QFrame(this);
			mLayout->addWidget(frame);
			frame->setMinimumHeight(10);

			QHBoxLayout * buttons = new QHBoxLayout;
			mLayout->addLayout(buttons);
			buttons->setMargin(5);
			buttons->setSpacing(3);
			
			QPushButton * applyButton = new QPushButton("Apply", this);
			buttons->addWidget(applyButton);
			applyButton->setAutoDefault(false);
			connect( applyButton, SIGNAL(clicked()), this, SLOT(Apply()) );

			QPushButton * discardButton = new QPushButton("Discard", this);
			discardButton->setAutoDefault(false);
			buttons->addWidget(discardButton);
			connect( discardButton, SIGNAL(clicked()), this, SLOT(Discard()) );

			QPushButton * okButton = new QPushButton("Ok", this);
			buttons->addWidget(okButton);
			connect( okButton, SIGNAL(clicked()), this, SLOT(Ok()) );

			buttons->setStretchFactor(applyButton,2);
			buttons->setStretchFactor(discardButton,2);
			buttons->setStretchFactor(okButton,2);

			adjustSize();
		}
	private:

		void GetInfo() {
			CLAM_ASSERT(mGetter,"Configurator: No config to set");
			mGetter->VisitConfig();
		}
		void SetInfo() {
			CLAM_ASSERT(mSetter,"Configurator: No config to set");
			mSetter->VisitConfig();
		}

		QWidget * GetWidget(const char * name) {
			tWidgets::iterator found=mWidgets.find(name);
			if (found==mWidgets.end()) return NULL;
			return found->second;
		}
		void PushWidget(const char * name, QWidget * widget)
		{
			mWidgets.insert(tWidgets::value_type(name, widget));
		}

	public:

		/** Default implementation, do nothing */
		template <typename T>
		void AddWidget(const char *name, void *foo, T& value) {
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			QLabel * mInput = new QLabel("Non editable attribute");
			cell->addWidget(mInput);
			PushWidget(name,mInput);
		}
		/** Default implementation, do nothing */
		template <typename T>
		void RetrieveValue(const char *name, void *foo, T& value) {
		}

		template <typename T>
		void AddWidget(const char *name, std::string *foo, T& value) {
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name, cell));
			QLineEdit * mInput = new QLineEdit(QString(value.c_str()), cell);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, std::string *foo, T& value) {
			QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->text().toStdString();
		}

		template <typename T>
		void AddWidget(const char *name, TData *foo, T& value) {
			QDoubleSpinBox * mInput = new QDoubleSpinBox;
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, TData *foo, T& value) {
			QDoubleSpinBox * mInput = dynamic_cast<QDoubleSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, TSize *foo, T& value) {
			QSpinBox * mInput = new QSpinBox;
			mInput->setMinimum(0);
			mInput->setValue(value);

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, TSize *foo, T& value) {
			QSpinBox * mInput = dynamic_cast<QSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, bool *foo, T& value) {
			QPushButton * mInput = new QPushButton(name, this);
			mLayout->addWidget(mInput);
			mInput->setCheckable(true);
			mInput->setChecked(value);
			mInput->setAutoDefault(false);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, bool *foo, T& value) {
			QPushButton * mInput = dynamic_cast<QPushButton*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->isChecked();
		}

		template <typename T>
		void AddWidget(const char *name, Enum *foo, T& value) {
			QComboBox * mInput = new QComboBox(/*editable*/false);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mInput->addItem( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mInput->setCurrentIndex(i);
			}
			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, Enum *foo, T& value) {
			QComboBox * mInput = dynamic_cast<QComboBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			CLAM_BEGIN_CHECK
				int i;
				for (i = 0; mapping[i].name; i++);
				CLAM_ASSERT(mInput->currentIndex()<i, "Configurator: Choice value out of range");
			CLAM_END_CHECK
			value=mapping[mInput->currentIndex()].value;
		}

		template <typename T>
		void AddWidget(const char *name, DynamicType *foo, T&value) {
			QPushButton * mInput = new QPushButton("Details...");
			mInput->setAutoDefault(false);
			Qt4Configurator * subConfigurator = new Qt4Configurator(this);
			subConfigurator->SetConfig(value);
			connect( mInput, SIGNAL(clicked()), subConfigurator, SLOT(show()) );

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}
		template <typename T>
		void RetrieveValue(const char *name, DynamicType *foo, T&value) {
		}

		template <typename T>
		void AddWidget(const char *name, BPF *foo, T& value) {
			QtEnvelopeEditor* mInput = new QtEnvelopeEditor;
			// Those ones should be BPF properties
			mInput->BoundMinX( 0 );
			mInput->BoundMaxX( 22050 );
			mInput->BoundMinY( 0 );
			mInput->BoundMaxY( 1000 );
			mInput->SetValue(value);
			mInput->setMinimumSize( QSize( 350, 350) );

			QHBoxLayout * cell = new QHBoxLayout;
			mLayout->addLayout(cell);
			cell->addWidget(new QLabel(name));
			cell->addWidget(mInput);
			PushWidget(name, mInput);
		}

		template <typename T>
		void RetrieveValue(const char *name, BPF *foo, T& value) {
			QtEnvelopeEditor * mInput = dynamic_cast<QtEnvelopeEditor*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->GetValue();
		}

	public slots:

		void Apply() {
			SetInfo();
		}
		void Discard() {
			close();
		}
		void Ok() {
			Apply();
			Discard();
		}
	public:
	
		void show() {
			super::show();
		}

	private:
		QVBoxLayout * mLayout;
		ConfigurationVisitor * mGetter;
		ConfigurationVisitor * mSetter;
		tWidgets mWidgets;

	};
}
#endif//Qt4Configurator_hxx
