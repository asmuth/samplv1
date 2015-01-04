// samplv1_config.h
//
/****************************************************************************
   Copyright (C) 2012-2015, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef __samplv1_config_h
#define __samplv1_config_h

#include "config.h"

#define SAMPLV1_TITLE	PACKAGE_NAME
#define SAMPLV1_VERSION	PACKAGE_VERSION

#define SAMPLV1_SUBTITLE     "an old-school polyphonic sampler."
#define SAMPLV1_WEBSITE      "http://samplv1.sourceforge.net"
#define SAMPLV1_COPYRIGHT    "Copyright (C) 2012-2015, rncbc aka Rui Nuno Capela. All rights reserved."

#define SAMPLV1_DOMAIN	"rncbc.org"


//-------------------------------------------------------------------------
// samplv1_config - Prototype settings class (singleton).
//

#include <QSettings>
#include <QStringList>

// forward decls.
class samplv1_programs;


class samplv1_config : public QSettings
{
public:

	// Constructor.
	samplv1_config();

	// Default destructor.
	~samplv1_config();

	// Default options...
	QString sPreset;
	QString sPresetDir;
	QString sSampleDir;

	// Special persistent options.
	bool bUseNativeDialogs;

	// Run-time special non-persistent options.
	bool bDontUseNativeDialogs;

	// Singleton instance accessor.
	static samplv1_config *getInstance();

	// Preset utility methods.
	QString presetFile(const QString& sPreset);
	void setPresetFile(const QString& sPreset, const QString& sPresetFile);
	void removePreset(const QString& sPreset);
	QStringList presetList();

	// Programs utility methods.
	void loadPrograms(samplv1_programs *pPrograms);
	void savePrograms(samplv1_programs *pPrograms);

	void loadProgramsCurrent(samplv1_programs *pPrograms);
	void saveProgramsCurrent(samplv1_programs *pPrograms);

protected:

	// Explicit I/O methods.
	void load();
	void save();

	// Preset group path.
	QString presetGroup() const;

	// Banks programs group path.
	QString programsGroup() const;
	QString bankPrefix() const;
	QString currentGroup() const;

	void clearPrograms();

private:

	// The singleton instance.
	static samplv1_config *g_pSettings;
};


#endif	// __samplv1_config_h

// end of samplv1_config.h
