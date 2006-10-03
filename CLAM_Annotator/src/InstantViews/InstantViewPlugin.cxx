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

#include "InstantViewPlugin.hxx"
#include "Project.hxx"
#include "Tonnetz.hxx"
#include "KeySpace.hxx"

#include <string>
#include <map>
#include <QtGui/QInputDialog>

static InstantViewPlugin::PluginMap plugins;

class Initializer
{
public:
	Initializer();
};

static Initializer initializer;

InstantViewPlugin::~InstantViewPlugin() {}

std::list<std::string> InstantViewPlugin::availablePlugins()
{
	std::list<std::string> list;
	for (PluginMap::iterator it = plugins.begin(); it!=plugins.end(); it++)
		list.push_back(it->first);
	return list;
}

/// Concrete plugins


class TonnetzPlugin : public InstantViewPlugin
{
private:
	CLAM::VM::Tonnetz * _view;
public:
	TonnetzPlugin()
		: _view(0)
	{}
	~TonnetzPlugin()
	{
		if (_view) delete _view;
	}
private:
	virtual const char * id() const { return "Tonnetz"; }
	virtual QString name() const { return QObject::tr("Tonnezt"); }
	CLAM::VM::InstantView * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		_view =  new CLAM::VM::Tonnetz(parent);
		CLAM::VM::FloatArrayDataSource * dataSource = new CLAM::VM::FloatArrayDataSource;
		dataSource->setSource(project, config.GetAttributeScope(), config.GetAttributeName());
		_view->setSource( dataSource );
		return _view;
	}
	virtual bool configureDialog(const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		QStringList attributeList;
		const std::list<std::string>& divisionNames = project.GetNamesByScopeAndType("Song", "FrameDivision");
		std::list<std::string>::const_iterator divisionName;
		for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
		{
			const std::string & frameDivisionChildScope = project.GetAttributeScheme("Song", *divisionName).GetChildScope();
			const std::list<std::string>& descriptorsNames = project.GetNamesByScopeAndType(frameDivisionChildScope, "FloatArray");
			std::list<std::string>::const_iterator it;
			for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
			{
				const CLAM_Annotator::SchemaAttribute & attribute = project.GetAttributeScheme(frameDivisionChildScope,*it);
				if (!attribute.HasBinLabels()) continue;
				unsigned binSize = attribute.GetBinLabels().size();
				if (binSize!=12 && binSize!=24) continue; 
				attributeList << (frameDivisionChildScope+"::"+*it).c_str();
			}
		}
		if (attributeList.size()==0) return false;
		bool ok=false;
		QString chosen = QInputDialog::getItem(0, QObject::tr("Choose the attribute to take the pitches from"),
				QObject::tr("PCP attribute:"), attributeList, 0, false, &ok);
		if (!ok) return false;
		QStringList splitted = chosen.split("::");
		config.SetAttributeScope(splitted[0].toStdString());
		config.SetAttributeName(splitted[1].toStdString());
		return true;
	}

	virtual void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
	{
		_view->updateData(data, samplingRate);
	}
	virtual void clearData()
	{
		_view->clearData();
	}
	virtual void setCurrentTime(double timeMiliseconds)
	{
		_view->setCurrentTime(timeMiliseconds);
	}
};

class KeySpacePlugin : public InstantViewPlugin
{
private:
	CLAM::VM::KeySpace * _view;
public:
	KeySpacePlugin()
		: _view(0)
	{}
	~KeySpacePlugin()
	{
		if (_view) delete _view;
	}
private:
	virtual const char * id() const { return "KeySpace"; }
	virtual QString name() const { return QObject::tr("Key Space"); }
	CLAM::VM::InstantView * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		_view = new CLAM::VM::KeySpace(parent);
		CLAM::VM::FloatArrayDataSource * dataSource = new CLAM::VM::FloatArrayDataSource;
		dataSource->setSource(project, config.GetAttributeScope(), config.GetAttributeName());
		_view->setSource( dataSource );
		return _view;
	}
	virtual bool configureDialog(const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		QStringList attributeList;
		const std::list<std::string>& divisionNames = project.GetNamesByScopeAndType("Song", "FrameDivision");
		std::list<std::string>::const_iterator divisionName;
		for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
		{
			const std::string & frameDivisionChildScope = project.GetAttributeScheme("Song", *divisionName).GetChildScope();
			const std::list<std::string>& descriptorsNames = project.GetNamesByScopeAndType(frameDivisionChildScope, "FloatArray");
			std::list<std::string>::const_iterator it;
			for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
			{
				const CLAM_Annotator::SchemaAttribute & attribute = project.GetAttributeScheme(frameDivisionChildScope,*it);
				if (!attribute.HasBinLabels()) continue;
				unsigned binSize = attribute.GetBinLabels().size();
				if (binSize!=24) continue; 
				attributeList << (frameDivisionChildScope+"::"+*it).c_str();
			}
		}
		if (attributeList.size()==0) return false;
		bool ok=false;
		QString chosen = QInputDialog::getItem(0, QObject::tr("Choose the attribute to take the pitches from"),
				QObject::tr("PCP attribute:"), attributeList, 0, false, &ok);
		if (!ok) return false;
		QStringList splitted = chosen.split("::");
		config.SetAttributeScope(splitted[0].toStdString());
		config.SetAttributeName(splitted[1].toStdString());
		return true;
	}

	virtual void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
	{
		_view->updateData(data, samplingRate);
	}
	virtual void clearData()
	{
		_view->clearData();
	}
	virtual void setCurrentTime(double timeMiliseconds)
	{
		_view->setCurrentTime(timeMiliseconds);
	}
};

Initializer::Initializer()
{
	plugins["KeySpace"] = new KeySpacePlugin;
	plugins["Tonnetz"] = new TonnetzPlugin;
}

InstantViewPlugin * InstantViewPlugin::createPlugin(const std::string & type)
{
	if (type=="KeySpace") return new KeySpacePlugin;
	if (type=="Tonnetz") return new TonnetzPlugin;
	return 0;
}



